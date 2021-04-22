#ifndef MESHSHAPE_H
#define MESHSHAPE_H
#include"shape.h"
#include"triangle.h"
#include"octotree.h"

class MeshShape:public Shape
{
    Point centre;
    double psi;
    double fi;
    double teta;
    std::vector<Triangle> triangles;
    toctt::TriangleOctTree *octtree;
public:
    MeshShape() = delete;
    MeshShape(const std::vector<Triangle>&);
    MeshShape(Point centre, double psi, double fi, double teta,const std::vector<Triangle>&);
    MeshShape(std::ifstream &file);
    MeshShape(Point centre, double psi, double fi, double teta,std::ifstream &file);
    ~MeshShape();
    void make_octtree();
    XYZ coord_transform_to_local(XYZ);
    XYZ coord_transform_to_global(XYZ);
    bool intersections_with_ray(Ray ray, double *t, Vector *normal) override;
};

#endif // MESHSHAPE_H
