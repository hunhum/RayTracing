#include <iostream>
#include <fstream>
#include <string>
#include"octotree.h"
#include"sphere.h"
#include"meshshape.h"
#include"scene.h"
#include<chrono>
using namespace std;

int main()
{
    ifstream file("toyota-supra.obj");
    std::string str;
    int n,i = 0;
    char *buff = new char[1024];
    Point cam;
    Point light;
    XYZ normal;
    XYZ up;
    double dist;
    double vis_limit;
    double alpha;
    int width;
    int height;
    unsigned char bitmap[2];
    bitmap[0] = 0; bitmap[1] = 0;
    ifstream scene_param("scene_param.txt");
    if(scene_param.fail()){
        fprintf(stderr,"File scene_param.txt did not open!!!\n");
        return 1;
    }
    while(getline(scene_param,str)){
        if(sscanf(str.c_str(),"%s%n",buff,&n) != 1){
             fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
             return -1;
        }
        if(!strcmp(buff,"cam")){
            if(sscanf(str.c_str()+n,"%lf %lf %lf",&cam.x,&cam.y,&cam.z) != 3){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 1;
        }
        else if(!strcmp(buff,"normal")){
            if(sscanf(str.c_str()+n,"%lf %lf %lf",&normal.x,&normal.y,&normal.z) != 3){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 2;
        }
        else if(!strcmp(buff,"up")){
            if(sscanf(str.c_str()+n,"%lf %lf %lf",&up.x,&up.y,&up.z) != 3){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 4;
        }
        else if(!strcmp(buff,"screen")){
            if(sscanf(str.c_str()+n,"%lf",&dist) != 1){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 8;
        }
        else if(!strcmp(buff,"limit")){
            if(sscanf(str.c_str()+n,"%lf",&vis_limit) != 1){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 16;
        }
        else if(!strcmp(buff,"alpha")){
            if(sscanf(str.c_str()+n,"%lf",&alpha) != 1){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 32;
        }
        else if(!strcmp(buff,"width")){
            if(sscanf(str.c_str()+n,"%d",&width) != 1){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 64;
        }
        else if(!strcmp(buff,"height")){
            if(sscanf(str.c_str()+n,"%d",&height) != 1){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[0] |= 128;
        }
        else if(!strcmp(buff,"light")){
            if(sscanf(str.c_str()+n,"%lf %lf %lf",&light.x,&light.y,&light.z) != 3){
                fprintf(stderr,"scene_param.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            bitmap[1] |= 1;
        }
        else{
            fprintf(stderr,"scene_param.txt, line %d: Unexpected keyword %s\n",i,buff);
            return -1;
        }
        i++;
    }
    if(bitmap[0] != 255 || bitmap[1] != 1){
        fprintf(stderr, "scene_param.txt: not enough parameters\n");
        return -1;
    }
    Scene scene(cam,light,normal,up,dist,vis_limit,alpha,width,height);
    ifstream objects_file("objects.txt");
    if(objects_file.fail()){
        fprintf(stderr,"File objects.txt did not open!!!\n");
        return 2;
    }
    std::vector<Shape*> objects;
    std::vector<Triangle> triangles;
    Point centre;
    i = 0;
    while(getline(objects_file,str)){
        if(sscanf(str.c_str(),"%s%n",buff,&n) != 1){
             fprintf(stderr,"objects.txt, line %d: Incorrect input!!!\n",i);
             return -1;
        }
        if(!strcmp(buff,"sphere")){
            double r;
            if(sscanf(str.c_str()+n,"%lf %lf %lf %lf",&centre.x,&centre.y,&centre.z, &r) != 4){
                fprintf(stderr,"objects.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            objects.push_back(new Sphere(centre,r));
        }
        else if(!strcmp(buff,"box")){
            XYZ size;
            double psi = 0.0,fi = 0.0,teta = 0.0;
            if(sscanf(str.c_str()+n,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",&centre.x,&centre.y,&centre.z,&size.x,&size.y,&size.z,&psi,&fi,&teta) != 9){
                fprintf(stderr,"objects.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            triangles.clear();

            triangles.push_back({{size.x,-size.y,size.z},{size.x,-size.y,-size.z},{size.x,size.y,-size.z}});
            triangles.push_back({{size.x,-size.y,size.z},{size.x,size.y,-size.z},{size.x,size.y,size.z}});

            triangles.push_back({{size.x,size.y,size.z},{size.x,size.y,-size.z},{-size.x,size.y,-size.z}});
            triangles.push_back({{size.x,size.y,size.z},{-size.x,size.y,-size.z},{-size.x,size.y,size.z}});

            triangles.push_back({{-size.x,size.y,size.z},{-size.x,size.y,-size.z},{-size.x,-size.y,-size.z}});
            triangles.push_back({{-size.x,size.y,size.z},{-size.x,-size.y,-size.z},{-size.x,-size.y,size.z}});

            triangles.push_back({{-size.x,-size.y,size.z},{-size.x,-size.y,-size.z},{size.x,-size.y,-size.z}});
            triangles.push_back({{-size.x,-size.y,size.z},{size.x,-size.y,-size.z},{size.x,-size.y,size.z}});

            triangles.push_back({{size.x,size.y,size.z},{size.x,-size.y,size.z},{-size.x,-size.y,size.z}});
            triangles.push_back({{size.x,size.y,size.z},{-size.x,-size.y,size.z},{-size.x,size.y,size.z}});

            triangles.push_back({{size.x,size.y,-size.z},{size.x,-size.y,-size.z},{-size.x,-size.y,-size.z}});
            triangles.push_back({{size.x,size.y,-size.z},{-size.x,size.y,-size.z},{-size.x,-size.y,-size.z}});

            objects.push_back(new MeshShape(triangles,centre,psi,fi,teta,size.x));
        }
        else if(!strcmp(buff,"tetr")){
            Point p[4];
            if(sscanf(str.c_str()+n,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&p[0].x,&p[0].y,&p[0].z,&p[1].x,&p[1].y,&p[1].z,&p[2].x,&p[2].y,&p[2].z,&p[3].x,&p[3].y,&p[3].z) != 12){
                fprintf(stderr,"objects.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            Point min = {std::numeric_limits<double>::max(),std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};
            Point max = {std::numeric_limits<double>::min(),std::numeric_limits<double>::min(),std::numeric_limits<double>::min()};
            for(int j = 0; j < 4; j++){
                if(p[j].x < min.x){min.x = p[j].x;}
                if(p[j].y < min.y){min.y = p[j].y;}
                if(p[j].z < min.z){min.z = p[j].z;}
                if(p[j].x > max.x){max.x = p[j].x;}
                if(p[j].y > max.y){max.y = p[j].y;}
                if(p[j].z > max.z){max.z = p[j].z;}
            }
            centre = (min+max)*0.5;
            triangles.clear();
            triangles.push_back({p[0]-centre,p[1]-centre,p[2]-centre});
            triangles.push_back({p[1]-centre,p[2]-centre,p[3]-centre});
            triangles.push_back({p[0]-centre,p[2]-centre,p[3]-centre});
            triangles.push_back({p[0]-centre,p[1]-centre,p[3]-centre});
            objects.push_back(new MeshShape(triangles,centre,0.0,0.0,0.0,(max.x-min.x)/2));
        }
        else if(!strcmp(buff,"toyota-supra")){
            Point centre;
            double psi = 0.0,fi = 0.0,teta = 0.0;
            if(sscanf(str.c_str()+n,"%lf %lf %lf %lf %lf %lf",&centre.x,&centre.y,&centre.z,&psi,&fi,&teta) != 6){
                fprintf(stderr,"objects.txt, line %d: Incorrect input!!!\n",i);
                return -1;
            }
            ifstream ts("toyota-supra.obj");
            objects.push_back(new MeshShape(ts,centre,psi,fi,teta));
        }
        else{
            fprintf(stderr,"objects.txt, line %d: Unexpected keyword %s\n",i,buff);
            return -1;
        }
        i++;
    }
    delete[] buff;
    for(auto obj : objects){
        scene.add_object(obj);
    }
    scene.render("render.bmp");
    for(auto obj : objects){
        delete obj;
    }
    return 0;
}
