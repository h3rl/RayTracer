#pragma once

#include "Hittable.h"
#include "Light.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class World : public Hittable
{
public:
	World();
	World(shared_ptr<Hittable> object) { add(object); };

	void clear() { objects.clear(); lights.clear(); };
	void add(shared_ptr<Hittable> object) { objects.push_back(object); };
	void add(shared_ptr<Light> object) { lights.push_back(object); };

	virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& record) const override;

private:
	std::vector<shared_ptr<Hittable>> objects;
	std::vector<shared_ptr<Light>> lights;
};