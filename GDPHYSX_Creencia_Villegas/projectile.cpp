#include "projectile.h"
/*
projectile::projectile()
{

}

void projectile::initialize()
{
	sharedInstance = new projectile();
}

projectile* projectile::sharedInstance = NULL;

projectile* projectile::getInstance()
{
	return sharedInstance;
}
*/

projectile::projectileData projectile::createBullet(glm::vec3 accel, glm::vec3 velo, float mass, float damp, float radius)
{
	projectileData newData;
	newData.accel = accel;
	newData.velo = velo;
	newData.mass = mass;
	newData.damp = damp;
	newData.radius = radius;

	return newData;
}
