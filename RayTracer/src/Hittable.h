#pragma once
#include <glm/glm.hpp>

#include "Ray.h"

struct hit_record
{
	glm::vec3 pos;
	glm::vec3 normal;
	float t;
};

class Hittable
{
public:
	virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& record) const = 0;
};