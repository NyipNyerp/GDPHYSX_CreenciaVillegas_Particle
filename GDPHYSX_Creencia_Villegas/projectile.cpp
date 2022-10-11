#include "projectile.h"

projectile::projectileData projectile::createBullet(glm::vec3 accel, glm::vec3 velo, float mass, float damp, float radius, int type)
{
	projectileData newData;
	newData.accel = accel;
	newData.velo = velo;
	newData.mass = mass;
	newData.damp = damp;
	newData.radius = radius;
	newData.type = type;

	return newData;
}