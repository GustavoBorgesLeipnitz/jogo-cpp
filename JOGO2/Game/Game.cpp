#include "Game.h"
#include "../api/api.h"
#include "Classes/Entity.h"
#include "Classes/Projectile.h"
#include <string>
#include <chrono>
#include "../Misc/Misc.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "Classes/TempText.h"




Entity* Localplayer; 

Vector CameraPos;
Vector MouseDir;


Lista<Entity*> inimigos;
Lista<Projectile*> projectiles;
Lista<TempText> textos_temp;
float last_time_enemy_spawned = 0;
float spawn_enemy_cooldown = 0.15;
auto start_time = std::chrono::steady_clock::now();
float atual_time = 0;
int max_enemies = 200;
Entity* farest_enemy = nullptr;
Entity* nearest_enemy = nullptr;
int enemys_killed = 0;
int dead_enemies = 0;

void Game::Start() {
	Localplayer = new Entity(100, 50, 1000, Vector(), UiVector(50, 50), Color(0, 0, 255));
	Localplayer->Add_Attack(0.1, [](Attack* this_attack) {
		for (int i = 0; i < inimigos.size; i++) {
			Entity* enemy = inimigos.Get(i);
			if (enemy->pos.Dist(Localplayer->pos) < 200) {
				Localplayer->GiveDMG(enemy, 100, GetAtualTime());
			}
		}
	});
	/*Localplayer->Add_Attack(1, [](Attack* this_attack) {
		if (GetNearestEnemy()) {
			Projectile* projectile = new Projectile(this_attack, Localplayer->pos, UiVector(8, 8), Color(200, 255), 900, -1, 5, nearest_enemy);
			projectile->on_collision = [](Projectile* projectile, Entity* entity) {
				if (entity != Localplayer) {
					Localplayer->GiveDMG(entity, 100, GetAtualTime());
					projectile->alive = false;
				}
			};
			projectiles.Add(projectile);
		}
	});*/
	Localplayer->Add_Attack(0.2, [](Attack* this_attack) {
		if (GetNearestEnemy()) {
			Projectile* projectile = new Projectile(this_attack, Localplayer->pos, UiVector(30, 30), Color(168, 119, 50), 900, -1, 5, MouseDir);
			projectile->on_collision = [](Projectile* projectile, Entity* entity) {
				if (entity != Localplayer) {
					Localplayer->GiveDMG(entity, 100, GetAtualTime());
					projectile->alive = false;
				}
				};
			projectiles.Add(projectile);
		}
		});
	Localplayer->on_death = [](Entity* this_entity) {
	
	};

}

void Game::Render() {
	UpdateCameraPos();
	ClearScreen(Color()); // background
	RenderCircle((*GetWindowSize()) / 2, 200, Color(10, 0, 10));
	//RenderCircleOutline((*GetWindowSize()) / 2, 200, Color(50, 50, 50));
	RenderEntity(Localplayer->pos, Localplayer->hitbox_size, Localplayer->color, Localplayer->health, Localplayer->maxHealth);
	RenderEntities();
	RenderProjectiles();
}


void Game::Main(float DeltaTime) {
	if (!Localplayer->alive) {
		ShowLoseScreen();
		return;
	}
	Update_atual_time();
	UpdateMouseDir();
	LocalPlayerMovement(DeltaTime);
	UpdateNearestEntity();
	ExecAttacks();
	SpawnEnemies();
	EnemyMovement(DeltaTime);
	UpdateFarestEntity();
	MoveProjectiles(DeltaTime);
	VerifyCollisions();
	VerifyProjectileCollisions();
	DeleteDeadEnemies();
	DeleteDeadProjectiles();
	Render();
	RenderText("inimigos: " + std::to_string(inimigos.size), UiVector(50, 20), Color(255, 255, 255));
	RenderText("alive: " + std::to_string(inimigos.size - dead_enemies), UiVector(50, 40), Color(255, 255, 255));
	RenderText("dead: " + std::to_string(dead_enemies), UiVector(50, 60), Color(255, 255, 255));
	RenderText("kills: " + std::to_string(enemys_killed), UiVector(50, 80), Color(255, 255, 255));
	RenderText("projectiles: " + std::to_string(projectiles.size), UiVector(50, 100), Color(255, 255, 255));
	RenderText("x: " + std::to_string(Localplayer->pos.x), UiVector(50, 120), Color(255, 255, 255));
	RenderText("y: " + std::to_string(Localplayer->pos.y), UiVector(50, 140), Color(255, 255, 255));

	RenderText("x: " + std::to_string(GetMousePos()->x), UiVector(50, 200), Color(255, 255, 255));
	RenderText("y: " + std::to_string(GetMousePos()->y), UiVector(50, 220), Color(255, 255, 255));


	RenderText("x: " + std::to_string(MouseDir.x), UiVector(50, 260), Color(255, 255, 255));
	RenderText("y: " + std::to_string(MouseDir.y), UiVector(50, 280), Color(255, 255, 255));

	RenderText("x: " + std::to_string(GetWindowPos()->x), UiVector(50, 420), Color(255, 255, 255));
	RenderText("y: " + std::to_string(GetWindowPos()->y), UiVector(50, 440), Color(255, 255, 255));

	RenderText("x: " + std::to_string(GetWindowSize()->x), UiVector(50, 500), Color(255, 255, 255));
	RenderText("y: " + std::to_string(GetWindowSize()->y), UiVector(50, 520), Color(255, 255, 255));
	
}


