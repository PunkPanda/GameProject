#pragma once
#include "Point.h"
#include "Vector.h"


#include <Windows.h>
#include "Graphics.h"

#include <list>
#include <vector>

class Object
{
	public:
		Point pos;

		Point rot;

		Vector vel;

		float width;
		float height;

		Vector rgb;
		
		bool Static;

		int gridCollision;

		GLuint texture[1];

		enum classType
		{
			BASE,
			PLAYER,
			A_I,
			DOOD,
			WEAPON,
			HITBOX,
			HURTBOX,
			DOOR
		}classType;

		bool active;

		Object(Point Pos, Point Rot, float w, float h, Vector RGB, bool Stat, enum classType type) : 
		pos(Pos), rot(Rot), width(w), height(h), rgb(RGB), Static(Stat), vel(0, 0, 0), classType(type)
		{
		}
};

extern std::list<Object*> objects;

void destroyObject(Object* obj);