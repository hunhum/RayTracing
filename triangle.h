#ifndef TRIANGLE_H
#define TRIANGLE_H
#include"base.h"

struct Triangle
{
public:
    Point p0;
    Point p1;
    Point p2;
    Triangle() = default;
    Triangle(Point p0, Point p1, Point p2);
    Point& operator[](int i);
    bool intersect(Ray ray, double *t);
    bool intersect(Ray ray, double *t, double *u, double *v);
};

#endif // TRIANGLE_H
