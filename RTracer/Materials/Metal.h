#ifndef METAL_H
#define METAL_H

#include "Material.h"
#include "Vector3D.h"

class Metal : public Material
{
public:
    Metal(const Color& a, double f) : m_albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override;

public:
    Color m_albedo;
    double fuzz;
};


#endif