#include "geometry.hpp"
#include <algorithm>

glm::vec3 closest_point_plane(const vec3& point, const vec3& plane_normal, const float point_dot_normal) {
	float distance = glm::dot(plane_normal, point) - point_dot_normal;
	return point - distance * plane_normal;
}

glm::vec3 closest_segment_segment(const vec3& p1, const vec3& q1, const vec3& p2, const vec3& q2, glm::vec3* result1, glm::vec3* result2) {
	vec3 v = q1 - p1; // Segment A dir
	vec3 w = q2 - p2; // Segment B dir
	vec3 k = p1 - p2;

	float a = dot(v, v);
	float b = dot(v, w);
	float c = dot(w, w);
	float d = dot(v, k);
	float e = dot(w, k);

	float s = 0.0f; // tA
	float t = 0.0f; // tB

	float div = (b * b - a * c);

	// Until here, treated as a line

	if (div != 0.0f) // Check if parallel
		t = glm::clamp((b * d - a * e) / div, 0.0f, 1.0f);

	if (a != 0) // Avoid nan
		s = glm::clamp((-d + b * t) / a, 0.0f, 1.0f);
	else
		s = glm::clamp((-e + c * t) / b, 0.0f, 1.0f);

	if (c != 0) // Avoid nan
		t = glm::clamp((e + s * b) / c, 0.0f, 1.0f);
	else
		t = glm::clamp((d + a * b) / b, 0.0f, 1.0f);

	*result1 = p1 + v * s; // Segment A closest point
	*result2 = p2 + w * t; // Segment B closest point

	return *result1 - *result2;
}

bool overlap_point_aabb(const vec3& point, const vec3& aabb_min, const vec3& aabb_max) {
	return (aabb_min.x <= point.x && point.x <= aabb_max.x)
		&& (aabb_min.y <= point.y && point.y <= aabb_max.y)
		&& (aabb_min.z <= point.z && point.z <= aabb_max.z);
}

bool overlap_point_sphere(const vec3& point, const vec3& sphere_center, const float sphere_radius) {
	return sphere_radius >= length(point - sphere_center);
}

eResult classify_plane_point(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& point) {
	float t = glm::dot(point, plane_normal) - planepos_dot_planenormal; // Get time by formula

	return t * t < std::numeric_limits<float>::epsilon() ? eOVERLAPPING : t < 0 ? eINSIDE : eOUTSIDE;
}

eResult classify_plane_triangle(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& a, const vec3& b, const vec3& c) {
	int first = classify_plane_point(plane_normal, planepos_dot_planenormal, a);
	if (first == eOVERLAPPING) // If first == overlap, done, so skip others
		return eOVERLAPPING;

	// first != overlap, so any diferent result will be an overlap
	if (first != classify_plane_point(plane_normal, planepos_dot_planenormal, b)) return eOVERLAPPING;
	if (first != classify_plane_point(plane_normal, planepos_dot_planenormal, c)) return eOVERLAPPING;

	return eResult(first);
}

eResult classify_plane_aabb(const vec3& plane_normal, float d, const vec3& aabb_min, const vec3& aabb_max) {
	vec3 c = (aabb_max + aabb_min) * 0.5f; // center
	vec3 h = aabb_max - c; // half size

	float s = dot(plane_normal, c) - d; // distance between aabb center and plane
	eResult noOverlap = s < 0 ? eINSIDE : eOUTSIDE;
	float s2 = s * s;

	if (s2 > glm::length2(h))
		return noOverlap;

	vec3 nAbs = abs(plane_normal);
	float r = 0; // AABB projection radius
	r += nAbs.x * h.x;
	r += nAbs.y * h.y;
	r += nAbs.z * h.z;
	float r2 = r * r;

	return s2 - r2 < std::numeric_limits<float>::epsilon() ? eOVERLAPPING : noOverlap;
}

