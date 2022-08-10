#include "Camera.h"

#include <imgui.h>
#include <iostream>
#include <Windows.h>


Camera::Camera()
{
    //auto aspect_ratio = 4.0f / 3.0f;
    //auto viewport_height = 2.0f;
    //auto viewport_width = viewport_height * aspect_ratio;
    const float focal_length = 1.0f;

    origin = { 0,0,-2.5f };
    direction = { 0,0, 0 };
    screen_center = {0,0,0};
    screen_right = { 0,0,0 };
    screen_up = { 0,0,0 };
    yaw = -90;
    pitch = 0;
    old_mouse_pos = { 0,0 };

    POINT p;
    if (GetCursorPos(&p))
    {
        old_mouse_pos.x = p.x;
        old_mouse_pos.y = p.y;
    }
}

Ray Camera::get_ray(float u, float v)
{
    glm::vec3 pos_on_screen_plane = screen_center + screen_right * u + screen_up * v;
    glm::vec3 dir = pos_on_screen_plane - origin;
    return Ray(origin, dir);
}

glm::vec3 directionFromAngles(float& pitch, float& yaw)
{
    auto pitch_radians = degrees_to_radians(pitch);
    auto yaw_radians = degrees_to_radians(yaw);

    auto xzLen = glm::cos(pitch_radians);
    float x = xzLen * cos(yaw_radians);
    float y = sin(pitch_radians);
    float z = xzLen * sin(-yaw_radians);
      
    return glm::vec3(x, y, z);
}

void Camera::OnMove(float dt)
{
    if (dt <= 0.0f) return;


    const float move_speed = 1.0f;
    const float mouse_sens = 0.5f;

    ImVec2 pos = ImGui::GetMousePos();
    glm::vec2 npos = { pos.x,pos.y };
    glm::vec2 delta = npos - old_mouse_pos;
    old_mouse_pos = npos;

    pitch += delta.y * mouse_sens;
    yaw += delta.x * mouse_sens;

    pitch = glm::clamp(pitch, -90.f, 90.f);
    while (yaw > 180.0f || yaw <= -180.0f)
    {
        yaw -= (yaw < 0 ? -360.f : 360.f);
    }

    direction = directionFromAngles(pitch, yaw);

    const glm::vec3 up = { 0,1,0 };
    if (direction.x == 0.0f && direction.z == 0.0f)
    {
        direction.x = 1.0f;
    }

    screen_right = glm::cross(direction, up); // no need to normalize, vecs are len(1)
    screen_up = glm::cross(screen_right, direction);

    if (ImGui::IsKeyPressed(ImGuiKey_W))
    {
        origin += direction * dt * move_speed;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_S))
    {
        origin -= direction * dt * move_speed;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_A))
    {
        origin -= screen_right * dt * move_speed;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_D))
    {
        origin += screen_right * dt * move_speed;
    }

    screen_center = origin + direction * focal_length;
}
