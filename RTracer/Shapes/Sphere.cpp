#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitInfo& rec) const
{
    /*
       Calculate numbers to solve this quadratic equation
       (t^2)n*n+2tn*(A−C)+(A−C)*(A−C)−r^2=0
       discriminant = (b2−4ac)
       t == constant that raprsents the current position on the ray [0, 1]
       r == sphere radius
       A == vector ray origin
       C == vector sphere origin
       n == vector ray direction

       a = n * n
       b = 2 * n * (A - C)
       c = (A-C)^2 - r^2
    */
    Vector3D oc_vec = r.origin() - center; //Vector that passes the ray origin and the sphere center
    double a = r.direction().length_squared(); // dot == squared lenght
    double half_b = dot(oc_vec, r.direction()); // b == 2h => simplifies the equation
    double c = oc_vec.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;

    double sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    Vector3D outward_normal = (rec.point - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}
