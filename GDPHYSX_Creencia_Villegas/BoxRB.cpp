#include "BoxRB.h"


BoxRB::BoxRB()
{
	this->type = 0;
	this->name = "BOX";
	this->velocity = MyVector(0.0f, 0.0f, 0.0f);
	this->acceleration = MyVector(0.0f, 0.0f, 0.0f);
	this->mass = 5.0f;
	this->damping = 0.99f;
}

void BoxRB::addForceAtPoint(MyVector point, MyVector f)
{
	accumulatedForce = accumulatedForce + f;
	accumulatedTorque = accumulatedTorque + MyVector::crossProductF(point, f);
}

void BoxRB::updateBoxPos()
{
	points.at(0) = (Utils::rotatePoint(upperLeft1, rotation) + position);
	points.at(1) = (Utils::rotatePoint(lowerLeft1, rotation) + position);
	points.at(2) = (Utils::rotatePoint(upperRight1, rotation) + position);
	points.at(3) = (Utils::rotatePoint(lowerRight1, rotation) + position);

	points.at(4) = (Utils::rotatePoint(upperLeft2, rotation) + position);
	points.at(5) = (Utils::rotatePoint(lowerLeft2, rotation) + position);
	points.at(6) = (Utils::rotatePoint(upperRight2, rotation) + position);
	points.at(7) = (Utils::rotatePoint(lowerRight2, rotation) + position);
}