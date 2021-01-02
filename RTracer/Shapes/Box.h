#ifndef BOX_H
#define BOX_H

#include "VMath.h"
#include "Rect.h"
#include "HittableList.h"

class Box : public Hittable
{
public:
	Box() {}
	Box(const Point3D& p0, const Point3D& p1, shared_ptr<Material> ptr);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override
    {
        output_box = AABB(box_min, box_max);
        return true;
    }

public:
    Point3D box_min;
    Point3D box_max;
    HittableList sides;
};

#endif
