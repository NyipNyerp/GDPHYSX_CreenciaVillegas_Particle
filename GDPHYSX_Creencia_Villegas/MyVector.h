#ifndef MYVECTOR
#define MYVECTOR
#include <iostream>
#include <math.h>
using namespace std;

class MyVector
{
public:
	float x;
	float y;
	float z;

public:
	float magnitude();
	MyVector direction();
	MyVector operator*(const float f); //A*Bx
	MyVector operator+(MyVector b); //A+B
	MyVector operator-(MyVector b); //A-B
	MyVector operator*(MyVector b); //Component Product of A and B
	MyVector crossProduct(MyVector a, MyVector b);
	float dotProduct(MyVector a, MyVector b);
	float operator*=(const MyVector v); //dot prod alt
	float magnitudeSquared();
	float getDistanceBetweenPoints(MyVector a, MyVector b);
	static float crossProductF(MyVector a, MyVector b);

	void normalize();

	MyVector(float newX, float newY, float newZ);
};

#endif