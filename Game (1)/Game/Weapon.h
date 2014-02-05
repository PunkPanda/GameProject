#pragma once
#ifndef OBJECT_H
	#define OBJECT_H
	#include "Object.h"
#endif

class Weapon : public Object
{
	public:
		Weapon(Point p, Point r, float w, float h, Vector rgb, bool s, float cHeight) : Object(p, r, w, h, rgb, s, WEAPON)
		{
			classType = WEAPON;
		};

		enum Type
		{
			SWORD,
			AXE,
			MACE,
			HAMMER,
			SPEAR,
			STAFF,
			WAND,
			TOME,
			BOW,
			KNIFE
		}type;
};

void createWeapon(Point p, Point r, float w, float h, Vector rgb, bool s);