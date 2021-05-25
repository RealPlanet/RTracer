#pragma once
#include "Scene.h"
class CornellBoxScene :
    public Scene
{
public:
    CornellBoxScene()
    {
        world = cornell_box();
        samples_per_pixel = 100;
        lookfrom = Point3D(278, 278, -800);
        lookat = Point3D(278, 278, 0);
        vfov = 40.0;
    }
private:
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

};

