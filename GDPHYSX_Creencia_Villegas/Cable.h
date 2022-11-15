#pragma once
#include "ParticleLink.h"
class Cable : public ParticleLink
{
public:
	float length;
	float restitution = 0;
	MyVector anchorPoint = MyVector(0, 0, 0);

	ParticleContact* getContact() override;
};
