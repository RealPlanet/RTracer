#include "Metal.h"


bool Metal::scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const
{
    Vector3D reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.point, reflected + fuzz * random_in_unit_sphere());
    attenuation = m_albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}

