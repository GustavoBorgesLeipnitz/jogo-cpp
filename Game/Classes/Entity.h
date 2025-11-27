#pragma once
#include "../../api/api.h"
#include "../../BaseClass/Lista.h"
class Attack;


class Entity {
public:
	float maxHealth;
	float health;
	bool alive;
	float hora_da_morte;
	float speed;
	float damage;
	Vector pos;
	UiVector hitbox_size;
	Color color;
	Lista<Attack*> attacks;

	void (*on_death)(Entity* this_entity);
	void (*on_collision)(Entity* this_entity, Entity* colidida);
	Entity(float maxHealth, float damage, float speed, Vector pos, UiVector hitbox_size, Color color);
	void Add_Attack(float cooldown, void (*attack_func)(Attack* this_attack));
	void TakeDamage(float amount, float atual_time);
	void Heal(float amount);
	void On_Collision(Entity* colidida);
	void GiveDMG(Entity* target, float amount, float atual_time);
	void ExecAttacks(float atual_time);

	~Entity();
};