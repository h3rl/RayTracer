#include <memory>

#include "World.h"
#include "Shapes.h"

using std::make_shared;

World::World()
{
	this->add(make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f));
	this->add(make_shared<Sphere>(glm::vec3(0.0f, -100.5f, 0.0f), 100.f));
	this->add(make_shared<Light>(glm::vec3(1.0f, 1.0f, 0.0f)));

}

bool World::hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const
{
	hit_record temp_record;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& obj : objects)
	{

		if (obj->hit(ray, t_min, t_max, temp_record))
		{
			if (closest_so_far < temp_record.t)
			{
				continue;
			}
			hit_anything = true;
			closest_so_far = temp_record.t;
			rec = temp_record;
		}
	}

	//glm::vec3 emition = glm::vec3(1.0f, 0.0f, 0.0f);

	//for (const auto& light : lights)
	//{
	//	float dot = glm::dot(glm::normalize(light->position), rec.normal);
	//	//printf("%.3f\n", dot);
	//	if (dot > 0)
	//		dot * 
	//}

	return hit_anything;
}


