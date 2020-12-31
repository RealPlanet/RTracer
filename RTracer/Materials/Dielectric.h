#pragma once
#include "Material.h"

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override;
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx);
};
