#pragma once
#include "../../api/api.h"
#include <Windows.h>
#include <iostream>

class TempText {
	std::string text;
	Vector atual_pos;
	Vector target_pos;
	float max_time_alive;
	float timer;
	float speed;
	bool alive;
	bool centered;
	Color color;
	bool on_destiny;

	void (*on_death)(TempText* thisText);
	void (*in_targetpos)(TempText* thisText);

	TempText(std::string text, Vector startpos = Vector(), Vector endpos = Vector(), float speed = 5, float time_alive = -1, bool centered = true, Color color = Color(255, 255, 255)) {
		this->text = text;
		this->atual_pos = startpos;
		this->target_pos = endpos;
		this->speed = speed;
		this->timer = 0;
		this->max_time_alive = time_alive;
		this->alive = true;
		this->centered = centered;
		this->color = color;
		this->on_destiny = false;
	}

	void UpdatePos(float DeltaTime) {
		if (!alive || on_destiny) {
			return;
		}
		if (max_time_alive != -1) {
			timer += DeltaTime;
			if (timer >= max_time_alive) {
				alive = false;
				On_Death();
				return;
			}
		}
		if (target_pos.Dist(atual_pos) < DeltaTime * speed) {
			atual_pos = target_pos;
			on_destiny = true;
			In_TargetPos();
			return;
		}
		Vector dif = target_pos - atual_pos;
		float soma = fabs(dif.x) + fabs(dif.y);
		Vector dir = dif / soma;
		atual_pos = atual_pos + dir * DeltaTime * speed;
	}

	void Render() {

	}

	void On_Death() {
		if (on_death) {
			on_death(this);
		}
	}

	void In_TargetPos() {
		if (in_targetpos) {
			in_targetpos(this);
		}
	}


};