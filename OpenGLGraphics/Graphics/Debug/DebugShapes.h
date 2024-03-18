#ifndef __DEBUG_SYSTEM_HPP__
#define __DEBUG_SYSTEM_HPP__

#include <glm.hpp>
#include "Graphics/Camera.h"
#include "DebugPrimitives.h"

class debug_system {
private:
public:
    debug_system(Core::Primitives::Camera const*);
    void draw_point(glm::vec3 pt, glm::vec4 color);
    void draw_segment(glm::vec3 s, glm::vec3 e, glm::vec4 color);
    void draw_triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec4 color);
    void draw_aabb(glm::vec3 c, glm::vec3 size, glm::vec4 color);
    void draw_plane(glm::vec3 pt, glm::vec3 n, float scale, glm::vec4 color);
    void draw_sphere(glm::vec3 c, float r, glm::vec4 color);
    void draw_frustum_lines(glm::mat4 const& vp, glm::vec4 color);

    void change_camera(Core::Primitives::Camera const*);

private:
    Core::Primitives::Camera const* mCamera;
    Primitive mPoint;
    Primitive mSegment;
    Primitive mTriangle;
    Primitive mAABB;
    Primitive mPlane;
    Primitive mSphere;
    Primitive mFrustum;
};

#endif // __DEBUG_SYSTEM_HPP__