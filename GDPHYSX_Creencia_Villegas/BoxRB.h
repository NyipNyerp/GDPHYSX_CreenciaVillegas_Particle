#pragma once
#include "Utils.h"
#include "MyParticle.h"
#include <vector>

class BoxRB : public MyParticle
{
public:
	BoxRB(int newType);
	void addForceAtPoint(MyVector point, MyVector f);
	float length = 5;
	std::vector<MyVector> points;

	MyVector upperLeft1 = MyVector(0, 0, 0);
	MyVector lowerLeft1 = MyVector(0, 0, 0);
	MyVector upperRight1 = MyVector(0, 0, 0);
	MyVector lowerRight1 = MyVector(0, 0, 0);

	MyVector upperLeft2 = MyVector(0, 0, 0);
	MyVector lowerLeft2 = MyVector(0, 0, 0);
	MyVector upperRight2 = MyVector(0, 0, 0);
	MyVector lowerRight2 = MyVector(0, 0, 0);

	void updateBoxPos();
};