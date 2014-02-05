#include <Windows.h>

#include "Graphics.h"
#include "Player.h"

#include <stdio.h>
#include <iostream>
#include "Physics.h"
#include "Region.h"
#include "Input.h"

bool keys[256];		
bool keyTriggered[256];
bool keyReleased[256];

bool fullscreenswitch = false;
bool escapeswitch = false;

void InputUpdate(void)
{
	if(keys[VK_UP])
	{
		xrot+=0.01f;
	}
	if(keys[VK_DOWN])
	{
		xrot-=0.01f;
	}
	if(keys[VK_LEFT])
	{
		yrot-=0.01f;                             // X Axis Rotation
	}
	if(keys[VK_RIGHT])
	{
		yrot+=0.01f;                             // X Axis Rotation
	}
	if(keyTriggered['W'])
	{
		switch(P1->orientation)
		{
			case P1->STANDING:
			{
				if(!doorTimer)
				{
					if (doorCollide > 0)
					{
						FreeCurrentRegion();
						LoadRegion(adjRegions[doorCollide - 1].c_str());
					}

					/*switch(doorCollide)
					{
						case 0:
							break;
						case 1:
							FreeCurrentRegion();
							LoadRegion(nextRegion1.c_str());
							break;
						case 2:
							FreeCurrentRegion();
							LoadRegion(nextRegion2.c_str());
							break;
						case 3:
							FreeCurrentRegion();
							LoadRegion(nextRegion3.c_str());
							break;
						case 4:
							FreeCurrentRegion();
							LoadRegion(nextRegion4.c_str());
							break;
						default:
							break;
					}*/
					doorTimer += 1000;
				}
				break;
			}
			case P1->CROUCHING:
			{
				if(!(P1->gridCollision & COLLISION_BUMP))
				{
					P1->height = P1->standHeight;
					P1->pos.y += (P1->standHeight - P1->crouchHeight)/2;
					P1->orientation = P1->STANDING;
				}
				break;
			}
			case P1->CRAWLING:
			{
				if(!(P1->gridCollision & COLLISION_BUMP))
				{
					P1->height = P1->crouchHeight;
					P1->width = P1->standWidth;
					P1->pos.y += (P1->crouchHeight - P1->crawlHeight)/2;
					P1->orientation = P1->CROUCHING;
				}
				break;
			}
			default:
				break;
		}
	}
	if(keyTriggered['S'])
	{
		if(P1->orientation == P1->STANDING)
		{
			P1->height = P1->crouchHeight;
			P1->pos.y -= (P1->standHeight - P1->crouchHeight)/2;
			P1->orientation = P1->CROUCHING;
		}
		else if(P1->orientation == P1->CROUCHING)
		{
			P1->height = P1->crawlHeight;
			P1->width = P1->crawlWidth;
			P1->pos.y -= (P1->crouchHeight - P1->crawlHeight)/2;
			P1->orientation = P1->CRAWLING;
		}
	}
	if(keys['A'])
	{
		P1->vel.x = -P1->walkSpeed;
	}
	if(keys['D'])
	{
		P1->vel.x = P1->walkSpeed;
	}
	if(keys[VK_SHIFT] && P1->orientation == P1->STANDING)
	{
		if(keys['A'])
		{
			P1->vel.x = -P1->runSpeed;
		}
		if(keys['D'])
		{
			P1->vel.x = P1->runSpeed;
		}
	}
	if(keys[VK_SPACE])
	{
		if(P1->orientation != P1->CRAWLING)
		{
			if(P1->gridCollision & COLLISION_BOTTOM)
			{
				P1->vel.y = P1->jumpHeight;
			}
			else if(P1->gridCollision & COLLISION_RAMP_LEFT)
			{
				P1->pos.y = int(P1->pos.y) + 1;
				P1->vel.y = P1->jumpHeight;
			}
			else if(P1->gridCollision & COLLISION_RAMP_RIGHT)
			{
				P1->pos.y = int(P1->pos.y) + 1;
				P1->vel.y = P1->jumpHeight;
			}
		}
	}
	if(keyTriggered['F'])
	{
		P1->Attack();
	}
	if(keys[VK_NEXT])
	{
		z+=0.01f;
	}
	if(keys[VK_PRIOR])
	{
		z-=0.01f;
	}
	if(keys[VK_ESCAPE])                // Was ESC Pressed?
		escapeswitch = true;
	if(keys[VK_F1])                    // Is F1 Being Pressed?
		fullscreenswitch = true;

	//clear the keyArrays
	memset(keyTriggered, 0, 256);
	memset(keyReleased, 0, 256);
}