#pragma once
#include "Classes/Entity.h"


namespace Game {
	void Start();
	void Main(float DeltaTime);
	void Render();
	void UpdateCameraPos();
	void RenderEntities();
	void RenderEntity(Vector pos, UiVector hitbox_size, Color color, float health = 0, float maxHealth = 0);
	void RenderProjectiles();
	void LocalPlayerMovement(float DeltaTime);
	void UpdateNearestEntity();
	void UpdateFarestEntity();
	void SpawnEnemies();
	void Update_atual_time();
	void EnemyMovement(float DeltaTime);
	void VerifyCollisions();
	void ShowLoseScreen();
	void ExecAttacks();
	void MoveProjectiles(float DeltaTime);
	void VerifyProjectileCollisions();
	void DeleteDeadEnemies();
	void DeleteDeadProjectiles();
	void UpdateMouseDir();
	Entity* GetNearestEnemy();
	float GetAtualTime();
}