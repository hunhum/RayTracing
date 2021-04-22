#ifndef SHAPE_H
#define SHAPE_H
#include "base.h"

class Shape
{
private:
    unsigned char color;
public:
    Shape() = default;
    virtual ~Shape(){};
    virtual bool intersections_with_ray(Ray ray, double *t, Vector *normal) = 0;
};

#endif // SHAPE_H
