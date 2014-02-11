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
		//check if slot in doodlist is active or not
		if(!(*it)->active)
			continue;
		//skip physics if object is static
		if((*it)->Static)
			continue;

		ApplyGravity(*it);

		//apply velocity
		(*it)->pos += (*it)->vel;

		//returns a bool indicating whether or not to apply friction for floor contact
		if(MapCollision(*it))
		{
			ApplyFriction(*it);
		}
	}
}

bool MapCollision(Object* obj)
{
	obj->gridCollision = GridCollision(obj->pos, obj->width, obj->height);

	if(obj->gridCollision & COLLISION_RAMP_RIGHT)
	{
		RampSnapUpRight(obj->pos, obj->width, obj->height);
		obj->vel.y = 0;
		return true;
	}
	else if(obj->gridCollision & COLLISION_RAMP_LEFT)
	{
		RampSnapUpLeft(obj->pos, obj->width, obj->height);
		obj->vel.y = 0;
		return true;
	}
	else
	{
		if(obj->gridCollision & COLLISION_LEFT)
		{
			SnapUp(obj->pos.x);
			obj->vel.x = 0;
		}
		if(obj->gridCollision & COLLISION_RIGHT)
		{
			SnapDown(obj->pos.x);
			obj->vel.x = 0;
		}
		if(obj->gridCollision & COLLISION_TOP)
		{
			SnapDown(obj->pos.y);
			obj->vel.y = 0;
		}
		if(obj->gridCollision & COLLISION_BOTTOM)
		{
			SnapUp(obj->pos.y);
			obj->vel.y = 0;
			return true;
		}
	}
	return false;
}

void ApplyGravity(Object* obj)
{
	if(obj->vel.y > - 1.0f)
		obj->vel.y -= 0.025f; //replace this later
}

void ApplyFriction(Object* obj)
{
	if(obj->vel.x > 0)
	{
		obj->vel.x -= 0.1f;
		if(obj->vel.x < 0.1f)
			obj->vel.x = 0;
	}
	else if(obj->vel.x < 0)
	{
		obj->vel.x += 0.1f;
		if(obj->vel.x > -0.1f)
			obj->vel.x = 0;
	}
}

int GridCollision(Point pos, float width, float height)
{
	if(pos.x > regionWidth-1 || pos.y > regionHeight-1 || pos.y < 0 || pos.x < 0)
	{
		return 0;
	}
	int flag = 0;//set flag

	//set hotspots
	Point topL(pos.x + width/5, pos.y + height);
	Point topR(pos.x + 4*width/5, pos.y + height);
	Point rightT(pos.x + width, pos.y + 4*height/5);
	Point rightB(pos.x + width, pos.y + height/5);
	Point leftT(pos.x, pos.y + 4*height/5);
	Point leftB(pos.x, pos.y + height/5);
	Point bottomL(pos.x + width/5, pos.y);
	Point bottomR(pos.x + 4*width/5, pos.y);
	Point midL(pos.x, pos.y + height/2);
	Point midR(pos.x + width, pos.y + height/2);
	Point rampR(pos.x + width, pos.y);
	Point rampL(pos.x, pos.y);
	
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
	if(currentRegion[(int)rampR.x][(int)rampR.y] == '<')
	{
		flag += COLLISION_RAMP_RIGHT;
	}
	else if(currentRegion[(int)rampL.x][(int)rampL.y] == '>')
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

void SnapUp(float& Coordinate)
{
	Coordinate = int(Coordinate + 1);
}

void SnapDown(float& Coordinate)
{
	Coordinate = (int)Coordinate;//convert to int
}

void RampSnapUpRight(Point& Coordinate, float width, float height)
{
	Coordinate.y = int(Coordinate.y + 1);
	Coordinate.y +=  int(Coordinate.x + 1) - Coordinate.x;  //move y up equal to distance into the block we are
}
//opposite of above
void RampSnapUpLeft(Point& Coordinate, float width, float height)
{
	Coordinate.y = int(Coordinate.y + 1) - (Coordinate.x - int(Coordinate.x));
}
