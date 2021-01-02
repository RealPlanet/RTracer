#ifndef AARECT_H
#define AARECT_H

#include "VMath.h"
#include "Hittable.h"

class XYRect : public Hittable
{
public:
    XYRect() {}
    XYRect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat) : x0{ _x0 }, x1{ _x1 }, y0{ _y0 }, y1{_y1}, k{_k}, mp{ mat } {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        output_box = AABB(Point3D(x0, y0, k - 0.0001), Point3D(x1, y1, k + 0.0001));
        return true;
    }

    shared_ptr<Material> mp;
    double x0 = 0.0f;
    double x1 = 0.0f;
    double y0 = 0.0f;
    double y1 = 0.0f;
    double k = 0.0f;
};

class XZRect : public Hittable
{
public:
    XZRect() {}

    XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
        shared_ptr<Material> mat) : x0{ _x0 }, x1{ _x1 }, z0{ _z0 }, z1{ _z1 }, k{ _k }, mp{ mat } {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = AABB(Point3D(x0, k - 0.0001, z0), Point3D(x1, k + 0.0001, z1));
        return true;
    }
    shared_ptr<Material> mp;
    double x0 = 0.0f;
    double x1 = 0.0f;
    double z0 = 0.0f;
    double z1 = 0.0f;
    double k = 0.0f;
};

class YZRect : public Hittable
{
public:
    YZRect() {}

    YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
        shared_ptr<Material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the X
        // dimension a small amount.
        output_box = AABB(Point3D(k - 0.0001, y0, z0), Point3D(k + 0.0001, y1, z1));
        return true;
    }
    shared_ptr<Material> mp;
    double z0 = 0.0f;
    double z1 = 0.0f;
    double y0 = 0.0f;
    double y1 = 0.0f;
    double k = 0.0f;
};

#endif