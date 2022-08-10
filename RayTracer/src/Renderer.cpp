#include "Walnut/Random.h"

#include "Renderer.h"
#include "Camera.h"
#include "Shapes.h"
#include "World.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

#include <iostream>

void Renderer::Render(Camera& camera, World& world)
{
	const int samples_per_pixel = 2;
	const int max_depth = 50;
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{

			//glm::vec3 color;

			//for (int k = 0; k < samples_per_pixel; k++)
			//{
			//	float u = ((float)x + Walnut::Random::Float()) / (float)m_FinalImage->GetWidth();
			//	float v = ((float)y + Walnut::Random::Float()) / (float)m_FinalImage->GetHeight();

			//	u = u * 2.0f - 1.0f;// -1 -> 1
			//	v = v * 2.0f - 1.0f;// -1 -> 1
			//	//printf("%.2f %.2f\n", u, v);

			//	Ray ray = camera.get_ray(u, v);

			//	color += PerPixel(ray, scene, max_depth);
			//}
			//color /= samples_per_pixel;

			glm::vec3 color;

			float u = (float)x / (float)m_FinalImage->GetWidth();
			float v = (float)y / (float)m_FinalImage->GetHeight();

			u = u * 2.0f - 1.0f;// -1 -> 1
			v = v * 2.0f - 1.0f;// -1 -> 1
			//printf("%.2f %.2f\n", u, v);

			Ray ray = camera.get_ray(u, v);

			color = PerPixel(ray, world, max_depth);

			color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA({ color,1.0f });
		}
	}
	//m_FinalImage->GetHeight();

	m_FinalImage->SetData(m_ImageData);
}

#include "Constants.h"

glm::vec3 Renderer::PerPixel(Ray& ray, World& world, int depth)
{
	hit_record rec;
	glm::vec3 light(1, 1, 1);

	if (depth <= 0)
	{
		return glm::vec4(0.2, 0.2, 0.2, 1.0);
	}

	if (world.hit(ray, 0.001, infinity, rec))
	{
		return glm::vec3(1, 0, 0);
		auto dot = glm::dot(glm::normalize(light), rec.normal);
		//printf("%.3f\n", dot);
		if(dot > 0)
			return dot * glm::vec3(1, 0, 0);
		return glm::vec3(0, 0, 0);/*
		glm::vec3 target = rec.pos + Walnut::Random::InHemnisphere(rec.normal);
		return 0.5f * PerPixel(Ray(rec.pos, target - rec.pos), world, depth - 1);*/
	}
	return glm::vec3(0, 0, 0);

	float t = 0.5f * (ray.direction.y + 1.0f);
	return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
	
	//glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
	//glm::vec3 rayDirection(coord.x, coord.y, -1.0f);

	float radius = 0.5f;
	// rayDirection = glm::normalize(rayDirection);

	// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	// where
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance

	float a = glm::dot(ray.direction, ray.direction);
	float b = 2.0f * glm::dot(ray.origin, ray.direction);
	float c = glm::dot(ray.origin, ray.origin) - radius * radius;

	// Quadratic forumula discriminant:
	// b^2 - 4ac

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return glm::vec4(0, 0, 0, 1);
	return glm::vec4(1, 0, 0, 1);

	// Quadratic formula:
	// (-b +- sqrt(discriminant)) / 2a

	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)

	glm::vec3 hitPoint = ray.origin + ray.direction * closestT;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
	float lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(angle)

	glm::vec3 sphereColor(1, 0, 1);
	sphereColor *= lightIntensity;
	return glm::vec4(sphereColor, 1.0f);
}
