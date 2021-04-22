#include "triangle.h"

Triangle::Triangle(Point p0, Point p1, Point p2):p0(p0),p1(p1),p2(p2){}
Point& Triangle::operator[](int i){
    switch (i){
    case 0:
        return p0;
        break;
    case 1:
        return p1;
        break;
    case 2:
        return p2;
        break;
    default:
        throw std::out_of_range("ЭЭЭЭЭ");
        break;
    }
}
bool  Triangle::intersect(Ray ray, double *t){
    //b = ray.begin; d = ray.direction;
    //b+t*d = (1-u-v)*p0+u*p1+v*p2;
    //t*d+u(p0-p1)+v(p0-p2) = p0-b;
    //          (d.x  (p0-p1).x  (p0-p2).x)   (t)
    // (p0-b) = (d.y  (p0-p1).y  (p0-p2).y) * (u)
    //          (d.z  (p0-p1).z  (p0-p2).z)   (v)
    //
    double u,v,det,dett,detu,detv;
    XYZ d = ray.direction;
    XYZ b = ray.begin;
    det = d.x*(p0-p1).y*(p0-p2).z+(p0-p1).x*(p0-p2).y*d.z+(p0-p2).x*d.y*(p0-p1).z-(p0-p2).x*(p0-p1).y*d.z-(p0-p1).x*d.y*(p0-p2).z-d.x*(p0-p2).y*(p0-p1).z;
    dett = (p0-b).x*(p0-p1).y*(p0-p2).z+(p0-p1).x*(p0-p2).y*(p0-b).z+(p0-p2).x*(p0-b).y*(p0-p1).z-(p0-p2).x*(p0-p1).y*(p0-b).z-(p0-p1).x*(p0-b).y*(p0-p2).z-(p0-b).x*(p0-p2).y*(p0-p1).z;
    detu = d.x*(p0-b).y*(p0-p2).z+(p0-b).x*(p0-p2).y*d.z+(p0-p2).x*d.y*(p0-b).z-(p0-p2).x*(p0-b).y*d.z-(p0-b).x*d.y*(p0-p2).z-d.x*(p0-p2).y*(p0-b).z;
    detv = d.x*(p0-p1).y*(p0-b).z+(p0-p1).x*(p0-b).y*d.z+(p0-b).x*d.y*(p0-p1).z-(p0-b).x*(p0-p1).y*d.z-(p0-p1).x*d.y*(p0-b).z-d.x*(p0-b).y*(p0-p1).z;
    if(almost_equal_zero(det)){
        return false;
    }
    u = detu/det;
    if(u < 0.0 || u > 1.0){
        return false;
    }
    v = detv/det;
    if(v < 0.0 || u+v > 1.0){
        return false;
    }
    *t = dett/det;
    return true;
}
bool  Triangle::intersect(Ray ray, double *t, double *u, double *v){
     double det,dett,detu,detv;
     XYZ d = ray.direction;
     XYZ b = ray.begin;
     det = d.x*(p0-p1).y*(p0-p2).z+(p0-p1).x*(p0-p2).y*d.z+(p0-p2).x*d.y*(p0-p1).z-(p0-p2).x*(p0-p1).y*d.z-(p0-p1).x*d.y*(p0-p2).z-d.x*(p0-p2).y*(p0-p1).z;
     dett = (p0-b).x*(p0-p1).y*(p0-p2).z+(p0-p1).x*(p0-p2).y*(p0-b).z+(p0-p2).x*(p0-b).y*(p0-p1).z-(p0-p2).x*(p0-p1).y*(p0-b).z-(p0-p1).x*(p0-b).y*(p0-p2).z-(p0-b).x*(p0-p2).y*(p0-p1).z;
     detu = d.x*(p0-b).y*(p0-p2).z+(p0-b).x*(p0-p2).y*d.z+(p0-p2).x*d.y*(p0-b).z-(p0-p2).x*(p0-b).y*d.z-(p0-b).x*d.y*(p0-p2).z-d.x*(p0-p2).y*(p0-b).z;
     detv = d.x*(p0-p1).y*(p0-b).z+(p0-p1).x*(p0-b).y*d.z+(p0-b).x*d.y*(p0-p1).z-(p0-b).x*(p0-p1).y*d.z-(p0-p1).x*d.y*(p0-b).z-d.x*(p0-b).y*(p0-p1).z;
     if(almost_equal_zero(det)){
         return false;
     }
     *u = detu/det;
     *v = detv/det;
     *t = dett/det;
     if(*u < 0.0 || *v < 0.0 || *u+*v > 1.0){
         return false;
     }
     return true;
 }
