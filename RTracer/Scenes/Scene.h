#pragma once

#include <Vector3D.h>
#include <HittableList.h>
#include <Texture.h>
#include <Material.h>
#include <Sphere.h>
#include <Rect.h>

class Scene
{
public:
	Color background = Color(0, 0, 0);
	Point3D lookfrom = Point3D(0, 0, 0);
	Point3D lookat = Point3D(0, 0, 0);
	double vfov = 40.0;
	double aperture = 0.0;
	HittableList world = HittableList();
	int samples_per_pixel = 400;

	Scene()
	{
	}
};

