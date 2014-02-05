#include "Player.h"
#include "Collision.h"

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
    P1->texture[0] = SOIL_load_OGL_texture
        (
        "Assets/Weechan.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
 
    if(P1->texture[0] == 0)
        printf("Texture not found");

    // Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, P1->texture[0]);

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