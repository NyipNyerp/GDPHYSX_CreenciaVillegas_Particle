#include "MyVector.h"

MyVector::MyVector(float x, float y, float z) : x(x), y(y), z(z)
{

}

//gets the direction
void MyVector::Normalize()
{
	float n = getMagnitude();
	if (n > 0) {
		(*this) *= ((1.0f) / n);
	}
}

float MyVector::getMagnitude()
{
	return sqrt(SquareMagnitude());
}

MyVector MyVector::getDirection(float magnitude)
{
	MyVector direction(this->x, this->y, 0);
	direction.x = direction.x / magnitude;
	direction.y = direction.y / magnitude;
	return direction;
}

MyVector MyVector::operator+(MyVector vec)
{
	MyVector newVector(this->x + vec.x, this->y + vec.y, 0);
	return newVector;
}

MyVector MyVector::operator-(MyVector vec)
{
	MyVector newVector(this->x - vec.x, this->y - vec.y, 0);
	return newVector;
}

MyVector MyVector::operator*(const float scalar)
{
	MyVector newVector(this->x * scalar, this->y * scalar, 0);
	return newVector;
}

float MyVector::operator*(const MyVector v)
{
	return (this->x * v.x) + (this->y * v.y);
}

void MyVector::operator+=(const MyVector vec)
{
	this->x += vec.x;
	this->y += vec.y;
}

void MyVector::operator-=(MyVector vec)
{
	this->x = x - vec.x;
	this->y = y - vec.y;
}

void MyVector::operator*=(const float scalar)
{
	this->x = x * scalar;
	this->y = y * scalar;
}

MyVector MyVector::ComponentProduct(const MyVector v)
{
	return MyVector(x * v.x, y * v.y, 0);
}

float MyVector::getScalarProduct(MyVector vec1, MyVector vec2)
{
	return (vec1.x * vec2.x) + (vec1.y * vec2.y, 0);
}

MyVector MyVector::getVectorProduct(MyVector vec1, MyVector vec2)
{
	MyVector newVector((vec1.y * vec2.z) - (vec1.z * vec2.y),
		(vec1.z * vec2.x) - (vec1.x * vec2.z),
		(vec1.x * vec2.y) - (vec1.y * vec2.x));
	return newVector;
}


float MyVector::SquareMagnitude()
{
	return (x * x) + (y * y);
}

void MyVector::Invert()
{
	x = -x;
	y = -y;
}