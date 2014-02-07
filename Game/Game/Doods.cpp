#include "Doods.h"
#include "Combat.h"

std::list<Dood*> doods;

void DoodsUpdate(void)
{
	for(std::list<Dood*>::iterator it = doods.begin(); it != doods.end(); ++it)
	{
		if((*it)->hitbox != NULL)
		{
			--(*it)->hitbox->lifetime;
			(*it)->hitbox->pos = (*it)->pos;
			(*it)->hitbox->pos.x += 1.0f;

			//all of this is very bad, only to see if i can make this work, this needs to be changed, BADLY
			if((*it)->hitbox->lifetime <= 0)
			{
				delete (*it)->hitbox;
				(*it)->hitbox = NULL;
				hitboxes.clear();
			}
		}
		//fix this later, manually updating hurtbox positions, currently identical to object collison, probably should work on that
		(*it)->hurtbox->width = (*it)->width;
		(*it)->hurtbox->height = (*it)->height;
		(*it)->hurtbox->pos = (*it)->pos;

		(*it)->hp -= (*it)->damage;
		(*it)->damage = 0;

		if((*it)->hp <= 0)
		{
			(*it)->alive = false;
		}
	}
}

void Dood::Attack(void)
{
	hitbox = new Hitbox(Point(pos.x + 1.0f, pos.y, pos.z), rot, width/2, height/4, 1, 500);
	hitboxes.push_back(hitbox);
}