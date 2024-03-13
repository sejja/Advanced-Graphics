//
//  Transform.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/03/24
//  Copyright � 2024. All rights reserved.
//

#ifndef _TRANSFORM__H_
#define _TRANSFORM__H_

#include "glm.hpp"
namespace Core {
	namespace Mathematics {
		struct Transformation {
		#pragma region //Members
			glm::vec3 mPostion, mScale, mRotation;
		#pragma endregion
		};
	}
}

#endif