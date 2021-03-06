#ifndef SCENE_H
#define SCENE_H
#include"base.h"
#include"shape.h"
#include"CImg.h"
#include<omp.h>

class Scene
{
private:
    Point cam;
    Point light;
    Vector screen_normal;
    Vector up;
    double dist;
    double vis_limit;
    double alpha;
    double Ka = 0.1;
    double Kd = 0.8;
    double Ks = 0.7;
    unsigned char light_color = 128;
    int screen_res[2];
    std::vector<Shape*> objects;
public:
    Scene() = delete;
    Scene(Point cam, Point light, Vector screen_normal, Vector up, double dist, double vis_limit, double alpha, int w, int h);
    void render(const char *filename);
    void add_object(Shape*);
};

#endif // SCENE_H
