#ifndef HITTABLE_H
#define HITTABLE_H
#include "VMath.h"
#include "Ray.h"
#include "AABB.h"

class Material;
struct HitInfo
{
	Point3D point = {};
	Vector3D normal = {};
	shared_ptr<Material> mat_ptr;
	double t = 0.0f;
	bool front_face = false;
	// Surface coordinates
	double u = 0.0f;
	double v = 0.0f;

	inline void set_face_normal(const Ray& r, const Point3D& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

/*
	Abstract class which is derived for all objects that can be hit with a ray
*/
class Hittable
{
public:
	/*
		This function is implemented on derived objects and calculates the hit behaviour for the ray
		output in rec
	*/
	virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const = 0;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const = 0;
};

class RotateY : public Hittable
{
public:
    RotateY(shared_ptr<Hittable> p, double angle);
    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override
    {
        output_box = bbox;
        return hasbox;
    }

    shared_ptr<Hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    AABB bbox;
};

class Translate : public Hittable
{
public:
    Translate(shared_ptr<Hittable> p, const Vector3D& displacement) : ptr(p), offset(displacement) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;

public:
    shared_ptr<Hittable> ptr;
    Vector3D offset;
};
#endif
