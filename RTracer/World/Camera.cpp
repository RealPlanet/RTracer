#include "Camera.h"

Camera::Camera(Point3D lookfrom, Point3D lookat, Vector3D vup, double vertical_fov, double aspect_ratio, double aperture, double focus_dist)
{
	double theta = degrees_to_radians(vertical_fov);
	double height = tan(theta / 2);
	double viewport_height = 2.0 * height;
	double viewport_width = aspect_ratio * viewport_height;

    // Camera Plane
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2;
}

Ray Camera::get_ray(double s, double t) const
{
	Vector3D rd = lens_radius * random_in_unit_disk();
	Vector3D offset = u * rd.x() + v * rd.y();

	return Ray( origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset );
}
