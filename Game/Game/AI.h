#include "Doods.h"

class AI : public Dood
{	
	public:
		enum Type
		{
			STANDARD,
			JUMPER
		}Type;

		enum Orientation
		{
			STANDING,
			CROUCHING,
			CRAWLING
		}Orientation;

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

		AI(Point p, Point r, float w, float h, Vector rgb, bool s, float cHeight, enum AI::Type t) : Dood(p, r, w, h, rgb, s, 3), 
		crouchHeight(cHeight), standHeight(h), standWidth(w), crawlHeight(standHeight/4), crawlWidth(h), walkSpeed(0.002f), runSpeed(0.005f), currentSpeed(runSpeed), maxSpeed(runSpeed*2), 
		jumpHeight(0.01f), Orientation(STANDING), bumpFlag(false), Type(t)
		{
			classType = A_I;
		};
};

void createAI(Point p, Point r, float w, float h, Vector rgb, enum AI::Type t);
void destroyAI(AI& ai);

void AIUpdate(void);

extern std::list<AI*> AIlist;