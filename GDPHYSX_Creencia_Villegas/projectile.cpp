#include "projectile.h"

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