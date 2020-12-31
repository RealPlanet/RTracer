#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3D.h"
#include "Ray.h"

class Camera
{
public:
	Camera(Point3D lookfrom, Point3D lookat, Vector3D vup, double vertical_fov, double aspect_ratio, double aperture, double focus_dist);

	Ray get_ray(double s, double t) const;
	

private:
	Point3D origin;
	Vector3D horizontal;
	Vector3D vertical;
	Vector3D lower_left_corner;
	Vector3D u, v, w;
	double lens_radius;
};	

#endif
