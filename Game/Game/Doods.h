#pragma once
#ifndef OBJECT_H
	#define OBJECT_H
	#include "Object.h"
#endif

class Hitbox;
class Hurtbox;

//class for all world occupants(ai and players)
class Dood: public Object
{
	public:
		Dood(Point p, Point r, float w, float h, Vector rgb, bool s, int health) : Object(p, r, w, h, rgb, s, DOOD), damage(0), hp(health), alive(true), hitbox(NULL),
		crouchHeight(3.0f), standHeight(h), standWidth(w), crawlHeight(standHeight/4.0f), crawlWidth(h), walkSpeed(0.002f), runSpeed(0.005f), currentSpeed(runSpeed), maxSpeed(runSpeed*2), 
		jumpHeight(0.01f), bumpFlag(false)
		{
		};

		int damage;
		int hp;

		float standHeight;
		float crouchHeight;
		float crawlHeight;

		float standWidth;
		float crawlWidth;

		float walkSpeed;
		float runSpeed;
		float currentSpeed;
		float maxSpeed;
		float jumpHeight;

		bool alive;
		bool bumpFlag;

		Hurtbox* hurtbox;
		Hitbox*  hitbox;

		void Attack(void);
};

void DoodsUpdate(void);

extern std::list<Dood*> doods;