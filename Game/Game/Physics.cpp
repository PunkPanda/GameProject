#include "Physics.h"
#include "Region.h"
#include "Player.h"

unsigned doorCollide = 0;
unsigned doorTimer = 0;

void PhysicsUpdate(void)
{
	doorCollide = 0;

	if(doorTimer)
		--doorTimer;

	for(std::list<Dood*>::iterator it = doods.begin(); it != doods.end(); ++it)
	{
		bool gravFlag = true;
		Object* i = (*it);

		if(!i->active)
			continue;

		if(!i->Static)
		{
			i->gridCollision = GridCollision(i->pos, i->width, i->height);

			if(i->gridCollision & COLLISION_RAMP_RIGHT)
			{
				RampSnapUpRight(i->pos, i->width, i->height);
				i->vel.y = 0;
				gravFlag = false;
			}
			else if(i->gridCollision & COLLISION_RAMP_LEFT)
			{
				RampSnapUpLeft(i->pos, i->width, i->height);
				i->vel.y = 0;
				gravFlag = false;
			}
			else
			{
				if(i->gridCollision & COLLISION_LEFT)
				{
					SnapUp(i->pos.x, i->width);
					i->vel.x = 0;
				}
				if(i->gridCollision & COLLISION_RIGHT)
				{
					SnapDown(i->pos.x, i->width);
					i->vel.x = 0;
				}
				if(i->gridCollision & COLLISION_TOP)
				{
					SnapDown(i->pos.y, i->height);
					i->vel.y = 0;
				}
				if(i->gridCollision & COLLISION_BOTTOM)
				{
					SnapUp(i->pos.y, i->height);
					i->vel.y = 0;
					gravFlag = false;
				}
			}
			if(gravFlag)
				ApplyGravity(i);
			else	//apply friction
			{
				if(i->vel.x > 0)
				{
					i->vel.x -= 0.00002f;
					if(i->vel.x < 0.00002f)
						i->vel.x = 0;
				}
				else if(i->vel.x < 0)
				{
					i->vel.x += 0.00002f;
					if(i->vel.x > -0.00002f)
						i->vel.x = 0;
				}
			}

			i->pos += i->vel;
		}
	}
}

void ApplyGravity(Object* obj)
{
	if(abs(obj->vel.y < 0.5))
		obj->vel.y -= 0.00001f; //replace this later
}

int GridCollision(Point pos, float width, float height)
{
	if(pos.x > regionWidth-1 || pos.y > regionHeight-1 || pos.y < 0 || pos.x < 0)
	{
		return 0;
	}
	int flag = 0;//set flag

	//set hotspots
	Point topL(pos.x - width/4, pos.y + height/2);
	Point topR(pos.x + width/4, pos.y + height/2);
	Point rightT(pos.x + width/2, pos.y + height/4);
	Point rightB(pos.x + width/2, pos.y - height/4);
	Point leftT(pos.x - width/2, pos.y + height/4);
	Point leftB(pos.x - width/2, pos.y - height/4);
	Point bottomL(pos.x - width/4, pos.y - height/2);
	Point bottomR(pos.x + width/4, pos.y - height/2);
	Point midL(pos.x - width/2, pos.y);
	Point midR(pos.x + width/2, pos.y);
	
	if(currentRegion[(int)topL.x][(int)topL.y] == '-' || currentRegion[(int)topR.x][(int)topR.y] == '-' )//if on top
	{
		flag += COLLISION_TOP;
	}	
	if(currentRegion[(int)bottomL.x][(int)bottomL.y] == '-'  || currentRegion[(int)bottomR.x][(int)bottomR.y] == '-') //if on bottom
	{
		flag += COLLISION_BOTTOM;
	}	
	if(currentRegion[(int)leftB.x][(int)leftB.y] == '-'  || currentRegion[(int)leftT.x][(int)leftT.y] == '-'  || currentRegion[(int)midL.x][(int)midL.y] == '-' )//if on left
	{
		flag += COLLISION_LEFT;
	}
	if(currentRegion[(int)rightB.x][(int)rightB.y] == '-'  || currentRegion[(int)rightT.x][(int)rightT.y] == '-'  || currentRegion[(int)midR.x][(int)midR.y] == '-' )//if on right
	{
		flag += COLLISION_RIGHT;
	}

	if(currentRegion[(int)topL.x][int(topL.y + 0.1)] == '-' || currentRegion[(int)topR.x][int(topR.y + 0.1)] == '-' )
	{
		flag += COLLISION_BUMP;
	}

	//going up or down ramps
	if(currentRegion[(int)bottomR.x][(int)bottomR.y] == '<')
	{
		flag += COLLISION_RAMP_RIGHT;
	}
	else if(currentRegion[(int)bottomL.x][(int)bottomL.y] == '>')
	{
		flag += COLLISION_RAMP_LEFT;
	}

	//check if we are colliding with any doors
	if(currentRegion[(int)pos.x][(int)pos.y] == '1')
	{
		doorCollide = 1;
	}
	else if(currentRegion[(int)pos.x][(int)pos.y] == '2')
	{
		doorCollide = 2;
	}
	else if(currentRegion[(int)pos.x][(int)pos.y] == '3')
	{
		doorCollide = 3;
	}
	else if(currentRegion[(int)pos.x][(int)pos.y] == '4')
	{
		doorCollide = 4;
	}

	return flag;
}

void SnapUp(float& Coordinate, float scale)
{
	Coordinate += (int((Coordinate - scale/2) + 1) - (Coordinate - scale/2) + .0001f);//convert to int
}

void SnapDown(float& Coordinate, float scale)
{
	Coordinate -= ((Coordinate + scale/2) - int(Coordinate + scale/2) + .0001f);//convert to int
}

void RampSnapUpRight(Point& Coordinate, float width, float height)
{
	Coordinate.y -= height/2;		//translate down to the corner
	Coordinate.x += width/4;
	Coordinate.y = (int(Coordinate.y) + ((Coordinate.x) - int(Coordinate.x)));  //move y up equal to distance into the block we are
	Coordinate.y += height/2;
	Coordinate.x -= width/4;
}
//opposite of above
void RampSnapUpLeft(Point& Coordinate, float width, float height)
{
	Coordinate.y -= height/2;
	Coordinate.x -= width/4;
	Coordinate.y = (int(Coordinate.y) + 1) - ((Coordinate.x) - int(Coordinate.x));
	Coordinate.y += height/2;
	Coordinate.x += width/4;
}
