#ifndef RAY_H
#define RAY_H

#include "Vector3D.h"

class Ray
{
public:
	
	Ray() {}
	Ray(const Point3D& _origin, const Vector3D& _direction, double _time = 0.0f) : m_origin{ _origin }, m_direction{ _direction }, m_time{ _time }
	{}

	Point3D origin() const { return m_origin; }
	Vector3D direction() const { return m_direction; }
	double time() const { return m_time; }

	Point3D at(double t) const
	{
		return m_origin + t * m_direction;
	}

	Point3D m_origin;
	Vector3D m_direction;
	double m_time = 0.0f;
};

#endif // !RAY_H