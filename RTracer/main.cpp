
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
#include "Scenes/Scene.h"
#include "Scenes/RandomScene.h"
#include "Scenes/TwoSphereScene.h"
#include "Scenes/TwoPerlinSphereScene.h"
#include "Scenes/CornellSmokeScene.h"
#include "Scenes/ClusterScene.h"
#include "Scenes/CornellBoxScene.h"
#include "Scenes/SimpleLightScene.h"
#include "Scenes/EarthScene.h"

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

int main() {

    // Image
    const int max_depth = 10;
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    Scene current_scene;

    switch (2)
    {
    case 1:
        current_scene = RandomScene();
        break; 
    case 2:
        current_scene = TwoSphereScene();
        break;
    case 3:
        current_scene = TwoPerlinSphereScene();
        break;
    case 4:
        current_scene = EarthScene();   
        break;
    case 5:
        current_scene = SimpleLightScene(); 
        break;
    case 6:
        current_scene = CornellBoxScene();
        aspect_ratio = 1.0;
        image_width = 600;
        break;
    case 7:
        current_scene = CornellSmokeScene();
        aspect_ratio = 1.0;
        image_width = 1024;  
        break;
    default:
    case 8:
        current_scene = ClusterScene();  
        aspect_ratio = 1.0;
        image_width = 212;   
        break;
    }

    // Camera

    Vector3D vup(0, 1, 0);
    double dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    Camera scene_camera(current_scene.lookfrom, current_scene.lookat, vup, current_scene.vfov, aspect_ratio, current_scene.aperture, dist_to_focus, 0.0, 1.0);

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
            for (int s = 0; s < current_scene.samples_per_pixel; ++s)
            {
                //std::cout << s << std::flush;
                double u = (i + random_double()) / ((double)image_width - 1);
                double v = (j + random_double()) / ((double)image_height - 1);
                Ray r = scene_camera.get_ray(u, v);
                //pixel_color += ray_color(r, world, max_depth);
                pixel_color += ray_color(r, current_scene.background, current_scene.world, max_depth);
            }
            image_buffer.write_color(pixel_color, current_scene.samples_per_pixel);
        }
    }
    image_file << image_buffer.get_buffer_string();
    image_file.close();
    std::cerr << "\nDone.\n";
}