eResult classify_plane_sphere(const vec3& plane_normal, const float planepos_dot_planenormal, const vec3& sphere_center, const float sphere_radius) {
	float t = (dot(sphere_center, plane_normal) - planepos_dot_planenormal) / length2(plane_normal); // Get time by formula

	return (t * t - sphere_radius * sphere_radius) < std::numeric_limits<float>::epsilon() ? eOVERLAPPING : t < 0 ? eINSIDE : eOUTSIDE;
}

bool overlap_aabb_aabb(const vec3& aabb1_min, const vec3& aabb1_max, const vec3& aabb2_min, const vec3& aabb2_max) {
	return	aabb1_min.x <= aabb2_max.x && aabb1_min.y <= aabb2_max.y && aabb1_min.z <= aabb2_max.z &&
		aabb1_max.x >= aabb2_min.x && aabb1_max.y >= aabb2_min.y && aabb1_max.z >= aabb2_min.z;
}

bool overlap_sphere_sphere(const vec3& sphere1_center, const float sphere1_radius, const vec3& sphere2_center, const float sphere2_radius) {
	return distance(sphere1_center, sphere2_center) <= (sphere1_radius + sphere2_radius);
}

float intersection_time_ray_plane(const vec3& rayorigin, const vec3& raydir, const vec3& planenormal, const float planenormal_dot_planepos) {
	float dotNV = dot(planenormal, raydir);
	if (dotNV * dotNV < std::numeric_limits<float>::epsilon()) // Parallel check
		return -1.0f;

	float t = (planenormal_dot_planepos - dot(planenormal, rayorigin)) / dotNV; // Get time by formula

	return t < 0 ? -1.0f : t;
}

float intersection_time_ray_aabb(const vec3& rayorigin, const vec3& raydir, const vec3& aabb_min, const vec3& aabb_max) {
	if (overlap_point_aabb(rayorigin, aabb_min, aabb_max)) // Ray is inside
		return 0.0f;

	vec3 tMin3(0);
	vec3 tMax3(0);

	for (unsigned i = 0; i < 3; ++i)
	{
		if (raydir[i] == 0) // Axis parallel
		{
			// Check parallel is out
			if (rayorigin[i] < aabb_min[i] || aabb_max[i] < rayorigin[i])
				return -1.0f;

			// Set to a value that is gonna be omitted
			tMin3[i] = -INFINITY;
			tMax3[i] = INFINITY;
		}
		else if (raydir[i] < 0) // Negative ray dir, so swap min max values
		{
			tMin3[i] = ((aabb_max[i] - rayorigin[i]) / raydir[i]);
			tMax3[i] = ((aabb_min[i] - rayorigin[i]) / raydir[i]);
		}
		else // Just normal computation
		{
			tMin3[i] = ((aabb_min[i] - rayorigin[i]) / raydir[i]);
			tMax3[i] = ((aabb_max[i] - rayorigin[i]) / raydir[i]);
		}
	}
	// Biggest minimum value of the time intervals
	float tMin = std::max(-1.0f, std::max(tMin3.x, std::max(tMin3.y, tMin3.z)));
	if (tMin < 0)
		return -1.0f;

	// Smallest maximum value of the time intervals
	float tMax = std::min(INFINITY, std::min(tMax3.x, std::min(tMax3.y, tMax3.z)));
	if (tMax < 0)
		return -1.0f;

	return tMin <= tMax ? tMin : -1.0f; // Check time interval correctness
}

