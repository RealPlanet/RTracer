#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vector3D.h"

class Sphere : public Hittable
{
public:
    Sphere() {}
    Sphere(Point3D cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m){};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;

    Point3D center;
    double radius = 0.0f;
    shared_ptr<Material> mat_ptr;
};

#endif // !SPHERE_H



