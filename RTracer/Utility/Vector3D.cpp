#include "Vector3D.h"

double Vector3D::length_squared() const
{
	return coordinates[0] * coordinates[0] + coordinates[1] * coordinates[1] + coordinates[2] * coordinates[2];
}

bool Vector3D::near_zero() const
{
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(coordinates[0]) < s) && (fabs(coordinates[1]) < s) && (fabs(coordinates[2]) < s);
}

Vector3D random_in_unit_sphere()
{
    while (true)
    {
        Vector3D p = Vector3D::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vector3D random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

Vector3D random_in_hemisphere(const Vector3D& normal)
{
    Vector3D in_unit_sphere = random_in_unit_sphere();

    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vector3D reflect(const Vector3D& v, const Vector3D& n)
{
    return v - 2 * dot(v, n) * n;
}

Vector3D refract(const Vector3D& uv, const Vector3D& n, double etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3D r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3D r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

Vector3D random_in_unit_disk()
{
    while (true)
    {
        Vector3D p = Vector3D(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
