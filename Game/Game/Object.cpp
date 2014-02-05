#ifndef OBJECT_H
	#define OBJECT_H
	#include "Object.h"
#endif

std::list<Object*> objects;

void destroyObject(Object* obj)
{
	delete obj;
}
