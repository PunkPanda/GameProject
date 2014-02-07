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

		AI(Point p, Point r, float w, float h, Vector rgb, bool s, float cHeight, enum AI::Type t) : Dood(p, r, w, h, rgb, s, 3), Type(t)
		{
			classType = A_I;
		};
};

void createAI(Point p, Point r, float w, float h, Vector rgb, enum AI::Type t);
void destroyAI(AI& ai);

void AIUpdate(void);

extern std::list<AI*> AIlist;