struct Vector;

struct Point
{
	struct 
	{
		float x, y, z;
	};

	// Default constructor, sets x,y,z to zero and w to the defined value
	Point(void);
  
	// Copy constructor, copies every component from the other Point
	Point(const Point& rhs);
  
	// Non-Default constructor, self-explanatory
	Point(float xx, float yy, float zz, float ww = 1.0f);
  
	//2d Non-default constructor
	Point::Point(float xx, float yy) : x(xx), y(yy), z(0)
	{
	};

	// Assignment operator, copies every component from the other Point
	Point& operator=(const Point& rhs);
  
	// Unary negation operator, negates every component and returns a copy
	Point operator-(void) const;
  
	bool operator==(Point rhs);

	bool operator!=(Point rhs);

	// Binary subtraction operator, Subtract two Points and you get a Vector
	Vector operator-(const Point&  rhs) const;
  
	// Basic vector math operations with points, you can add a Vector to a Point, or subtract
	// a Vector from a Point
	Point  operator+ (const Vector& rhs) const;
	Point  operator- (const Vector& rhs) const;
  
	// Same as previous two operators, just modifies the original instead of returning a copy
	Point& operator+=(const Vector& rhs);
	Point& operator-=(const Vector& rhs);
  
	// Sets x,y,z to zeroes, w to defined value
	void Zero(void);
  
	// Already implemented, simple print function
	void Print(void) const;
};
