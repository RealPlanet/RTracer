#ifndef VECTOR3D_H
#define VECTOR3D_H
#include <cmath>
#include <iostream>
#include "VMath.h"

using std::sqrt;

class Vector3D
{
public:
    Vector3D() : coordinates{ 0,0,0 } {}
    Vector3D(double e0, double e1, double e2) : coordinates{ e0, e1, e2 } {}

    double x() const { return coordinates[0]; }
    double y() const { return coordinates[1]; }
    double z() const { return coordinates[2]; }

    Vector3D operator-() const { return Vector3D(-coordinates[0], -coordinates[1], -coordinates[2]); }
    double operator[](int i) const { return coordinates[i]; }
    double& operator[](int i) { return coordinates[i]; }

    inline double length() const { return sqrt(length_squared()); }

    inline static Vector3D random()
    {
        return Vector3D(random_double(), random_double(), random_double());
    }

    inline static Vector3D random(double min, double max)
    {
        return Vector3D(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    double length_squared() const;
    bool near_zero() const;

    Vector3D& operator+=(const Vector3D& v) {
        coordinates[0] += v.coordinates[0];
        coordinates[1] += v.coordinates[1];
        coordinates[2] += v.coordinates[2];
        return *this;
    }

    Vector3D& operator*=(const double t) {
        coordinates[0] *= t;
        coordinates[1] *= t;
        coordinates[2] *= t;
        return *this;
    }

    Vector3D& operator/=(const double t) {
        return *this *= 1 / t;
    }

public:
    double coordinates[3];
};

// Vector3D Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vector3D& v)
{
    return out << v.coordinates[0] << ' ' << v.coordinates[1] << ' ' << v.coordinates[2];
}

inline Vector3D operator+(const Vector3D& u, const Vector3D& v)
{
    return Vector3D(u.coordinates[0] + v.coordinates[0], u.coordinates[1] + v.coordinates[1], u.coordinates[2] + v.coordinates[2]);
}

inline Vector3D operator-(const Vector3D& u, const Vector3D& v)
{
    return Vector3D(u.coordinates[0] - v.coordinates[0], u.coordinates[1] - v.coordinates[1], u.coordinates[2] - v.coordinates[2]);
}

inline Vector3D operator*(const Vector3D& u, const Vector3D& v)
{
    return Vector3D(u.coordinates[0] * v.coordinates[0], u.coordinates[1] * v.coordinates[1], u.coordinates[2] * v.coordinates[2]);
}

inline Vector3D operator*(double t, const Vector3D& v)
{
    return Vector3D(t * v.coordinates[0], t * v.coordinates[1], t * v.coordinates[2]);
}

inline Vector3D operator*(const Vector3D& v, double t)
{
    return t * v;
}

inline Vector3D operator/(Vector3D v, double t)
{
    return (1 / t) * v;
}

inline double dot(const Vector3D& u, const Vector3D& v)
{
    return u.coordinates[0] * v.coordinates[0]
        + u.coordinates[1] * v.coordinates[1]
        + u.coordinates[2] * v.coordinates[2];
}

inline Vector3D cross(const Vector3D& u, const Vector3D& v)
{
    return Vector3D(u.coordinates[1] * v.coordinates[2] - u.coordinates[2] * v.coordinates[1],
        u.coordinates[2] * v.coordinates[0] - u.coordinates[0] * v.coordinates[2],
        u.coordinates[0] * v.coordinates[1] - u.coordinates[1] * v.coordinates[0]);
}

inline Vector3D unit_vector(Vector3D v)
{
    return v / v.length();
}

// Generate a sphere of radius 1 then generate a Vector, if the vector is inside of the sphere return it
Vector3D random_in_unit_sphere();
Vector3D random_unit_vector();
Vector3D random_in_hemisphere(const Vector3D& normal);
Vector3D reflect(const Vector3D& v, const Vector3D& n);
Vector3D refract(const Vector3D& uv, const Vector3D& n, double etai_over_etat);
Vector3D random_in_unit_disk();
// Type aliases for Vector3D
using Point3D = Vector3D;   // 3D point
using Color = Vector3D;    // RGB color

#endif