#ifndef SPHERE_H
#define SPHERE_H

#include "VMath.h"
#include "Hittable.h"

class Sphere : public Hittable
{
public:
    Sphere() {}
    Sphere(Point3D cen, double r, shared_ptr<Material> m) : center(cen), radius(r), mat_ptr(m){};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

    Point3D center;
    double radius = 0.0f;
    shared_ptr<Material> mat_ptr;
private:
    static void get_sphere_uv(const Point3D& p, double& u, double& v);
};


#endif // !SPHERE_H



