#ifndef MESHSHAPE_H
#define MESHSHAPE_H
#include"base.h"
#include"shape.h"
#include"triangle.h"
#include"octotree.h"

class MeshShape:public Shape
{
    double psi;
    double fi;
    double teta;
    std::vector<Triangle> triangles;
    toctt::TriangleOctTree *octtree;
public:
    MeshShape() = delete;
    MeshShape(const std::vector<Triangle>&,Point centre = {0,0,0}, double psi = 0.0, double fi = 0.0, double teta = 0.0, double min_size = MIN_SIZE);
    MeshShape(std::ifstream &file,Point centre = {0,0,0}, double psi = 0.0, double fi = 0.0, double teta = 0.0,  double min_size =  MIN_SIZE);
    ~MeshShape();
    void make_octtree(double min_size =  MIN_SIZE);
    XYZ coord_transform_to_local(XYZ);
    XYZ coord_transform_to_global(XYZ);
    bool intersections_with_ray(Ray ray, double *t, Vector *normal) override;
};

#endif // MESHSHAPE_H
