#include "Weapon.h"
#include "Doods.h"

class Player : public Dood
{
	public:
		enum Orientation
		{
			STANDING,
			CROUCHING,
			CRAWLING
		}orientation;

		Player(Point p, Point r, float w, float h, Vector rgb, bool s, float cHeight) : Dood(p, r, w, h, rgb, s, 5), orientation(STANDING)
		{
			classType = PLAYER;
		};

		Weapon* weapon;
};

void createPlayer(Point p, Point r, float w, float h, Vector rgb, bool s);

extern Player* P1;