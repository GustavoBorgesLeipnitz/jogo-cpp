#pragma once
#include "../../api/api.h"
class Attack;
class Entity;

class Projectile {
public:
	Attack* attack;
	Vector pos;
	Vector startpos;
	UiVector hitbox_size;
	Color color;
	Entity* target;
	Vector dir;
	float speed;
	float time_alive;
	float total_time;
	float maxdist;
	bool infinite_time;
	bool alive;
	void (*on_collision)(Projectile* this_projectile, Entity* entity);
	void (*on_death)(Projectile* this_projectile);

	Projectile(Attack* attack, Vector pos, UiVector hitbox_size, Color color, float speed, float maxdist, int time_alive, Vector dir) {
		this->attack = attack;
		this->pos = pos;
		this->startpos = pos;
		this->hitbox_size = hitbox_size;
		this->color = color;
		this->on_collision = on_collision;
		this->speed = speed;
		this->maxdist = maxdist;
		this->time_alive = time_alive;
		this->total_time = 0;
		this->infinite_time = time_alive == -1;
		this->dir = dir;
		this->target = nullptr;
		this->alive = true;
	}

	Projectile(Attack* attack, Vector pos, UiVector hitbox_size, Color color, float speed, float maxdist, int time_alive, Entity* target) {
		this->attack = attack;
		this->pos = pos;
		this->startpos = pos;
		this->hitbox_size = hitbox_size;
		this->color = color;
		this->on_collision = on_collision;
		this->speed = speed;
		this->maxdist = maxdist;
		this->time_alive = time_alive;
		this->total_time = 0;
		this->infinite_time = time_alive == -1;
		this->dir = Vector();
		this->target = nullptr;
		this->alive = true;
		this->target = target;

		Vector dif = target->pos - pos;
		float soma = fabs(dif.x) + fabs(dif.y);
		dir = dif / soma;
	}

	void Move(float DeltaTime) {
		if (!alive)
			return;
		if (!target || !target->alive) {
			target = nullptr;
		}
		else {
			Vector dif = target->pos - pos;
			float soma = fabs(dif.x) + fabs(dif.y);
			dir = dif / soma;
		}
		if (!infinite_time) {
			total_time += DeltaTime;
			if (total_time > time_alive) {
				alive = false;
				return;
			}
		}
		pos = pos + dir*DeltaTime*speed;
		if (maxdist != -1 && pos.Dist(startpos) > maxdist) {
			alive = false;
		}
	}


	void On_Collision(Entity* entity) {
		if (on_collision)
			on_collision(this, entity);
	}

	void On_Death() {
		if (on_death)
			on_death(this);

		this->alive = false;
	}




};