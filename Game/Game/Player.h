#include "Weapon.h"
#include "Doods.h"

class Player : public Dood
{
	public:

		Player(Point p, Point r, float w, float h, Vector rgb, bool s, float cHeight) : Dood(p, r, w, h, rgb, s, 5), 
		crouchHeight(cHeight), standHeight(h), standWidth(w), crawlHeight(standHeight/4), crawlWidth(h), walkSpeed(0.002f), runSpeed(0.005f), currentSpeed(runSpeed), maxSpeed(runSpeed*2), 
		jumpHeight(0.01f), orientation(STANDING), bumpFlag(false)
		{
			classType = PLAYER;
		};

		float standHeight;
		float crouchHeight;
		float crawlHeight;

		float standWidth;
		float crawlWidth;

		float walkSpeed;
		float runSpeed;
		float currentSpeed;
		float maxSpeed;
		float jumpHeight;

		bool bumpFlag;
		enum Orientation
		{
			STANDING,
			CROUCHING,
			CRAWLING
		}orientation;

		Weapon* weapon;
};

void createPlayer(Point p, Point r, float w, float h, Vector rgb, bool s);

extern Player* P1;