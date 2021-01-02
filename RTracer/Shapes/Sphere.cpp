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
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool Sphere::bounding_box(double time0, double time1, AABB& output_box) const
{
    output_box = AABB( center - Vector3D(radius, radius, radius), center + Vector3D(radius, radius, radius));
    return true;
}

void Sphere::get_sphere_uv(const Point3D& p, double& u, double& v)
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
