//
//	DebugShapes.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 15/04/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef __DEBUG_SYSTEM_HPP__
#define __DEBUG_SYSTEM_HPP__

#include <glm.hpp>
#include "Graphics/Camera.h"
#include "DebugPrimitives.h"

namespace Graphics {
    namespace Debug {
        class DebugSystem {
        #pragma region //Declarations
            CLASS_EXCEPTION(DebugSystem)
        #pragma endregion

        #pragma region //Constructor
        public:
            DebugSystem();
        #pragma endregion

        #pragma region //Methods
            void DrawPoint(const glm::vec3& pt, const glm::vec4& color, const Core::Primitives::Camera& cam) const;
            void DrawSegment(const glm::vec3& s, const glm::vec3& e, const glm::vec4& color, const Core::Primitives::Camera& cam) const;
            void DrawTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec4& color, const Core::Primitives::Camera& cam);
            void DrawAABB(const glm::vec3& c, const glm::vec3& size, const glm::vec4& color, const Core::Primitives::Camera& cam) const;
            void DrawPlane(const glm::vec3& pt, const glm::vec3& n, float scale, const glm::vec4& color, const Core::Primitives::Camera& cam) const;
            void DrawSphere(const glm::vec3& c, const float r, const glm::vec4& color, const Core::Primitives::Camera& cam) const;
            void DrawFrustum(const glm::mat4& vp, const glm::vec4& color, const Core::Primitives::Camera& cam) const;
        #pragma endregion

        #pragma region //Members
        private:
            Core::Assets::Asset<Core::Graphics::ShaderProgram> mShader;
            DebugPrimitive mPoint, mSegment, mTriangle, mAABB, mPlane, mSphere, mFrustum;
        #pragma endregion
        };
    }
}

#endif // __DEBUG_SYSTEM_HPP__