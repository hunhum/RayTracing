#include "sphere.h"

Sphere::Sphere(Point centre, double r):Shape(centre),r(r){}
bool Sphere::intersections_with_ray(Ray ray, double *t, Vector *normal){
    double a = ray.direction*ray.direction;
    double b = ray.direction*(ray.begin-centre);
    double c = (ray.begin-centre)*(ray.begin-centre)-r*r;
    double d = b*b-a*c;
    double t1,t2;
    if(d < 0){
        return false;
    }
    t1 = (-b-std::sqrt(d))/a;
    t2 = (-b+std::sqrt(d))/a;
    *t = (t1 < t2 ? t1 : t2);
    *normal = ray.begin+ray.direction*(*t)-centre;
    *normal = (*normal)*(1/normal->lenght());
    if(ray.direction*(*normal) > 0){
        (*normal) = (*normal)*(-1);
    }
    return true;
}
