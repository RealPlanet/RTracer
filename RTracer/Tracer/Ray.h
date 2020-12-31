#ifndef RAY_H
#define RAY_H

#include <cstdlib>
#include "Vector3D.h"
#include "VMath.h"

class Material;
class Ray
{
public:
	Ray() {}
	Ray(const Point3D& point, const Vector3D& vec) : _origin{ point }, _direction{ vec }{}

	Point3D origin() const { return _origin; }
	Vector3D direction() const { return _direction; }

	Point3D at(double t) const
	{
		return _origin + t * _direction;
	}

	Point3D _origin;
	Vector3D _direction;
};

struct HitInfo
{
	Point3D point = {};
	Vector3D normal = {};
	shared_ptr<Material> mat_ptr;
	double t = 0.0f;
	bool front_face = false;

	inline void set_face_normal(const Ray& r, const Point3D& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

#endif // !RAY_H