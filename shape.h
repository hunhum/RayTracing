#ifndef SHAPE_H
#define SHAPE_H
#include "base.h"

class Shape
{
protected:
    Point centre;
    unsigned char color;
public:
    Shape() = default;
    Shape(Point centre):centre(centre){}
    virtual ~Shape(){};
    virtual bool intersections_with_ray(Ray ray, double *t, Vector *normal) = 0;
    unsigned char get_color(){
        return color;
    }
    void set_color(unsigned char c){
        color = c;
    }
    Point get_centre(){
        return centre;
    }
};

#endif // SHAPE_H
