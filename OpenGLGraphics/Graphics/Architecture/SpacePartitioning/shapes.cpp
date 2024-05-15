#include "shapes.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

aabb::aabb(glm::vec3 _min, glm::vec3 _max) : min(_min), max(_max) {
	pos = glm::vec3((_max.x + _min.x) / 2, (_max.y + _min.y) / 2, (_max.z + _min.z) / 2);
	sca = glm::vec3(glm::abs(_max.x - _min.x), glm::abs(_max.y - _min.y), glm::abs(_max.z - _min.z));
}

std::vector<triangle> load_triangles(std::string const& filename) {
		std::vector<triangle> tris;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Could not open file " << filename << std::endl;
		return tris;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') continue;
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "v") {
			glm::vec3 v;
			ss >> v.x >> v.y >> v.z;
		}
		else if (type == "f") {
			triangle t;
			ss >> t.a.x >> t.a.y >> t.a.z;
			ss >> t.b.x >> t.b.y >> t.b.z;
			ss >> t.c.x >> t.c.y >> t.c.z;
			tris.push_back(t);
		}
	}
	return tris;
}

std::vector<aabb> triangles_to_aabbs(std::vector<triangle> const& tris) {
	std::vector<aabb> bvs;
	for (auto const& t : tris) {
		aabb bv;
		bv.min = glm::min(glm::min(t.a, t.b), t.c);
		bv.max = glm::max(glm::max(t.a, t.b), t.c);
		bvs.push_back(bv);
	}
	return bvs;
}

plane::plane()
	: d(0)
	, n(vec3(0))
	, p(vec3(0))
{}

plane::plane(const vec4& plane)
	: d(-plane.w)
	, n(plane)
	, p(n* d)
{}

plane::plane(const vec3& _p, const vec3& _n)
	: d(dot(_p, _n))
	, n(_n)
	, p(_p)
{}

plane::plane(const plane& copy)
	: d(copy.d)
	, n(copy.n)
	, p(copy.p)
{}

plane& plane::operator=(const plane& copy)
{
	d = copy.d;
	n = copy.n;
	p = copy.p;
	return *this;
}
