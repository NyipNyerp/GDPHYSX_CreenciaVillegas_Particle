#include "Utils.h"

MyVector Utils::rotatePoint(MyVector vec, float rot)
{
	MyVector ret = MyVector(0, 0, 0);
	float rad = rot;

	ret.x = vec.x * cos(rad) + vec.y * sin(rad);
	ret.y = -vec.x * sin(rad) + vec.y * cos(rad);

	return ret;
}