#include "scene.h"
#include <chrono>
Scene::Scene(Point cam, Point light,Vector screen_normal, Vector up, double dist, double vis_limit, double alpha, int w, int h):cam(cam),light(light),dist(dist),vis_limit(vis_limit),alpha(alpha){
    this->screen_normal = screen_normal*(1/screen_normal.lenght());
    this->up = up*(1/up.lenght());
    this->screen_res[0] = w;
    this->screen_res[1] = h;
}
void Scene::render(const char *filename){
    Vector sideways,start;
    double screen_size[2];
    double pixel_size;
    //unsigned char res[screen_res[0]][screen_res[1]];

    screen_size[1] = dist*sin(alpha);
    pixel_size = screen_size[1]/screen_res[1];
    screen_size[0] = static_cast<double>(screen_res[0])*pixel_size;

    sideways.x = up.y*screen_normal.z-up.z*screen_normal.y;
    sideways.y = up.z*screen_normal.x-up.x*screen_normal.z;
    sideways.z = up.x*screen_normal.y-up.y*screen_normal.x;

    start = screen_normal*dist+sideways*((screen_size[0]-pixel_size)/2.0)+up*((screen_size[1]-pixel_size)/2.0);

    using namespace cimg_library;
    CImg<unsigned char> image(screen_res[0],screen_res[1],1,3,0);

    //Colorize
    constexpr unsigned char min_color = 64;
    constexpr unsigned char max_color = 191;

    double min_dist;
    double max_dist;

    for(auto obj : objects){
        if((obj->centre-cam).lenght() < min_dist){
            min_dist = (obj->centre-cam).lenght();
        }
        if((obj->centre-cam).lenght() > max_dist){
            max_dist = (obj->centre-cam).lenght();
        }
    }

    for(auto obj : objects){
        obj->color = (min_color+(((obj->centre-cam).lenght()-min_dist)/max_dist)*max_color);
    }
    //

    std::chrono::time_point<std::chrono::system_clock> start_t = std::chrono::system_clock::now();

#pragma omp parallel
{

    //N-normal,V - on camera, L - on light, H = L+V/|L+V|
   Vector Ntmp,N,V,L,H;
   Ray ray;
   ray.begin = cam;
   Point intersect_point;
   Shape* qq;
   double t,tmin = std::numeric_limits<double>::max();
   double ambient;
   double diffuse;
   double specular;
   double pixel_color;
   double sNL,sNV,sLV,sRV,sNH;

#pragma omp for
    for(int j = 0; j < screen_res[1]; j++){
        for(int i = 0; i < screen_res[0]; i++){
            //calculate ray direction
            ray.direction = start-sideways*(pixel_size*i)-up*(pixel_size*j);
            V = ray.direction*(-1);
            ray.direction = ray.direction*(1/dist);
            V = V*(1/V.lenght());
            //
            tmin = std::numeric_limits<double>::max();
            for(auto obj : objects){
                if(obj->intersections_with_ray(ray,&t,&Ntmp)){
                    if(t < tmin && t > dist){
                        N = Ntmp;
                        tmin = t;
                        qq = obj;
                    }
                }
            }
            if(tmin > vis_limit){
                //res[i][j] = 0;
                image(i,j,0) = image(i,j,1) = image(i,j,2) = 0;
            }
            else{
                //printf("tmin = %lf\n",tmin);
                intersect_point = ray.begin+ray.direction*tmin;
                L = light-intersect_point;
                L = L*(1/L.lenght());
                H = L+V;
                H = H*(1/H.lenght());
                sNL = N*L;
                sNL = sNL > 0 ? sNL : 0.0;
                //sNV = N*V;
                //sNV = sNV > 0 ? sNV : 0.0;
                //sLV = L*V;
                //sLV = sLV > 0 ? sLV : 0.0;
                //sRV = sNL*sNV-sLV;
                //sRV = sRV > 0 ? sRV : 0.0;
                sNH = N*H;
                sNH = sNH > 0 ? sNH : 0.0;
                ambient = qq->Ka*static_cast<double>(qq->color);
                diffuse = static_cast<double>(qq->color)*sNL*qq->Kd;
                //specular = 255.0*(std::pow(sRV,1.0)); Fong
                specular = qq->Ks*light_color*(std::pow(sNH,qq->shinines));//Blinn-Fong
                pixel_color = ambient+diffuse+specular;
                //res[i][j] = static_cast<unsigned char>(pixel_color>255?255:pixel_color<0?0:pixel_color);
                //printf("%d, %d, color = %d %lf\n",i,j,static_cast<unsigned char>(pixel_color>255?255:pixel_color<0?0:pixel_color),pixel_color);
                image(i,j,0) = image(i,j,1) = image(i,j,2) = static_cast<unsigned char>(pixel_color>255?255:pixel_color<0?0:pixel_color);
            }
        }
    }
}
    //image.noise(128);
    std::chrono::time_point<std::chrono::system_clock> end_t = std::chrono::system_clock::now();
    int elapsed_s = static_cast<int>( std::chrono::duration_cast<std::chrono::seconds>(end_t - start_t).count() );
    printf("render time = %d seconds\n",elapsed_s);
    image.save(filename);
    image.display("Render");
}
void Scene::add_object(Shape *qq){
    objects.push_back(qq);
}
