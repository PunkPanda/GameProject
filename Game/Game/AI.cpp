#include "AI.h"
#include "Player.h"
#include "Physics.h"
#include "Combat.h"

std::list<AI*> AIlist;

void createAI(Point p, Point r, float w, float h, Vector rgb, enum AI::Type t)
{
	AI* ai = new AI(p, r, w, h, rgb, false, h*.75f, t);

	//fix this later
	Hurtbox* hurt = new Hurtbox(p, r, w, h);
	hurt->owner = ai;
	ai->hurtbox = hurt;

	hurtboxes.push_back(hurt);

	switch(ai->Type)
	{
		case AI::STANDARD:
		{
			/* load an image file directly as a new OpenGL texture */
			if(!(ai->texture[0] = LoadTexture("Assets/img_test.bmp")))
				printf("Texture not found");

			break;
		}
		case AI::JUMPER:
		{
			/* load an image file directly as a new OpenGL texture */
			if(!(ai->texture[0] = LoadTexture("Assets/Spoink.png")))
				printf("Texture not found");
				
			break;
		}
	}

	/*//put AI into the object list
	bool assigned = false;
	for(std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if(!(*it)->active)
		{
			(*it) = ai;
			assigned = true;
			(*it)->active = true;
			break;
		}
	}
	if(!assigned)
		objects.push_back(ai);*/
	
	//put AI into the doodlist
	bool assigned = false;
	for(std::list<Dood*>::iterator it = doods.begin(); it != doods.end(); ++it)
	{
		if(!(*it)->active)
		{
			(*it) = ai;
			assigned = true;
			(*it)->active = true;
			break;
		}
	}
	if(!assigned)
		doods.push_back(ai);

	AIlist.push_back(ai);
}

void destroyAI(AI& ai)
{
	delete &ai;
}

void AIUpdate(void)
{
	for(std::list<AI*>::iterator it = AIlist.begin(); it != AIlist.end(); ++it)
	{
		if(!(*it)->alive)
			continue;

		switch((*it)->Type)
		{
			case AI::STANDARD:
			{
				if(abs(P1->pos.x - (*it)->pos.x) < 3)
					continue;
				else if(P1->pos.x < (*it)->pos.x)
					(*it)->vel.x = -(*it)->walkSpeed;
				else if(P1->pos.x > (*it)->pos.x)
					(*it)->vel.x = (*it)->walkSpeed;
				break;
			}
			case AI::JUMPER:
			{
				if((*it)->gridCollision & COLLISION_BOTTOM)
				{
					(*it)->vel.y = (*it)->jumpHeight;
				}
			}
			default:
				break;
		}
	}
}