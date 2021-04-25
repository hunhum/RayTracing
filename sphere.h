#ifndef SPHERE_H
#define SPHERE_H
#include"shape.h"

class Sphere:public Shape
{
private:
    double r;
public:
    Sphere() = delete;
    Sphere(Point,double);
    bool intersections_with_ray(Ray ray, double *t, Vector *normal, double dist, double vis_limit) override;
};

#endif // SPHERE_H
