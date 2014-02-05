#include "Doods.h"

class Hitbox : public Object
{
	public:
		Hitbox(Point p, Point r, float w, float h, unsigned pow, unsigned life) : Object(p, r, w, h, Vector(0, 0, 0), true, HITBOX), power(pow), lifetime(life){};

		Dood* owner;
		unsigned power;
		unsigned lifetime;
};

class Hurtbox : public Object
{
	public:
		Hurtbox(Point p, Point r, float w, float h) : Object(p, r, w, h, Vector(0, 0, 0), true, HURTBOX){};

		Dood* owner;
};

extern std::vector<Hitbox*> hitboxes;
extern std::vector<Hurtbox*> hurtboxes;

void CollisionUpdate(void);

short CheckCollision(Hurtbox* Frankie, Hitbox* Janice);

