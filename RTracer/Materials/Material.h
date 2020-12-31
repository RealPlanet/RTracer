#ifndef MATERIAL_H
#define MATERIAL_H

#include "VMath.h"
#include "Ray.h"

struct HitInfo;

class Material
{
public:
    virtual bool scatter( const Ray& r_in, const HitInfo& rec, Color& attenuation, Ray& scattered ) const = 0;
};

#endif
