#include "Material.h"

bool Lambertian::scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const
{
    auto scatter_direction = rec.normal + random_unit_vector();
    attenuation = m_albedo;
    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
    {
        scatter_direction = rec.normal;
    }
    scattered = Ray(rec.point, scatter_direction, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.point);
    return true;
}

bool Metal::scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const
{
    Vector3D reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.point, reflected + fuzz * random_in_unit_sphere(), r_in.time());
    attenuation = m_albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}

bool Dielectric::scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const
{
    attenuation = Color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    Vector3D unit_direction = unit_vector(r_in.direction());

    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3D direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = Ray(rec.point, direction, r_in.time());
    return true;
}

double Dielectric::reflectance(double cosine, double ref_idx)
{
    // Use Schlick's approximation for reflectance.
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
