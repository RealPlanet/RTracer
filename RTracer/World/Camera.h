#ifndef CAMERA_H
#define CAMERA_H

#include "VMath.h"
#include "Ray.h"

class Camera
{
public:
	Camera(Point3D lookfrom, Point3D lookat, Vector3D vup, double vertical_fov, double aspect_ratio, double aperture, double focus_dist,
		double _time0 = 0, double _time1 = 0);

	Ray get_ray(double s, double t) const;
	

private:
	Point3D origin;
	Point3D lower_left_corner;
	Vector3D horizontal;
	Vector3D vertical;
	Vector3D u, v, w;
	double lens_radius;
	double time0, time1;  // shutter open/close times
};	

#endif
