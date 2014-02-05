#include "Vector.h"
#include <math.h>
#include <stdio.h>

// Default constructor, initializes x,y,z to zeroes, w to defined value
Vector::Vector(void) : x(0), y(0), z(0), w(0)
{
}
  
// Copy constructor, copies every component from the other Vector
Vector::Vector(const Vector& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
}
  
// Non-Default constructor, self explanatory
Vector::Vector(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww)
{
}
  
// Assignment operator, does not need to handle self assignment
Vector& Vector::operator=(const Vector& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;

	return *this;
}
  
// Unary negation operator, negates all components and returns a copy
Vector Vector::operator-(void) const
{
	Vector temp;

	temp.x = -x;
	temp.y = -y;
	temp.z = -z;
	temp.w = -w;

	return temp;
}
    
// Basic Vector math operations. Add Vector to Vector B, or Subtract Vector A 
// from Vector B, or multiply a vector with a scalar, or divide a vector by 
// that scalar
Vector Vector::operator+(const Vector& rhs) const
{
	Vector temp;

	temp.x = x + rhs.x;
	temp.y = y + rhs.y;
	temp.z = z + rhs.z;
	temp.w = w + rhs.w;

	return temp;
}

Vector Vector::operator-(const Vector& rhs) const
{
	Vector temp;

	temp.x = x - rhs.x;
	temp.y = y - rhs.y;
	temp.z = z - rhs.z;
	temp.w = w - rhs.w;

	return temp;
}

Vector Vector::operator*(const float rhs) const
{
	Vector temp;

	temp.x = x * rhs;
	temp.y = y * rhs;
	temp.z = z * rhs;
	temp.w = w * rhs;

	return temp;
}

Vector Vector::operator/(const float rhs) const
{
	Vector temp;

	temp.x = x / rhs;
	temp.y = y / rhs;
	temp.z = z / rhs;
	temp.w = w / rhs;

	return temp;
}
  
// Same as above, just stores the result in the original vector
Vector& Vector::operator+=(const Vector& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;

	return *this;
}

Vector& Vector::operator-=(const Vector& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;

	return *this;
}

Vector& Vector::operator*=(const float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;

	return *this;
}

Vector& Vector::operator/=(const float rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	w /= rhs;

	return *this;
}
  
// Computes the dot product with the other vector. Treat it as 3D vector.
float Vector::Dot(const Vector& rhs) const
{
	return  x * rhs.x +
			y * rhs.y +
			z * rhs.z;
}
  
// Computes the cross product with the other vector. Treat it as a 3D vector.
Vector Vector::Cross(const Vector& rhs) const
{
	Vector temp;

	temp.x = y * rhs.z - z * rhs.y;
	temp.y = z * rhs.x - x * rhs.z;
	temp.z = x * rhs.y - y * rhs.x;

	return temp;
}
  
// Computes the true length of the vector 
float Vector::Length(void) const
{
	return sqrt(x*x + y*y + z*z + w*w);
}
  
// Computes the squared length of the vector
float Vector::LengthSq(void) const
{
	return x*x + y*y + z*z + w*w;
}
  
// Normalizes the vector to make the final vector be of length 1. If the length is zero
// then this function should not modify anything.
void Vector::Normalize(void)
{
	float Magnitude = Length();
	if(!Magnitude)
		return;

	x /= Magnitude;
	y /= Magnitude;
	z /= Magnitude;
	w /= Magnitude;
}
  
// Sets x,y,z to zeroes, w to defined value
void Vector::Zero(void)
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

void Vector::Print(void) const
{
  printf("%5.3f, %5.3f, %5.3f, %5.3f\n",x,y,z,w);
}
