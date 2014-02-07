#include <string>
#include <vector>
#include "Object.h"

#define BUFF 50

void LoadRegion(const char* regionName);
void FreeCurrentRegion(void);

extern char** currentRegion;		//2d map of the current region

extern std::string nextRegion1;	//character arrays for names of connected regions
extern std::string nextRegion2;
extern std::string nextRegion3;
extern std::string nextRegion4;

extern std::vector<std::string> adjRegions;

extern std::string backgroundImage;		//strings for holding names of region based textures
extern std::string tileImage;
extern std::string doorImage;

extern GLuint basetileTex;			//region based texture objects
extern GLuint doorTex;
extern GLuint backgroundTex;

extern short regionWidth;
extern short regionHeight;

class Door: public Object
{
	public:
		Door(Point p, Point r, float w, float h, Vector rgb, bool s, std::string region) : Object(p, r, w, h, rgb, s, DOOR), nextRegion(region)
		{
		};

		std::string nextRegion;
};