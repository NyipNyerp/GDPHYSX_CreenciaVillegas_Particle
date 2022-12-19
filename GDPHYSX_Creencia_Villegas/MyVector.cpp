#include "MyVector.h"

float MyVector::magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

MyVector MyVector::direction()
{
	MyVector result = MyVector(0, 0, 0);
	float mag;

	mag = magnitude();
	result.x = x / mag;
	result.y = y / mag;
	result.z = z / mag;

	return result;
}


MyVector MyVector::operator*(const float f) //A*Bx
{
	MyVector result = MyVector(0, 0, 0);
	result.x = x * f;
	result.y = y * f;
	result.z = z * f;
	return result;
}

MyVector MyVector::operator+(MyVector b) //A+B
{
	MyVector result = MyVector(0, 0, 0);
	result.x = x + b.x;
	result.y = y + b.y;
	result.z = z + b.z;
	return result;
}

MyVector MyVector::operator-(MyVector b) //A-B
{
	MyVector result = MyVector(0, 0, 0);
	result.x = x - b.x;
	result.y = y - b.y;
	result.z = z - b.z;
	return result;
}

MyVector MyVector::operator*(MyVector b) //Compo prod
{
	MyVector result = MyVector(0, 0, 0);
	result.x = x * b.x;
	result.y = y * b.y;
	result.z = z * b.z;
	return result;
}

MyVector MyVector::crossProduct(MyVector a, MyVector b)
{
	MyVector result = MyVector(0, 0, 0);

	result.x = (a.y * 0) - (0 * b.y);
	result.y = (0 * b.x) - (a.x * 0);

	return result;
}

float MyVector::dotProduct(MyVector a, MyVector b)
{
	float result;

	result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);

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

float MyVector::getDistanceBetweenPoints(MyVector a, MyVector b)
{
	return sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)) + ((b.z - a.z) * (b.z - a.z)));
}

float MyVector::crossProductF(MyVector a, MyVector b)
{
	return (a.x * b.y) - (a.y * b.x);
}

void MyVector::normalize()
{
	float mag = this->magnitude();

	this->x = x / mag;
	this->y = y / mag;
	this->z = z / mag;
}

MyVector::MyVector(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}