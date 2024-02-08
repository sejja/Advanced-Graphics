//
//  Transform.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 12/03/23
//  Copyright � 2023. All rights reserved.
//

#ifndef _TRANSFORM__H_
#define _TRANSFORM__H_

#include <glm/glm.hpp>

namespace Mathematics {
	struct Transformation {
		glm::vec3 mPostion;
		glm::vec3 mScale;
		glm::vec3 mRotation;
	};
}

#endif