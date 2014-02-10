#pragma once
#ifndef OBJECT_H
	#define OBJECT_H
	#include "Object.h"
#endif

void PhysicsUpdate(void);
bool MapCollision(Object* obj);
void ApplyGravity(Object* obj);
void ApplyFriction(Object* obj);
int GridCollision(Point pos, float width, float height);
void SnapUp(float& Coordinate);
void SnapDown(float& Coordinate);
void RampSnapUpRight(Point& Coordinate, float width, float height);
void RampSnapUpLeft(Point& Coordinate, float width, float height);

//Collision flags
#define	COLLISION_LEFT		 1	//0000001
#define	COLLISION_RIGHT		 2	//0000010
#define	COLLISION_TOP		 4	//0000100
#define	COLLISION_BOTTOM	 8	//0001000
#define COLLISION_RAMP_RIGHT 16 //0010000
#define COLLISION_RAMP_LEFT	 32 //0100000
#define COLLISION_BUMP		 64 //1000000

extern unsigned doorCollide;
extern unsigned doorTimer;