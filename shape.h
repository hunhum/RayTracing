#ifndef SHAPE_H
#define SHAPE_H
#include "base.h"

struct Shape
{
    Point centre;
    unsigned char color;
    double Ka = 0.1;
    double Kd = 0.8;
    double Ks = 0.7;
    double shinines = 20;

    Shape() = default;
    Shape(Point centre):centre(centre){}
    virtual ~Shape(){};
    virtual bool intersections_with_ray(Ray ray, double *t, Vector *normal) = 0;
};

#endif // SHAPE_H
