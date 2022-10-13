#include "ObjectPooling.h"

#include <cassert>
#include <iostream>

template <typename T>
ObjectPool<T>::ObjectPool(size_t maxProjectiles)
{
	// allocate memory for our free buffer
	m_projectiles = (T*)malloc(sizeof(T) * maxProjectiles);
	m_freeList.resize(maxProjectiles);
	// adding objects by 'index', and we do this in reverse
	// we set the vector to stack-like behavior, and we don't want to fragment memory
	// this will make our vector.top to be the end of our element list
	for (size_t i = 0; i < maxProjectiles; i++)
		m_freeList[i] = maxProjectiles - i - 1;
	//set the given parameter value for the m_maxProjectiles
	m_maxProjectiles = maxProjectiles;
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
	// checks if there are still projectiles that are not release before we free our
	// projectile pool
	assert(m_freeList.size() == m_maxProjectiles);
	// free pointer memory
	if (m_projectiles != nullptr)
		free(m_projectiles);
}

template <typename T>
T* ObjectPool<T>::alloc()
{
	// if the free list is empty
	if (m_freeList.empty())
		return nullptr;
	// get the last item of our free list and used it as an index for our projectile's buffer
	T* ret = new(&m_projectiles[m_freeList.back()]) T();
	// remove the used projectile
	m_freeList.pop_back();
	std::cout << "Request Pool!" << std::endl;
	return ret;
}

template <typename T>
void ObjectPool<T>::dealloc(T* object)
{
	// when we subtract the passed object(object)'s address to the beginning address(m_projectiles[0]),
	// it will return the index of the passed projectile object
	size_t index = (size_t)(object - m_projectiles);
	// if the index is within our maxProjectileSize, then it is valid
	assert(index < m_maxProjectiles);
	// checks if the passed object is found in the freelist, then it is invalid
	assert(std::find(m_freeList.begin(), m_freeList.end(), index) == m_freeList.end());
	// returns the projectile back to our freeList
	m_freeList.push_back((size_t)(index));
	std::cout << "Return Pool!" << std::endl;
}

//////////////////////////////////////////////////////////
 /*
  * This next area is for template specialization
  */
  //////////////////////////////////////////////////////////
  ///Specialization for "Pool_Structs::Projectile"
ObjectPool<Pool_Structs::projectileData>::ObjectPool(size_t maxProjectiles)
{
	// allocate memory for our free buffer
	m_projectiles = (Pool_Structs::projectileData*)malloc(sizeof(Pool_Structs::projectileData) * maxProjectiles);
	m_freeList.resize(maxProjectiles);
	// adding objects by 'index', and we do this in reverse
	// we set the vector to stack-like behavior, and we don't want to fragment memory
	// this will make our vector.top to be the end of our element list
	for (size_t i = 0; i < maxProjectiles; i++)
		m_freeList[i] = maxProjectiles - i - 1;
	//set the given parameter value for the m_maxProjectiles
	m_maxProjectiles = maxProjectiles;
}

ObjectPool<Pool_Structs::projectileData>::~ObjectPool()
{
	// checks if there are still projectiles that are not release before we free our
	// projectile pool
	assert(m_freeList.size() == m_maxProjectiles);
	// free pointer memory
	if (m_projectiles != nullptr)
		free(m_projectiles);
}

Pool_Structs::projectileData* ObjectPool<Pool_Structs::projectileData>::alloc()
{
	// if the free list is empty
	if (m_freeList.empty())
		return nullptr;
	// get the last item of our free list and used it as an index for our projectile's buffer
	Pool_Structs::projectileData* ret = new(&m_projectiles[m_freeList.back()]) Pool_Structs::projectileData();
	// remove the used projectile
	m_freeList.pop_back();
	std::cout << "Request Projectile Pool!" << std::endl;
	return ret;
}

void ObjectPool<Pool_Structs::projectileData>::dealloc(Pool_Structs::projectileData* object)
{
	// when we subtract the passed object(object)'s address to the beginning address(m_projectiles[0]),
	// it will return the index of the passed projectile object
	size_t index = (size_t)(object - m_projectiles);
	// if the index is within our maxProjectileSize, then it is valid
	assert(index < m_maxProjectiles);
	// checks if the passed object is found in the freelist, then it is invalid
	assert(std::find(m_freeList.begin(), m_freeList.end(), index) == m_freeList.end());
	// calls the destructor manually, since we've used "placement new" in our alloc() method
	object->~projectileData();
	// returns the projectile back to our freeList
	m_freeList.push_back((size_t)(index));
	std::cout << "Return Projectile Pool!" << std::endl;
}


