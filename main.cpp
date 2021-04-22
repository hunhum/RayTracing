#include <iostream>
#include"octotree.h"
#include"sphere.h"
#include"meshshape.h"
#include"scene.h"
#include<chrono>
using namespace std;

int main()
{
    /*Box qq = {{0,0,0},{1,1,1}};
    Triangle t1 = {{0.40131,-2.0732,0.78041},{0.53901,0.37444,1.2217},{-0.5,0.37444,1.2149}};
    Triangle t2 = {{1,2,3.8},{1,2,58.8},{56,2,3.8}};
    if(qq.contains_triangle(t1)){
        cout << "Hello World!" << endl;
    }
    if(qq.contains_triangle_fast(t1)){
        cout << "Hello Fast World!" << endl;
    }
    OctTree octtree({{64,64,64},{64,64,64}});
    octtree.insert(t1);
    octtree.insert(t2);
    Ray ray = {{0,0,0},{1,1,0.9}};
    std::vector<Triangle> tmp;
    tmp = octtree.intersections_with_ray(ray);
    for(auto i : tmp){
        for(int j = 0; j < 3; j++){
            cout << i[j].x << " " << i[j].y << " " << i[j].z << ", ";
        }
        cout << std::endl;
    }*/
    ifstream file("toyota-supra.obj");
    MeshShape monkey({3.0,0.0,0.0},0,-0.7,3.14/2,file);
    //Sphere s1({3.0,0.0,0.0},0.5);
    //Sphere s2({6.0,-3.0,0.0},0.5);
    Scene scene({0,0,0},{1,1,2},{1,0,0},{0,0,1},1.0,10.0,0.785,1920,1080);
    //scene.add_object(&s1);
    //scene.add_object(&s2);
    scene.add_object(&monkey);
    scene.render("qqwe");
    return 0;
}
