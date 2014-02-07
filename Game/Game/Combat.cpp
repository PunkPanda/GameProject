#include "Combat.h"
#include "Doods.h"

std::vector<Hitbox*> hitboxes;
std::vector<Hurtbox*> hurtboxes;

void CollisionUpdate(void)
{
	for(std::vector<Hurtbox*>::iterator hurtit = hurtboxes.begin(); hurtit != hurtboxes.end(); ++hurtit) //loop through all hurtboxes
	{
		for(std::vector<Hitbox*>::iterator hitit = hitboxes.begin(); hitit != hitboxes.end(); ++hitit)	//loop through all hitboxes
		{
			if(CheckCollision(*hurtit, *hitit))		//check all hurtboxes against all hitboxes
			{
				(*hurtit)->owner->damage = (*hitit)->power;
			}
		}
	}
}

short CheckCollision(Hurtbox* Frankie, Hitbox* Janice)
{
	float Fleft = Frankie->pos.x - Frankie->width/2;
	float Fright = Frankie->pos.x + Frankie->width/2;
	float Ftop = Frankie->pos.y + Frankie->height/2;
	float Fbottom = Frankie->pos.y - Frankie->height/2;

	float Jleft = Janice->pos.x - Janice->width/2;
	float Jright = Janice->pos.x + Janice->width/2;
	float Jtop = Janice->pos.y + Janice->height/2;
	float Jbottom = Janice->pos.y - Janice->height/2;

	if((Fright > Jleft && Fright < Jright) ||
	    (Fleft > Jleft && Fleft < Jright))		//Collision viable horizontally
	{
		if((Ftop > Jbottom && Ftop < Jtop) ||
		   (Fbottom > Jbottom && Fbottom < Jtop) ||
		    Ftop > Jtop && Fbottom < Jbottom)		//Collision viable vertically
		{
			return 1;
		}
	}
	else if(Fleft < Jleft && Fright > Jright &&
		    Ftop > Jtop && Fbottom < Jbottom)
	{
		return 1;
	}

	return 0;
}