#ifndef SCENE_H
#define SCENE_H
#include"base.h"
#include"shape.h"
#include"CImg.h"

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
    int screen_res[2];
    std::vector<Shape*> objects;
public:
    Scene() = delete;
    Scene(Point cam, Point light, Vector screen_normal, Vector up, double dist, double vis_limit, double alpha, int w, int h);
    void render(const char *filename);
    void add_object(Shape*);
};

#endif // SCENE_H
