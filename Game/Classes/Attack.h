#pragma once
class Entity;



class Attack {
public:
	Entity* caster;
	float cooldown;
	float last_time;
	void (*attack_func)(Attack* this_attack);

	Attack() {};

	Attack(Entity* caster, float cooldown, void (*attack_func)(Attack* this_attack)) {
		this->caster = caster;
		this->cooldown = cooldown;
		this->last_time = 0;
		this->attack_func = attack_func;
	}


	void Exec(float atual_time) {
		if ((last_time + cooldown) > atual_time)
			return;
		last_time = atual_time;
		attack_func(this);
	}


};