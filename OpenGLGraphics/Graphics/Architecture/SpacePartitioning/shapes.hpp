#ifndef _SHAPES__HPP_
#define _SHAPES__HPP_

#include "math.hpp"
#include <vector>
#include <string>

struct aabb {
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 pos;
	glm::vec3 sca;

	aabb() : min{ 0.f,0.f,0.f }, max{ 0.f, 0.f, 0.f }, pos{ 0.f, 0.f, 0.f }, sca{ 0.f, 0.f, 0.f } {}
	aabb(glm::vec3 _min, glm::vec3 _max);
};



struct triangle {
	glm::vec3 a, b, c;

	triangle() { a = b = c = glm::vec3(); }
	triangle(glm::vec3 _a, glm::vec3 _b, vec3 _c) : a(_a), b(_b), c(_c) {};

	glm::vec3& operator[](int i) {
		switch (i) {
		case 0: return a;
		case 1: return b;
		case 2: return c;
		}
	}

	glm::vec3 const& operator[](int i) const {
		switch (i) {
		case 0: return a;
		case 1: return b;
		case 2: return c;
		}
	}
};

struct plane {
	plane();
	plane(const glm::vec4& plane);
	plane(const glm::vec3& _p, const glm::vec3& _n);
	plane(const plane& copy);
	plane& operator=(const plane& copy);
	float d;
	glm::vec3 n, p;
};

struct frustrum {
	mat4 mMtx;
	plane mplanes[6];
	frustrum(glm::mat4 const& mtx) { mMtx = mtx; 
	extract_planes_from_projmat(mtx); }
	mat4 get_matrix() { return mMtx; }

	void extract_planes_from_projmat(
		glm::mat4 projmtx)
	{
		const mat4 tm = transpose(projmtx);

		const vec4& x = tm[0];
		const vec4& y = tm[1];
		const vec4& z = tm[2];
		const vec4& w = tm[3];

		vec4 p[6] =
		{
			-(w + x),	//left
			-(w - x),	//right
			-(w + y),	//bottom
			-(w - y),	//top
			-(w + z),	//near
			-(w - z),	//far
		};

		for (unsigned i = 0; i < 6; i++)
		{
			float l = length(vec3(p[i]));
			p[i] /= l;
		}

		for (unsigned i = 0; i < 6; ++i) {
			mplanes[i] = p[i];
		}
	}
};


std::vector<triangle> load_triangles(std::string const& filename);
std::vector<aabb> triangles_to_aabbs(std::vector<triangle> const& tris);

#endif