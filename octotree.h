#ifndef OCTOTREE_H
#define OCTOTREE_H
#include "base.h"
#include"triangle.h"
#include<set>
namespace  toctt{
    struct Box{
        Point middle;
        XYZ size;
        bool intersect(Ray, double *t_near, double *t_far);
        bool contains_point(Point);
        bool contains_triangle(Triangle);
        bool contains_triangle_fast(Triangle);
        bool contains_box(Box);
    };

    class TriangleOctTree
    {
        TriangleOctTree* child[8];
        std::vector<Triangle> triangles;
        std::vector<int> ptrs_triangles;//stores the triangle number in vector
        const double min_size = 0.00125;
        Box boundary;
    public:
        TriangleOctTree() = delete;
        TriangleOctTree(Box);
        TriangleOctTree(Box,const std::vector<Triangle>&);
        TriangleOctTree(const TriangleOctTree&) = delete;
        TriangleOctTree(TriangleOctTree&&) = delete;
        ~TriangleOctTree();
        TriangleOctTree& operator=(const TriangleOctTree&) = delete;
        TriangleOctTree& operator=(TriangleOctTree&&) = delete;
        void insert(Triangle);
        void buildtree(const std::vector<Triangle>&);
        void deletetree(TriangleOctTree*);
        bool intersections_with_ray(Ray,  double *t, Triangle *triangle);
    };
}

#endif // OCTOTREE_H
