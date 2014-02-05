#include "Point.h"
#include "Vector.h"
#include <stdio.h>

// Default constructor, sets x,y,z to zero
Point::Point(void) : x(0), y(0), z(0)
{
}

// Copy constructor, copies every component from the other Point
Point::Point(const Point& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{
}
  
// Non-Default constructor, self-explanatory
Point::Point(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz)
{
}
 
// Assignment operator, copies every component from the other Point
Point& Point::operator=(const Point& rhs)
{
	x = rhs.x;	y = rhs.y;	z = rhs.z;
	return *this;
}
  
// Unary negation operator, negates every component and returns a copy
Point Point::operator-(void) const
{
	Point temp;

	temp.x = -x; temp.y = -y; temp.z = -z;

	return temp;
}
  
bool Point::operator==(Point rhs)
{
	if(x == rhs.x && y == rhs.y && z == rhs.z)
		return true;

	return false;
}

bool Point::operator!=(Point rhs)
{
	if(*this == rhs)
		return false;

	return true;
}
// Binary subtraction operator, Subtract two Points and you get a Vector
Vector Point::operator-(const Point&  rhs) const
{
	Vector temp;

	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;

	return temp;
}
  
// Basic vector math operations with points, you can add a Vector to a Point, or subtract
// a Vector from a Point
Point  Point::operator+ (const Vector& rhs) const
{
	Point temp;

	temp.x = x + rhs.x; temp.y = y + rhs.y; temp.z = z + rhs.z;

	return temp;
}

Point  Point::operator- (const Vector& rhs) const
{
	Point temp;

	temp.x = x - rhs.x; temp.y = y - rhs.y; temp.z = z - rhs.z;

	return temp;
}
  
// Same as previous two operators, just modifies the original instead of returning a copy
Point& Point::operator+=(const Vector& rhs)
{
	x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

Point& Point::operator-=(const Vector& rhs)
{
	x -= rhs.x; y -= rhs.y; z -= rhs.z;
	return *this;
}
  
// Sets x,y,z to zeroes
void Point::Zero(void)
{
	x = 0; y = 0; z = 0;
}

void Point::Print(void) const
{
  printf("%5.3f, %5.3f, %5.3f\n",x,y,z);
}
