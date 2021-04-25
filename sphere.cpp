#include "sphere.h"

Sphere::Sphere(Point centre, double r):Shape(centre),r(r){}
bool Sphere::intersections_with_ray(Ray ray, double *t, Vector *normal, double dist, double vis_limit){
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
    if(t1 > t2){
        std::swap(t1,t2);
    }
    if(t1 > vis_limit){
        return false;
    }
    if(t2 < dist){
        return false;
    }
    if(t1 < dist){
        t1 = t2;
    }
    *t = t1;
    *normal = ray.begin+ray.direction*(*t)-centre;
    *normal = (*normal)*(1/normal->lenght());
    if(ray.direction*(*normal) > 0){
        (*normal) = (*normal)*(-1);
    }
    return true;
}
