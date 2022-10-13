

#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Pool_Structs
{
	struct projectileData {
		glm::vec3 velo;
		glm::vec3 accel;
		float mass;
		float damp;
		float radius;
		int type;
		int count = 1;
		float ageLimit = 2;
	};
}

template <typename T>
class ObjectPool
{
public:
	ObjectPool(size_t maxProjectiles = 1000);

	~ObjectPool();

	// Similar to RequestPoolable()
	T* alloc();

	// Similar to ReleasePoolable()
	void dealloc(T* object);
	//member variables
private:
	// free buffer
	T* m_projectiles;
	// total number of projectiles we can hold
	size_t m_maxProjectiles;
	// free list buffer; contains the indices of our projectiles
	std::vector<size_t> m_freeList;
};

// specify all the data types that will be passed to this template class,
// not doing this will result into a linker error
template class ObjectPool<int>;
template class ObjectPool<float>;
template class ObjectPool<double>;
template class ObjectPool<std::string>;

// template Specialization for a specific passed data type
template < >
class ObjectPool<Pool_Structs::projectileData>
{
public:
	ObjectPool(size_t maxProjectiles);
	~ObjectPool();
	Pool_Structs::projectileData* alloc();
	void dealloc(Pool_Structs::projectileData* object);
private:
	// free buffer
	Pool_Structs::projectileData* m_projectiles;
	// total number of projectiles we can hold
	size_t m_maxProjectiles;
	// free list buffer; contains the indices of our projectiles
	std::vector<size_t> m_freeList;
};




