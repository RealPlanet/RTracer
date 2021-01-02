
#include <iostream>
#include <fstream>
#include <sstream>

#include "VMath.h"
#include "Box.h"
#include "BVHNode.h"
#include "Camera.h"
#include "ImageOutput.h"
#include "ConstantMedium.h"
#include "HittableList.h"
#include "Material.h"
#include "MovingSphere.h"
#include "Sphere.h"
#include "Texture.h"
#include "Ray.h"

using namespace std;

Color ray_color(const Ray& r, const Color& background, const HittableList& world, int depth)
{
    if (depth <= 0)
    {
        return Color(0, 0, 0);
    }

    HitInfo rec;

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
    {
        return background;
    }
      
    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.point);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    {
        return emitted;
    }
        
    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

HittableList random_scene()
{
    HittableList world;

    auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3D(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_double();
            Point3D center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3D(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vector3D(0, random_double(0, .5), 0);
                    world.add(make_shared<MovingSphere>( center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // Metal
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

HittableList two_spheres() {
    HittableList objects;

    auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Point3D(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    objects.add(make_shared<Sphere>(Point3D(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    return objects;
}

HittableList two_perlin_spheres()
{
    HittableList objects;

    auto pertext = make_shared<NoiseTexture>(4);
    objects.add(make_shared<Sphere>(Point3D(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    objects.add(make_shared<Sphere>(Point3D(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    return objects;
}

HittableList earth()
{
    auto earth_texture = make_shared<ImageTexture>("_images/earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(Point3D(0, 0, 0), 2, earth_surface);

    return HittableList(globe);
}

HittableList simple_light()
{
    HittableList objects;

    auto pertext = make_shared<NoiseTexture>(4);
    objects.add(make_shared<Sphere>(Point3D(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    objects.add(make_shared<Sphere>(Point3D(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    objects.add(make_shared<XYRect>(3, 5, 1, 3, -2, difflight));
    objects.add(make_shared<Sphere>(Point3D(0, 5, 0), 2, difflight));

    return objects;
}

HittableList cornell_box()
{
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3D(265, 0, 295));
    objects.add(box1);

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3D(130, 0, 65));
    objects.add(box2);

    return objects;
}

HittableList cornell_smoke()
{
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3D(265, 0, 295));

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3D(0, 0, 0), Point3D(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3D(130, 0, 65));

    objects.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    objects.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    return objects;
}

HittableList final_scene()
{
    HittableList objects;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            objects.add(make_shared<Box>(Point3D(x0, y0, z0), Point3D(x1, y1, z1), ground));
        }
    }

    objects.add(make_shared<BVHNode>(objects, 0, 1));

    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

    auto center1 = Point3D(400, 400, 200);
    auto center2 = center1 + Vector3D(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<Sphere>(Point3D(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    objects.add(make_shared<Sphere>(
        Point3D(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
        ));

    auto boundary = make_shared<Sphere>(Point3D(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3D(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("_images/earthmap.jpg"));
    objects.add(make_shared<Sphere>(Point3D(400, 200, 400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.1);
    objects.add(make_shared<Sphere>(Point3D(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        objects.add(make_shared<Sphere>(Point3D::random(0, 165), 10, white));
    }

    objects.add(make_shared<Translate>( make_shared<RotateY>( make_shared<BVHNode>(objects, 0.0, 1.0), 15), Vector3D(-100, 270, 395) ) );

    return objects;
}

int main() {

    // Image
    const int max_depth = 10;
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 250;
    int samples_per_pixel = 100;

    HittableList world;
    Point3D lookfrom;
    Point3D lookat;
    double vfov = 40.0;
    double aperture = 0.0;
    Color background(0, 0, 0);

    switch (0)
    {
    case 1:
        {
        world = random_scene();
        background = Color(0.70, 0.80, 1.00);
        lookfrom = Point3D(13, 2, 3);
        lookat = Point3D(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break; 
        }
    case 2:
        world = two_spheres();
        background = Color(0.70, 0.80, 1.00);
        lookfrom = Point3D(13, 2, 3);
        lookat = Point3D(0, 0, 0);
        vfov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        background = Color(0.70, 0.80, 1.00);
        lookfrom = Point3D(13, 2, 3);
        lookat = Point3D(0, 0, 0);
        vfov = 20.0;
        break;
    case 4:
        world = earth();
        background = Color(0.70, 0.80, 1.00);
        lookfrom = Point3D(13, 2, 3);
        lookat = Point3D(0, 0, 0);
        vfov = 20.0;
        break;
    case 5:
        world = simple_light();
        samples_per_pixel = 400;
        background = Color(0, 0, 0);
        lookfrom = Point3D(26, 3, 6);
        lookat = Point3D(0, 2, 0);
        vfov = 20.0;
        break;
    case 6:
        world = cornell_box();
        aspect_ratio = 1.0;
        image_width = 600;
        samples_per_pixel = 100;
        lookfrom = Point3D(278, 278, -800);
        lookat = Point3D(278, 278, 0);
        vfov = 40.0;
        break;
    case 7:
        world = cornell_smoke();
        aspect_ratio = 1.0;
        image_width = 1024;
        samples_per_pixel = 100;
        lookfrom = Point3D(278, 278, -800);
        lookat = Point3D(278, 278, 0);
        vfov = 40.0;
        break;
    default:
    case 8:
        world = final_scene();
        aspect_ratio = 1.0;
        image_width = 800;
        samples_per_pixel = 100;
        background = Color(0, 0, 0);
        lookfrom = Point3D(478, 278, -600);
        lookat = Point3D(278, 278, 0);
        vfov = 40.0;
        break;
    }

    // Camera

    Vector3D vup(0, 1, 0);
    double dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    Camera scene_camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

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
                //pixel_color += ray_color(r, world, max_depth);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            image_buffer.write_color(pixel_color, samples_per_pixel);
        }
    }
    image_file << image_buffer.get_buffer_string();
    image_file.close();
    std::cerr << "\nDone.\n";
}