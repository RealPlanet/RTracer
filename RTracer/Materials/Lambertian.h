#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include"Vector3D.h"
#include"Ray.h"
#include"Material.h"


class Lambertian : public Material 
{
public:
    Lambertian(const Color& a) : m_albedo(a) {}
    // Inherited via Material
    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override;

    Color m_albedo;
};

#endif // !LAMBERTIAN_H