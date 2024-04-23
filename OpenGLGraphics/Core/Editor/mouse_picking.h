#ifndef MOUSE_PICKING_H
#define MOUSE_PICKING_H


#include "glm.hpp"
#include "Graphics/Camera.h"
#include <assimp/vector3.h>


class MousePicking {

public:
	void performRayCasting(glm::vec2 mousePos ,int viewportWidth, int viewportHeight, Core::Primitives::Camera* cam);



private:
	int viewportWidth;
	int viewportHeight; 
	Core::Primitives::Camera* cam;

	glm::vec3 calculateRayDirection(glm::vec2 mousePos, int viewportWidth, int viewportHeight, Core::Primitives::Camera* cam);
	bool IntersectRayWithMesh(glm::vec3 rayOrigin, glm::vec3 rayDirection, aiVector3D minExtents, aiVector3D maxExtents);



};


#endif


