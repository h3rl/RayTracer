#pragma once
#include <glm/glm.hpp>

class Light
{
public:
	Light(glm::vec3 pos) : position(pos) {};

public:
	glm::vec3 position;
};