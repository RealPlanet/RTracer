#ifndef AABB_H
#define AABB_H
#include "VMath.h"
#include "Ray.h"

class AABB
{
public:
	AABB() {}
	AABB(const Point3D& a, const Point3D& b) { minimum = a; maximum = b; }
	Point3D min() const { return minimum; }
	Point3D max() const { return maximum; }
	bool hit(const Ray& r, double t_min, double t_max) const;

	Point3D minimum;
	Point3D maximum;
};

inline AABB surrounding_box(AABB box0, AABB box1)
{
	Point3D small(fmin(box0.min().x(), box1.min().x()), fmin(box0.min().y(), box1.min().y()), fmin(box0.min().z(), box1.min().z()));
	Point3D big(fmax(box0.max().x(), box1.max().x()), fmax(box0.max().y(), box1.max().y()), fmax(box0.max().z(), box1.max().z()));

	return AABB(small, big);
}


#endif