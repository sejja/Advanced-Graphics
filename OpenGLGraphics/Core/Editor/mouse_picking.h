#ifndef MOUSE_PICKING_H
#define MOUSE_PICKING_H


#include "glm.hpp"
#include "Graphics/Camera.h"
#include <assimp/vector3.h>


class MousePicking {

public:
	void performRayCasting(glm::vec2 mousePos ,int viewportWidth, int viewportHeight, Core::Primitives::Camera* cam);



private:
	glm::vec3 calculateRayDirection(glm::vec2 mousePos, int viewportWidth, int viewportHeight, Core::Primitives::Camera* cam);

	//--------------------------------------------------------------------------------------------
	// IntersectRayWithMesh
	// 
	// Returns the distance from the cam to the intersection point
	//--------------------------------------------------------------------------------------------
	float IntersectRayWithMesh(glm::vec3 rayOrigin, glm::vec3 rayDirection, aiVector3D mMin, aiVector3D mMax, glm::vec3 scale, glm::vec3 position);



};


#endif


