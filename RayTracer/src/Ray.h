#pragma once
#include <glm/glm.hpp>

class Ray
{
public:
	Ray() = default;
	Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(glm::normalize(direction)) {};

	glm::vec3 at(float t) const
	{
		return origin + direction * t;
	}

public:
	glm::vec3 origin;
	glm::vec3 direction;
};

