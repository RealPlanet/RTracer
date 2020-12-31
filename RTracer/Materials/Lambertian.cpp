#include "Lambertian.h"

bool Lambertian::scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const
{
    auto scatter_direction = rec.normal + random_unit_vector();
    scattered = Ray(rec.point, scatter_direction);
    attenuation = m_albedo;
    // Catch degenerate scatter direction

    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

    return true;
}
