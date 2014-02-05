struct Vector
{
  union
  {
    struct 
    {
      float x, y, z, w;
    };
  
    struct  
    {
      float r, g, b, a;
    };
  
    float v[4];
  };
  
    // Default constructor, initializes x,y,z to zeroes, w to defined value
  Vector(void);
  
    // Copy constructor, copies every component from the other Vector
  Vector(const Vector& rhs);
  
    // Non-Default constructor, self explanatory
  Vector(float xx, float yy, float zz, float ww = 0.0f);
  
    // Assignment operator, does not need to handle self assignment
  Vector& operator=(const Vector& rhs);
  
    // Unary negation operator, negates all components and returns a copy
  Vector operator-(void) const;  
    
    // Basic Vector math operations. Add Vector to Vector B, or Subtract Vector A 
    // from Vector B, or multiply a vector with a scalar, or divide a vector by 
    // that scalar
  Vector operator+(const Vector& rhs) const;
  Vector operator-(const Vector& rhs) const;
  Vector operator*(const float rhs) const;
  Vector operator/(const float rhs) const;
  
    // Same as above, just stores the result in the original vector
  Vector& operator+=(const Vector& rhs);
  Vector& operator-=(const Vector& rhs);
  Vector& operator*=(const float rhs);
  Vector& operator/=(const float rhs);
  
    // Computes the dot product with the other vector. Treat it as 3D vector.
  float Dot(const Vector& rhs) const;
  
    // Computes the cross product with the other vector. Treat it as a 3D vector.
  Vector Cross(const Vector& rhs) const;
  
    // Computes the true length of the vector 
  float Length(void) const;
  
    // Computes the squared length of the vector
  float LengthSq(void) const;
  
    // Normalizes the vector to make the final vector be of length 1. If the length is zero
    // then this function should not modify anything.
  void Normalize(void);
  
    // Sets x,y,z to zeroes, w to defined value
  void Zero(void);
  
    // Already implemented, simple print function
  void Print(void) const;
};
