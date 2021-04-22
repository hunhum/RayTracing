#include "meshshape.h"
#include<chrono>

MeshShape::MeshShape(std::ifstream &file):centre({}),psi(0),fi(0),teta(0){
    std::string str;
    Triangle t;
    Point p;
    int v0,v1,v2;
    std::vector<Point> points;
    while(getline(file,str)){
        if(str[0] == 'v' && str[1] != 'n'){
             if(sscanf(str.c_str()+1,"%lf %lf %lf",&p.x,&p.y,&p.z)==3){
                points.push_back(p);
             }
        }
        if(str[0] == 'f'){
            if(sscanf(str.c_str()+1,"%d %d %d",&v0,&v1,&v2)==3){
                t.p0 = points[v0-1];
                t.p1 = points[v1-1];
                t.p2 = points[v2-1];
                triangles.push_back(t);
            }
        }
    }
    make_octtree();
}
MeshShape::MeshShape(Point centre, double psi, double fi, double teta,std::ifstream &file):centre(centre),psi(psi),fi(fi),teta(teta){
    std::string str;
    Triangle t;
    Point p;
    int v0,v1,v2;
    std::vector<Point> points;
    while(getline(file,str)){
        if(str[0] == 'v' && str[1] != 'n'){
             if(sscanf(str.c_str()+1,"%lf %lf %lf",&p.x,&p.y,&p.z)==3){
                points.push_back(p);
             }
        }
        if(str[0] == 'f'){
            if(sscanf(str.c_str()+1,"%d %d %d",&v0,&v1,&v2)==3){
                //printf("%d %d %d\n",v0,v1,v2);
                t.p0 = points[v0-1];
                t.p1 = points[v1-1];
                t.p2 = points[v2-1];
                triangles.push_back(t);
            }
        }
    }
    printf("points number = %d\n",points.size());
    printf("triangles number = %d\n",triangles.size());
    make_octtree();
}
MeshShape::MeshShape(const std::vector<Triangle>& triangles):centre({0,0,0}),psi(0),fi(0),teta(0),triangles(triangles){
    make_octtree();
}
MeshShape::MeshShape(Point centre, double psi, double fi, double teta,const std::vector<Triangle>& triangles):centre(centre),psi(psi),fi(fi),teta(teta),triangles(triangles){
    make_octtree();
}
MeshShape::~MeshShape(){
    if(octtree != nullptr){
        delete octtree;
    }
}
void MeshShape::make_octtree(){
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    Point min = {std::numeric_limits<double>::max(),std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};
    Point max = {std::numeric_limits<double>::min(),std::numeric_limits<double>::min(),std::numeric_limits<double>::min()};
    for(auto i : triangles){
        for(int j = 0; j < 3; j++){
            if(i[j].x < min.x){min.x = i[j].x;}
            if(i[j].y < min.y){min.y = i[j].y;}
            if(i[j].z < min.z){min.z = i[j].z;}
            if(i[j].x > max.x){max.x = i[j].x;}
            if(i[j].y > max.y){max.y = i[j].y;}
            if(i[j].z > max.z){max.z = i[j].z;}
        }
    }
    toctt::Box boundary{(min+max)*0.5,(max-min)*0.7};
    octtree = new toctt::TriangleOctTree(boundary, triangles);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    int elapsed_ms = static_cast<int>( std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() );
    printf("OctTree creating time = %d milliseconds\n",elapsed_ms);
}
XYZ MeshShape::coord_transform_to_global(XYZ l){
    XYZ g;
    g.x = (cos(psi)*cos(fi)-cos(teta)*sin(psi)*sin(fi))*l.x+(-cos(fi)*sin(psi)-cos(psi)*cos(teta)*sin(fi))*l.y+(sin(teta)*sin(fi))*l.z;
    g.y = (cos(teta)*cos(fi)*sin(psi)+cos(psi)*sin(fi))*l.x+(cos(psi)*cos(teta)*cos(fi)-sin(psi)*sin(fi))*l.y+(-cos(fi)*sin(teta))*l.z;
    g.z = (sin(psi)*sin(teta))*l.x+(cos(psi)*sin(teta))*l.y+(cos(teta))*l.z;
    return g;
}
XYZ MeshShape::coord_transform_to_local(XYZ g){
    XYZ l;
    double alpha = -fi;
    double beta = -teta;
    double gama = -psi;
    l.x = (cos(alpha)*cos(gama)-cos(beta)*sin(alpha)*sin(gama))*g.x+(-cos(gama)*sin(alpha)-cos(alpha)*cos(beta)*sin(gama))*g.y+(sin(beta)*sin(gama))*g.z;
    l.y = (cos(beta)*cos(gama)*sin(alpha)+cos(alpha)*sin(gama))*g.x+(cos(alpha)*cos(beta)*cos(gama)-sin(alpha)*sin(gama))*g.y+(-cos(gama)*sin(beta))*g.z;
    l.z = (sin(alpha)*sin(beta))*g.x+(cos(alpha)*sin(beta))*g.y+(cos(beta))*g.z;
    return l;
}
bool MeshShape::intersections_with_ray(Ray ray, double *t, Vector *normal){
    Ray lray;
    Vector N;
    Triangle qq;
    double tmin;
    lray.begin = coord_transform_to_local(ray.begin-centre);
    lray.direction = coord_transform_to_local(ray.direction);
    if(octtree == nullptr){
        throw std::exception();
    }
    else{
        if(!octtree->intersections_with_ray(lray,&tmin,&qq)){
            return false;
        }
    }
    *t = tmin;
    N.x = (qq.p0-qq.p1).y*(qq.p0-qq.p2).z-(qq.p0-qq.p1).z*(qq.p0-qq.p2).y;
    N.y = (qq.p0-qq.p1).z*(qq.p0-qq.p2).x-(qq.p0-qq.p1).x*(qq.p0-qq.p2).z;
    N.z = (qq.p0-qq.p1).x*(qq.p0-qq.p2).y-(qq.p0-qq.p1).y*(qq.p0-qq.p2).x;
    N = N*(1/N.lenght());
    if(lray.direction*N > 0){
        N = N*(-1);
    }
    (*normal) = coord_transform_to_global(N);
    return true;
}
