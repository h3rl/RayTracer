#include "Shapes.h"

bool Sphere::hit(const Ray& ray, float t_min, float t_max, hit_record& record) const
{
    glm::vec3 oc = ray.origin - center;
    auto a = glm::dot(ray.direction,ray.direction);
    auto half_b = glm::dot(oc, ray.direction);
    auto c = glm::dot(oc,oc) - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    record.t = root;
    record.pos = ray.at(root);
    record.normal = (record.pos - center) / radius;

    return true;
}