#pragma once
#include <glm/glm.hpp>

#include "Ray.h"

#include "Constants.h"

struct viewangles
{
    float pitch = 0.0f;
    float yaw = 0.0f;
};

class Camera
{
public:
	Camera();
    Ray get_ray(float u, float v);

    void OnMove(float dt);

    const float Pitch() { return pitch; };
    const float Yaw() { return yaw; };
private:
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 screen_center;
    glm::vec3 screen_right;
    glm::vec3 screen_up;
    const float focal_length = 1.0f;
    float pitch;
    float yaw;

    glm::vec2 old_mouse_pos;
};