void Game::ShowLoseScreen() {
	ClearScreen(Color());
	RenderText("YOU LOSE", (*GetWindowSize()) / 2, Color(255, 255, 255));
}


void Game::RenderEntity(Vector pos, UiVector hitbox_size, Color color, float health, float maxHealth) {
	int relativeX = pos.x - CameraPos.x;
	int relativeY = CameraPos.y - pos.y;

	UiVector* screensize = GetWindowSize();

	relativeX += screensize->x / 2;
	relativeY += screensize->y / 2;

	if ((relativeX - hitbox_size.x / 2) > screensize->x) {
		return;
	}
	if ((relativeX + hitbox_size.x / 2) < 0) {
		return;
	}
	if ((relativeY - hitbox_size.y / 2) > screensize->y) {
		return;
	}
	if ((relativeY + hitbox_size.y / 2) < 0) {
		return;
	}


	RenderRect(UiVector(relativeX, relativeY), hitbox_size, color, true);
	if (health) {
		RenderRect(UiVector(relativeX, relativeY - hitbox_size.y / 2 - 8), UiVector(hitbox_size.x * (health / maxHealth), 3), Color(0, 255, 0), true);
	}
}

Entity* Game::GetNearestEnemy() {
	return nearest_enemy;
}

void Game::UpdateMouseDir() {
	Vector mousepos = GetMousePos()->ToVector();
	Vector midscreen = GetWindowPos()->ToVector() + (GetWindowSize()->ToVector() / 2);
	Vector dif = Vector(mousepos.x - midscreen.x, midscreen.y - mousepos.y);
	float soma = fabs(dif.x) + fabs(dif.y);
	MouseDir = (dif / soma);
}

void Game::RenderEntities() {
	for (int i = 0; i < inimigos.size; i++) {
		Entity* inimigo = inimigos.Get(i);
		RenderEntity(inimigo->pos, inimigo->hitbox_size, inimigo->color, inimigo->health, inimigo->maxHealth);
	}
}

void Game::RenderProjectiles() {
	for (int i = 0; i < projectiles.size; i++) {
		Projectile* projectile = projectiles.Get(i);
		RenderEntity(projectile->pos, projectile->hitbox_size, projectile->color);
	}
}

void Game::UpdateCameraPos() {
	CameraPos = Localplayer->pos;
}

void Game::LocalPlayerMovement(float DeltaTime) {
	float speed = Localplayer->speed;
	Vector velocity = Vector();
	if (GetAsyncKeyState('W') & 0x8000) {
		velocity.y++;
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		velocity.x--;
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		velocity.y--;
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		velocity.x++;
	}

	float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if (length != 0) {
		velocity.x /= length;
		velocity.y /= length;
	}

	Localplayer->pos.x += velocity.x * speed * DeltaTime;
	Localplayer->pos.y += velocity.y * speed * DeltaTime;

};

void Game::UpdateNearestEntity() {
	nearest_enemy = nullptr;
	float dist = 9999999999999999999;
	for (int i = 0; i < inimigos.size; i++) {
		Entity* inimigo = inimigos.Get(i);
		if (!inimigo->alive)
			continue;
		float tempdist = inimigo->pos.Dist(Localplayer->pos);
		if (tempdist < dist) {
			dist = tempdist;
			nearest_enemy = inimigo;
		}
	}
}

void Game::UpdateFarestEntity() {
	Entity** tempinimigos = inimigos.GetValues();
	farest_enemy = nullptr;
	float dist = 0;
	for (int i = 0; i < inimigos.size; i++) {
		Entity* inimigo = tempinimigos[i];
		if (!inimigo->alive)
			continue;
		float tempdist = inimigo->pos.Dist(Localplayer->pos);
		if (tempdist > dist) {
			dist = tempdist;
			farest_enemy = inimigo;
		}
	}
	delete[] tempinimigos;
}

