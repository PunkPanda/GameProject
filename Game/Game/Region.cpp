#include "Region.h"
#include <stdio.h>
#include "Player.h"
#include "AI.h"
#include <fstream>
#include "Graphics.h"
#include "Collision.h"

char** currentRegion;

std::string nextRegion1;
std::string nextRegion2;
std::string nextRegion3;
std::string nextRegion4;

std::vector<std::string> adjRegions;

std::string backgroundImage;
std::string tileImage;
std::string doorImage;

short regionWidth;
short regionHeight;

std::string lastRegion;

void createDoor(Point p, Point r, float w, float h, Vector rgb, std::string region)
{
	Door *door = new Door(p, r, w, h, rgb, true, region);

	objects.push_back(door);

	/* load an image file directly as a new OpenGL texture */
	door->texture[0] = SOIL_load_OGL_texture
		(
		"Assets/Door.bmp",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);
 
	if(door->texture[0] == 0)
		printf("Texture not found");

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, door->texture[0]);
}

void LoadRegion(const char* regionName)
{
	//buffer is screwy, need this temp for now
	std::string temper = regionName;

	//open the file
	std::ifstream fp;
	fp.open(regionName, std::ifstream::in);

	//check if we successfully opened it
	if(!fp.is_open())
	{
		printf("File not found");
		return;
	}

	std::string temp;

	//read in the first line for width and height
	std::getline(fp, temp);
	regionWidth = atoi(temp.c_str());

	std::getline(fp, temp);
	regionHeight = atoi(temp.c_str());

	//read in name of textures
	std::getline(fp, backgroundImage);
	std::getline(fp, tileImage);
	std::getline(fp, doorImage);

	adjRegions.clear();

		//read in names of connected regions
	while(1)
	{
		std::getline(fp, temp);

		if (temp[0] == '@')
		{
			break;
		}
		else
		{
			adjRegions.push_back(temp);
		}
	}
	/*std::getline(fp, nextRegion1);
	std::getline(fp, nextRegion2);
	std::getline(fp, nextRegion3);
	std::getline(fp, nextRegion4);*/

	//assign memory for the array
	currentRegion = new char*[regionWidth];
	for(short i = 0; i < regionWidth; ++i)
	{
		currentRegion[i] = new char[regionHeight];
	}

	//read in a line for each row of binary map
	for(short i = regionHeight-1; i >= 0; --i)
	{
		getline(fp, temp);				//for each element in the row
		short region = 0;
		for(short j = 0; j < regionWidth; ++j)
		{
			char tile = temp[j];					//isolate the value
			currentRegion[j][i] = tile;
			if(tile == 'e')
			{
				createAI(Point(j,i, 0.03f), Point(0,0), 1.0f, 1.0f, Vector(1,1,1), AI::STANDARD);
			}
			else if(tile == 'j')
			{
				createAI(Point(j,i, 0.03f), Point(0,0), 1.0f, 1.0f, Vector(1,1,1), AI::JUMPER);
			}
			else if(tile == '#')
			{
				createDoor(Point(j, i, 0.03f), Point(0,0), 3.0f, 4.0f, Vector(1,1,1), adjRegions[region++]);
			}
		}
	}

	//really hope this gets replaced
	if(lastRegion == adjRegions[0])
	{
		for(short i = 0; i < regionWidth; ++i)
			for(short j = regionHeight-1; j >= 0; --j)
				if(currentRegion[i][j] == '1')
				{
					P1->pos.x = i+1;
					P1->pos.y = j;
					j = -1;
					i = regionWidth;
				}
	}
	else if(lastRegion == adjRegions[1])
	{
		for(short i = 0; i <regionWidth; ++i)
			for(short j = regionHeight-1; j >= 0; --j)
				if(currentRegion[i][j] == '2')
				{
					P1->pos.x = i+1;
					P1->pos.y = j;
					j = -1;
					i = regionWidth;
				}
	}
	else if(lastRegion == adjRegions[2])
	{
		for(short i = 0; i <regionWidth; ++i)
			for(short j = regionHeight-1; j >= 0; --j)
				if(currentRegion[i][j] == '3')
				{
					P1->pos.x = i+1;
					P1->pos.y = j;
					j = -1;
					i = regionWidth;
				}
	}
	else if(lastRegion == adjRegions[3])
	{
		for(short i = 0; i <regionWidth; ++i)
			for(short j = regionHeight-1; j >= 0; --j)
				if(currentRegion[i][j] == '4')
				{
					P1->pos.x = i+1;
					P1->pos.y = j;
					j = -1;
					i = regionWidth;
				}
	}

	fp.close();
	lastRegion = temper;
	if(!LoadGLTextures())                          // Jump To Texture Loading Routine ( NEW )
    {
        printf("Textures failed to load");
    }
}

void FreeCurrentRegion(void)
{
	//free all relevant objects from the current region
	for(std::list<Dood*>::iterator it = doods.begin(); it != doods.end(); ++it)
	{
		if((*it)->classType == Object::A_I)
		{
			(*it)->active = false;
		}
	}
	AIlist.clear();
	hitboxes.clear();
	for(std::vector<Hurtbox*>::iterator it = hurtboxes.begin(); it != hurtboxes.end();)
	{
		if((*it)->owner->classType != Object::PLAYER)
		{
			it = hurtboxes.erase(it);
		}
		else
			++it;
	}

	//free the binary map
	for(short i = 0; i < regionWidth; ++i)
		delete[] currentRegion[i];

	delete[] currentRegion;
}