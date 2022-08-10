#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "Walnut/Image.h"

class Camera;
class World;
class Ray;

class Renderer
{
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Renderer::Render(Camera& camera, World& scene);

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
private:
	glm::vec3 Renderer::PerPixel(Ray& ray, World& scene, int depth);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};