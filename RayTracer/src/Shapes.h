#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(glm::vec3 center, float radius) : center(center), radius(radius) {};

	virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& record) const override;
private:
	glm::vec3 center;
	float radius;
};