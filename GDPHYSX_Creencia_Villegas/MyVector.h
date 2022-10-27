#ifndef MYVECTOR
#define MYVECTOR
#include <iostream>
#include <math.h>
using namespace std;

class MyVector
{
public:  //vector's components
	float x;
	float y;
	float z;

public:
	MyVector(float x = 0, float y = 0, float z = 0); //only called once, when an object is created
	float getMagnitude();
	MyVector getDirection(float magnitude);

	//HW03
	MyVector operator+(MyVector vec); //this is for vector addition
	MyVector operator-(MyVector vec); //this is for vector subtraction
	MyVector operator*(const float scalar); //this is for scalar multiple
	void operator+=(MyVector vec); //this is for vector addition
	void operator-=(MyVector vec); //this is for vector subtraction
	void operator*=(const float scalar); //this is for scalar multiple
	float operator*(const MyVector v);
	MyVector ComponentProduct(const MyVector v); //this is for ComponentProduct
	float getScalarProduct(MyVector vec1, MyVector vec2); //this is for ScalarProduct
	MyVector getVectorProduct(MyVector vec1, MyVector vec2); //this is for VectorProduct
	void Normalize(); //this is for VectorProduct
	float SquareMagnitude();
	void Invert();

};


#endif //MYVECTOR

