#pragma once

#include <Vector3.h>
#include <HittableList.h>
#include <Texture.h>
#include <Material.h>
#include <Sphere.h>
#include <Rect.h>

class Scene
{
public:
	Color background = Color(0, 0, 0);
	Point3 lookfrom = Point3(0, 0, 0);
	Point3 lookat = Point3(0, 0, 0);
	double vfov = 40.0;
	double aperture = 0.0;
	HittableList world = HittableList();
	int samples_per_pixel = 400;
	int max_depth = 10;
	Scene()
	{
	}
};

