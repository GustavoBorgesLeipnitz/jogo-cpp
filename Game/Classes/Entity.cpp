#include "Entity.h"
#include "Attack.h"


Entity::Entity(float maxHealth, float damage, float speed, Vector pos, UiVector hitbox_size, Color color) {
	this->maxHealth = maxHealth;
	this->health = maxHealth;
	this->alive = true;
	this->speed = speed;
	this->damage = damage;
	this->pos = pos;
	this->hitbox_size = hitbox_size;
	this->color = color;
	this->hora_da_morte = 0;
}

void Entity::Add_Attack(float cooldown, void (*attack_func)(Attack* this_attack)) {
	attacks.Add(new Attack(this, cooldown, attack_func));
}
void Entity::TakeDamage(float amount, float atual_time) {
	health -= amount;
	if (health <= 0) {
		health = 0;
		if (alive && on_death) {
			alive = false;
			on_death(this);
			hora_da_morte = atual_time;
		}
	}
}
void Entity::Heal(float amount) {
	health += amount;
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Entity::On_Collision(Entity* colidida) {
	if (on_collision)
		on_collision(this, colidida);
}


void Entity::GiveDMG(Entity* target, float amount, float atual_time) {
	target->TakeDamage(amount, atual_time);
}

void Entity::ExecAttacks(float atual_time) {
	for (int i = 0; i < attacks.size; i++) {
		attacks.Get(i)->Exec(atual_time);
	}
};

Entity::~Entity() {
	for (int i = attacks.size-1; i >= 0; i--) {
		//delete attacks.Get(i);
	}
}