float intersection_time_ray_sphere(const vec3& rayorigin, const vec3& raydir, const vec3& sphere_center, const float sphere_radius) {
	if (length(rayorigin - sphere_center) <= sphere_radius) // Check inside
		return 0.0f;

	// By using a quadratic equation [Ax^2 + Bx + C = 0]

	float a = length2(raydir); // Wrong input
	if (a == 0)
		return -1.0f;

	vec3 cs = rayorigin - sphere_center;
	float b = 2.0f * (dot(raydir, cs));
	float c = length2(cs) - sphere_radius * sphere_radius;

	float inSqrt = b * b - 4 * a * c;
	if (inSqrt < 0)
		return -1.0f;

	float div = 2 * a;
	b *= -1;
	if (inSqrt == 0) // One result == tangent 
	{
		float t = b / div;
		return t < 0 ? -1.0f : t;
	}

	inSqrt = sqrt(inSqrt);
	float t0 = (b + inSqrt) / div;
	float t1 = (b - inSqrt) / div;

	// Avoid negative time
	bool avoid0 = t0 < 0;
	bool avoid1 = t1 < 0;
	if (avoid0) {
		if (avoid1)
			return -1.0f;
		return t1;
	}
	if (avoid1)
		return t0;
	return std::min(t0, t1);
}

float intersection_time_ray_triangle(const vec3& rayorigin, const vec3& raydir, const vec3& a, const vec3& b, const vec3& c) {
	vec3 ab(b - a);
	vec3 bc(c - b);
	vec3 n = normalize(cross(ab, bc));

	float d = dot(n, a);
	float t = (d - dot(n, rayorigin)) / dot(n, raydir);

	if (t < 0) // Behind ray
		return -1.0f;

	vec3 point = rayorigin + raydir * t;

	vec3 ap(point - a);
	if (dot(n, cross(ab, ap)) < 0) // Outside ab segment
		return -1.0f;

	vec3 bp(point - b);
	if (dot(n, cross(bc, bp)) < 0) // Outside bc segment
		return -1.0f;

	vec3 ca(a - c);
	vec3 cp(point - c);
	if (dot(n, cross(ca, cp)) < 0) // Outside ca segment
		return -1.0f;

	return t;
}

eResult classify_frustum_sphere_naive(vec3 frustrumnormals[6], float frustrumplaned[6], vec3 spherepos, float radius) {
	bool overlapped = false; // One or more overlaps && no out == body overlaps
	for (unsigned i = 0; i < 6; ++i) {
		int result = classify_plane_sphere(frustrumnormals[i], frustrumplaned[i], spherepos, radius);
		if (result == eOUTSIDE) // One out == body out
			return eOUTSIDE;
		else if (result == eOVERLAPPING)
			overlapped = true;
	}

	return overlapped ? eOVERLAPPING : eINSIDE;
}

eResult classify_frustum_aabb_naive(vec3 frustrumnormals[6], float frustrumplaned[6], vec3 aabbmin, vec3 aabbmax) {
	bool overlapped[6] = {false, false, false, false, false, false}; // One or more overlaps && no out == body overlaps
	for (unsigned i = 0; i < 6; ++i) {
		int result = classify_plane_aabb(frustrumnormals[i], frustrumplaned[i], aabbmin, aabbmax);
		if (result == eINSIDE)
			overlapped[i] = true;
		if (result == eOUTSIDE) // One out == body out
			overlapped[i] = false;
	}

	if(overlapped[0] && overlapped[1] && overlapped[2] && overlapped[3] && overlapped[4]
		&& overlapped[5])
		return eINSIDE;
	else if(!overlapped[0] && !overlapped[1] && !overlapped[2] && !overlapped[3] && !overlapped[4]
		&& !overlapped[5])
		return eOUTSIDE;
	else
		return eOVERLAPPING;
}

eResult classify_plane_aabb(const plane& plane, const aabb& aabb)
{
	return classify_plane_aabb(plane.n, plane.d, aabb.min, aabb.max);
}

eResult classify_frustum_aabb_naive(const frustrum& f, const aabb& bv)
{
	bool overlapped = false; // One or more overlaps && no out == body overlaps
	for (unsigned i = 0; i < 6; ++i)
	{
		int result = classify_plane_aabb(f.mplanes[i], bv);
		if (result == eINSIDE)
			continue;
		if (result == eOUTSIDE) // One out == body out
			return eOUTSIDE;
		overlapped = true;
	}

	return overlapped ? eOVERLAPPING : eINSIDE;
}