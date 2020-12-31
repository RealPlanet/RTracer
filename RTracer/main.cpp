#include <iostream>
#include <fstream>
#include <sstream>

#include "Vector3D.h"
#include "ImageOutput.h"
#include "Ray.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Camera.h"
#include "Sphere.h"
#include "Material.h"
#include "Metal.h"
#include "Lambertian.h"
#include "VMath.h"
#include "Dielectric.h"

using namespace std;

Color ray_color(const Ray& r, const Hittable& world, int depth)
{
    if (depth <= 0)
    {
        return Color(0, 0, 0);
    }

    HitInfo rec;

    if (world.hit(r, 0.001f, infinity, rec))
    {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return Color(0, 0, 0);
    }

    Vector3D unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

HittableList random_scene()
{
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3D(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            Point3D center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3D(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3D(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3D(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3D(4, 1, 0), 1.0, material3));

    return world;
}

int main() {

    // Image
    const double aspect_ratio = 3.0 / 2.0;
    const int image_width = 1240;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 250;
    const int max_depth = 8;

    // World
    HittableList world = random_scene();

    double randomX = random_double();
    double randomY = random_double();
    double randomZ = random_double();
    Point3D lookfrom(20, 10, 10);
    Point3D lookat(0, 0, 0);
    Vector3D vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera scene_camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    ofstream image_file = {};
    ImageOut image_buffer = {};
    image_file.open("render_image.ppm", ios::binary);
    image_file.clear();

    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                //std::cout << s << std::flush;
                double u = (i + random_double()) / ((double)image_width - 1);
                double v = (j + random_double()) / ((double)image_height - 1);
                Ray r = scene_camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            image_buffer.write_color(pixel_color, samples_per_pixel);
        }
    }
    image_file << image_buffer.get_buffer_string();
    image_file.close();
    std::cerr << "\nDone.\n";
}