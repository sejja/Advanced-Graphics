#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "math.hpp"
#include "shapes.hpp"

enum eResult
{
    eINSIDE      = -1,
    eOVERLAPPING = 0,
    eOUTSIDE     = 1
};

glm::vec3 closest_point_plane(const vec3& point, const vec3& plane_normal, const float point_dot_normal);
glm::vec3 closest_segment_segment(const vec3& p1, const vec3& q1, const vec3& p2, const vec3& q2, glm::vec3* result1, glm::vec3* result2);
bool overlap_point_aabb(const vec3& point, const vec3& aabb_min, const vec3& aabb_max);
bool overlap_point_sphere(const vec3& point, const vec3& sphere_center, const float sphere_radius);
eResult classify_plane_point(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& point);
eResult classify_plane_triangle(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& a, const vec3& b, const vec3& c);
eResult classify_plane_aabb(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& aabb_min, const vec3& aabb_max);
eResult classify_plane_sphere(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& sphere_center, const float sphere_radius);
bool overlap_aabb_aabb(const vec3& aabb1_min, const vec3& aabb1_max, const vec3& aabb2_min, const vec3& aabb2_max);
bool overlap_sphere_sphere(const vec3& sphere1_center, const float sphere1_radius, const vec3& sphere2_center, const float sphere2_radius);
float intersection_time_ray_plane(const vec3& rayorigin, const vec3& raydir, const vec3& planenormal, const float planenormal_dot_planepos);
float intersection_time_ray_aabb(const vec3& rayorigin, const vec3& raydir, const vec3& aabb_min, const vec3& aabb_max);
float intersection_time_ray_sphere(const vec3& rayorigin, const vec3& raydir, const vec3& sphere_center, const float sphere_radius);
float intersection_time_ray_triangle(const vec3& rayorigin, const vec3& raydir, const vec3& a, const vec3& b, const vec3& c);
eResult classify_frustum_sphere_naive(vec3 frustrumnormals[6], float frustrumplaned[6], vec3 spherepos, float radius);
eResult classify_frustum_aabb_naive(vec3 frustrumnormals[6], float frustrumplaned[6], vec3 aabbmin, vec3 aabbmax);
eResult classify_frustum_aabb_naive(const frustrum& f, const aabb& bv);

#endif // __GEOMETRY_HPP__
