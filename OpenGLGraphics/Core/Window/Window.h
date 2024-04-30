//
//  Window.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 12/4/24.
//  Copyright � 2024 . All rights reserved.
//

#ifndef _WINDOW__H_
#define _WINDOW__H_

#include "glm.hpp"

namespace Core {
	namespace Window {
		class Window {
		#pragma region //Functions
		public:
			virtual void Create() = 0;
			virtual bool Present() = 0;
			virtual void SetDimensions(const glm::lowp_u16vec2& dim) = 0;
		#pragma endregion
		};
	}
}

#endif