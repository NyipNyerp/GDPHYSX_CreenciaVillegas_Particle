#include "MyVector.h"

float MyVector::magnitude()
{
	return sqrt((x * x) + (y * y) + (z*z));
}

MyVector MyVector::direction()
{

	MyVector result = MyVector(0, 0,0);
	float magnit;

	magnit = magnitude();
	result.x = x / magnit;
	result.y = y / magnit;
	result.z = z / magnit;

	return result;
}


MyVector MyVector::operator*(const float f) //A*Bx
{
	MyVector result = MyVector(0, 0,0);
	result.x = x * f;
	result.y = y * f;
	result.z = z * f;
	return result;
}

MyVector MyVector::operator+(MyVector b) //A+B
{
	MyVector result = MyVector(0, 0,0);
	result.x = x + b.x;
	result.y = y + b.y;
	result.z = z + b.z;

	return result;
}

MyVector MyVector::operator-(MyVector b) //A-B
{
	MyVector result = MyVector(0, 0,0);
	result.x = x - b.x;
	result.y = y - b.y;
	result.z = z + b.z;

	return result;
}

MyVector MyVector::operator*(MyVector b) //Compo prod
{
	MyVector result = MyVector(0, 0,0);
	result.x = x * b.x;
	result.y = y * b.y;
	result.z = z + b.z;

	return result;
}

MyVector MyVector::crossProduct(MyVector a, MyVector b)
{
	MyVector result = MyVector(0, 0,0);

	result.x = (a.y * 0) - (0 * b.y);
	result.y = (0 * b.x) - (a.x * 0);

	return result;
}

float MyVector::dotProduct(MyVector a, MyVector b)
{
	float result;

	result = (a.x * b.x) + (a.y * b.y);

	return result;
}

float MyVector::operator*=(const MyVector v)
{
	return this->x * v.x + this->y * v.y;
}

float MyVector::magnitudeSquared()
{
	return pow(magnitude(), 2);
}

void MyVector::normalize()
{
	//MyVector result = MyVector(0, 0);

	float mag = this->magnitude();

	this->x = x / mag;
	this->y = y / mag;
	this->z = z / mag;


}

MyVector::MyVector(float xIn, float yIn, float zIn)
{
	x = xIn;
	y = yIn;
	z = zIn;
}



