#include <iostream>
#include"octotree.h"
#include"sphere.h"
#include"meshshape.h"
#include"scene.h"
#include<chrono>
using namespace std;

int main()
{
    ifstream file("toyota-supra.obj");
    MeshShape supra(file,{3.0,0.0,0.0},0,-0.7,3.14/2);
    //Sphere s1({3.0,0.0,0.0},0.5);
    Sphere s2({6.0,3.0,0.0},0.5);
    Scene scene({0,0,0},{1,1,2},{1,0,0},{0,0,1},1.0,10.0,0.785,1920,1080);
    //scene.add_object(&s1);
    scene.add_object(&s2);
    scene.add_object(&supra);
    scene.render("qqwe");
    return 0;
}
