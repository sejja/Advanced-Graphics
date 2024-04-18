//
//	DebugPrimitives.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 15/04/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef __PRIMITIVE_HPP__
#define __PRIMITIVE_HPP__

#include <vec3.hpp>
#include "Graphics/Primitives/ShaderProgram.h"

namespace Graphics {
	namespace Debug {
		class DebugPrimitive {
#pragma region //Constructor & Destructor
		public:
			DebugPrimitive();
			~DebugPrimitive();
#pragma endregion

#pragma region //Methods
			void InitPoint();
			void InitSegment();
			void InitAABB();
			void InitTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
			void InitPlane();
			void InitSphereDisc();
			void InitFrustum();
			void Draw() const;
		private:
			void InitializeBuffers(std::vector<glm::vec3>& vertices);
#pragma endregion

#pragma region //Members
			GLuint mVAO, mVBO;
			GLsizei mVertexCount;
#pragma endregion
		};
	}
}

#endif // __PRIMITIVE_HPP__