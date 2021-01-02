#ifndef MATERIAL_H
#define MATERIAL_H

#include "VMath.h"

#include "Hittable.h"
#include "Texture.h"

class Material
{
public:
    virtual bool scatter( const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered ) const = 0;
    virtual Color emitted(double u, double v, const Point3D& p) const { return Color(0, 0, 0); }
};

class Lambertian : public Material
{
public:
    Lambertian(const Color& a) : albedo(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo(a) {}
    // Inherited via Material
    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override;

    Color m_albedo;
    shared_ptr<Texture> albedo;
};

class Metal : public Material
{
public:
    Metal(const Color& a, double f) : m_albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override;

public:
    Color m_albedo;
    double fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override;
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx);
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
    DiffuseLight(Color c) : emit(make_shared<SolidColor>(c)) {}

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered) const override { return false; }
    virtual Color emitted(double u, double v, const Point3D& p) const override { return emit->value(u, v, p); }
    shared_ptr<Texture> emit;
};

class Isotropic : public Material {
public:
    Isotropic(Color c) : albedo(make_shared<SolidColor>(c)) {}
    Isotropic(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered ) const override
    {
        scattered = Ray(rec.point, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.point);
        return true;
    }

public:
    shared_ptr<Texture> albedo;
};

#endif