void Game::SpawnEnemies() {
	if ((last_time_enemy_spawned + spawn_enemy_cooldown) > GetAtualTime())
		return;

	last_time_enemy_spawned = GetAtualTime();
	float dir = RandomFloat(-180, 180);
	Vector posdif = Vector(sin(dir), cos(dir)) * 1000;

	if ((inimigos.size-dead_enemies) >= max_enemies && farest_enemy) {
		if (farest_enemy->pos.Dist(Localplayer->pos) > 1500) {
			farest_enemy->pos = Localplayer->pos + posdif;
		}
		return;
	}

	Entity* inimigo = new Entity(100, 2, 200, Localplayer->pos + posdif, UiVector(30, 30), Color(255));
	inimigo->on_collision = [](Entity* atual_entity, Entity* colidida) {
		if (!atual_entity->alive)
			return;
		atual_entity->GiveDMG(colidida, atual_entity->damage, GetAtualTime());
	};
	inimigo->on_death = [](Entity* atual_entity) {
		PlaySound(TEXT("C:\\Windows\\Media\\Windows Startup.wav"), NULL, SND_FILENAME | SND_ASYNC);
		atual_entity->color = Color(50, 0, 0);
		enemys_killed++;
		dead_enemies++;
	};
	inimigos.Add(inimigo);
}

void Game::EnemyMovement(float DeltaTime) {
	for (int i = 0; i < inimigos.size; i++) {
		Entity* inimigo = inimigos.Get(i);
		if (!inimigo->alive)
			continue;
		Vector dif = Localplayer->pos - inimigo->pos;
		float soma = fabs(dif.x) + fabs(dif.y);
		Vector direction = Vector(dif.x / soma, dif.y / soma);
		inimigo->pos = inimigo->pos + direction * inimigo->speed * DeltaTime;
	}
}


void Game::Update_atual_time() {
	atual_time = (float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count()/1000;
}

float Game::GetAtualTime() {
	return atual_time;
}

void Game::MoveProjectiles(float DeltaTime) {
	for (int i = 0; i < projectiles.size; i++) {
		Projectile* projectile = projectiles.Get(i);
		projectile->Move(DeltaTime);
	}
}

void Game::VerifyCollisions() {
	for (int i = 0; i < inimigos.size; i++) {
		Entity* inimigo = inimigos.Get(i);
		if (!inimigo->alive)
			continue;

		Vector dif = Vector(fabs(Localplayer->pos.x - inimigo->pos.x), fabs(Localplayer->pos.y - inimigo->pos.y));
		float X = inimigo->hitbox_size.x / 2 + Localplayer->hitbox_size.x / 2;
		float Y = inimigo->hitbox_size.y / 2 + Localplayer->hitbox_size.y / 2;
		if (dif.x < X && dif.y < Y) {
			Localplayer->On_Collision(inimigo);
			inimigo->On_Collision(Localplayer);
		}
	}
}

void Game::VerifyProjectileCollisions() {
	for (int i = 0; i < projectiles.size; i++) {
		Projectile* projectile = projectiles.Get(i);
		if (!projectile->alive)
			continue;

		Vector dif = Vector(fabs(Localplayer->pos.x - projectile->pos.x), fabs(Localplayer->pos.y - projectile->pos.y));
		float X = projectile->hitbox_size.x / 2 + Localplayer->hitbox_size.x / 2;
		float Y = projectile->hitbox_size.y / 2 + Localplayer->hitbox_size.y / 2;
		if (dif.x < X && dif.y < Y) {
			projectile->On_Collision(Localplayer);
		}

		for (int j = 0; j < inimigos.size; j++) {
			Entity* inimigo = inimigos.Get(j);
			if (!inimigo->alive)
				continue;
			Vector dif = Vector(fabs(inimigo->pos.x - projectile->pos.x), fabs(inimigo->pos.y - projectile->pos.y));
			float X = projectile->hitbox_size.x / 2 + inimigo->hitbox_size.x / 2;
			float Y = projectile->hitbox_size.y / 2 + inimigo->hitbox_size.y / 2;
			if (dif.x < X && dif.y < Y) {
				projectile->On_Collision(inimigo);
			}
		}
	}
}

void Game::DeleteDeadEnemies() {
	Entity** tempinimigos = inimigos.GetValues();
	for (int i = inimigos.size-1; i >= 0; i--) {
		Entity* inimigo = tempinimigos[i];
		if (!inimigo->alive && (GetAtualTime() - inimigo->hora_da_morte) > 5) {
			inimigos.Remove(i);
			delete inimigo;
			dead_enemies--;
		}
	}
	delete[] tempinimigos;
}
void Game::DeleteDeadProjectiles() {
	Projectile** tempprojectiles = projectiles.GetValues();
	for (int i = projectiles.size - 1; i >= 0; i--) {
		Projectile* projectile = tempprojectiles[i];
		if (!projectile->alive) {
			projectiles.Remove(i);
			delete projectile;
		}
	}
	delete[] tempprojectiles;
}

void Game::ExecAttacks() {
	Localplayer->ExecAttacks(atual_time);
}