#pragma once
#ifndef OBJECT_H
	#define OBJECT_H
	#include "Object.h"
#endif

class Hitbox;
class Hurtbox;

class Dood: public Object
{
	public:
		Dood(Point p, Point r, float w, float h, Vector rgb, bool s, int health) : Object(p, r, w, h, rgb, s, DOOD), damage(0), hp(health), alive(true), hitbox(NULL)
		{
		};

		int damage;
		int hp;

		bool alive;
		Hurtbox* hurtbox;
		Hitbox*  hitbox;

		void Attack(void);
};

void DoodsUpdate(void);

extern std::list<Dood*> doods;