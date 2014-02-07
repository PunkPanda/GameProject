#include "Player.h"
#include "Combat.h"

Player* P1;

void createPlayer(Point p, Point r, float w, float h, Vector rgb, bool s)
{
	P1 = new Player(p, r, w, h, rgb, s, h*.75f);
	P1->weapon = NULL;

	//fix this later
	Hurtbox* hurt = new Hurtbox(p, r, w, h);
	hurt->owner = P1;
	P1->hurtbox = hurt;

	hurtboxes.push_back(hurt);

    /* load an image file directly as a new OpenGL texture */
	if(!(P1->texture[0] = LoadTexture("Assets/Weechan.jpg")))
		printf("Texture not found");

	//put Player into the doodlist
	bool assigned = false;
	for(std::list<Dood*>::iterator it = doods.begin(); it != doods.end(); ++it)
	{
		if(!(*it)->active)
		{
			(*it) = P1;
			assigned = true;
			(*it)->active = true;
			break;
		}
	}
	if(!assigned)
		doods.push_back(P1);
}

void destroyPlayer(Player& p)
{
	delete &p;
}