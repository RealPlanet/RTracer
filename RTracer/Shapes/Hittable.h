#ifndef HITTABLE_H
#define HITTABLE_H
#include "Ray.h"


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
};

#endif
