#include "octotree.h"
namespace toctt{
    bool intersection_lines_segments(XY p0, XY p1, XY q0, XY q1){
        if(almost_equal(p0[0],p1[0]) && almost_equal(p0[1],p1[1]) && almost_equal(q0[0],q1[0]) && almost_equal(q0[1],q1[1])){
            if(almost_equal(q0[0],p0[0])&&almost_equal(q0[1],p0[1])){
                return true;
            }
            else{
                return false;
            }
        }
        if(almost_equal(q0[0],q1[0])&&almost_equal(q0[1],q1[1])){
            if(almost_equal_zero((q0[0]-p0[0])*(p1[1]-p0[1])-(q0[1]-p0[1])*(p1[0]-p0[0]))){
                if(((q0[0]-p0[0])*(q0[0]-p1[0]) <= 0) && ((q0[1]-p0[1])*(q0[1]-p1[1]) <= 0)){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        if(almost_equal(p0[0],p1[0])&&almost_equal(p0[1],p1[1])){
            if(almost_equal_zero((p0[0]-q0[0])*(q1[1]-q0[1])-(p0[1]-q0[1])*(q1[0]-q0[0]))){
                if(((p0[0]-q0[0])*(p0[0]-q1[0]) <= 0) && ((p0[1]-q0[1])*(p0[1]-q1[1]) <= 0)){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        if(((q0[0]-p0[0])*(p1[1]-p0[1])-(q0[1]-p0[1])*(p1[0]-p0[0]))*((q1[0]-p0[0])*(p1[1]-p0[1])-(q1[1]-p0[1])*(p1[0]-p0[0])) > 0){
            return false;
        }
        if(((p0[0]-q0[0])*(q1[1]-q0[1])-(p0[1]-q0[1])*(q1[0]-q0[0]))*((p1[0]-q0[0])*(q1[1]-q0[1])-(p1[1]-q0[1])*(q1[0]-q0[0])) > 0){
            return false;
        }
        return true;
    }
    bool intersection_line_segment_with_rectangle(XY min, XY max, XY p0, XY p1){
        XY p[4];
        p[0] = {max[0],min[1]};
        p[1] = {max[0],max[1]};
        p[2] = {min[0],max[1]};
        p[3] = {min[0],min[1]};
        if(p0[0] < max[0] && p0[0] > min[0] && p0[1] < max[1] && p0[1] > min[1]){
            return true;
        }
        if(p1[0] < max[0] && p1[0] > min[0] && p1[1] < max[1] && p1[1] > min[1]){
            return true;
        }
        for(int i = 0; i < 3; i++){
            if(intersection_lines_segments(p[i],p[i+1],p0,p1)){
                //printf("p[%d] = {%lf,%lf}, p[%d] = {%lf,%lf}, p0 = {%lf,%lf}, p1={%lf,%lf}\n",i,p[i][0],p[i][1],i+1,p[i+1][0],p[i+1][1],p0[0],p0[1],p1[0],p1[1]);
                return true;
            }
        }
        if(intersection_lines_segments(p[3],p[0],p0,p1)){
            return true;
        }
        return false;
    }
    bool Box::contains_point(Point p){
        return ((p.x < middle.x+size.x || almost_equal(p.x,middle.x+size.x)) && (p.x > middle.x-size.x || almost_equal(p.x,middle.x-size.x)) &&
           (p.y < middle.y+size.y || almost_equal(p.y,middle.y+size.y)) && (p.y > middle.y-size.y || almost_equal(p.y,middle.y-size.y)) &&
           (p.z < middle.z+size.z || almost_equal(p.z,middle.z+size.z)) && (p.z > middle.z-size.z || almost_equal(p.z,middle.z-size.z)));
    }
    bool Box::contains_triangle(Triangle t){
        if(contains_point(t.p0) || contains_point(t.p1) || contains_point(t.p2)){
            return true;
        }
        Point p0 = t.p0;
        XYZ e1 = t.p1-t.p0;
        XYZ e2 = t.p2-t.p0;
        double u0 = 0.0 ,v0 = 0.0;
        bool i1 = true ,i2 = true;//Пересекает ли прямая, прямые v = 0, u = 0;
        Point ip1;//Первая точка пересечения
        Point ip2;//Вторая точка пересечения
        XY ip1_2d;
        XY ip2_2d;
        XY min, max;
        double q[6];
        q[0] = (middle.x+size.x);
        q[1] = (middle.x-size.x);
        q[2] = (middle.y+size.y);
        q[3] = (middle.y-size.y);
        q[4] = (middle.z+size.z);
        q[5] = (middle.z-size.z);
        //P1: x = (middle.x+size.x) = q1
        //P2: x = (middle.x-size.x) = q2
        //P3: y = (middle.y+size.y) = q3
        //P4: y = (middle.y-size.y) = q4
        //P5: z = (middle.z+size.z) = q5
        //P6: z = (middle.z-size.z) = q6
        //Пересечение с P1 p0.x+u*e1.x+v*e2.x = q1 <-- прямая в плоскости p0+u*e1+v*e2;
        //Если при u = 0  0 <= v <= 1 то прямая пересекает треугольник или
        //Если при v = 0  0 <= u <= 1 то прямая пересекает треугольник
        //Иначе не пересекает
        //u0 = (q.x-p0.x)/e1.x;
        //v0 = (q.x-p0.x)/e2.x;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 2; j++){
                i1 = false; i2 = false;
                if(!(std::fabs(e1[i]) < std::numeric_limits<double>::epsilon())){
                    u0 = (q[i*2+j]-p0[i])/e1[i];
                    //printf("u = %lf\n",u0);
                    i1 = true;
                }
                if(!(std::fabs(e2[i]) < std::numeric_limits<double>::epsilon())){
                    v0 = (q[i*2+j]-p0[i])/e2[i];
                    //printf("v = %lf\n",v0);
                    i2 = true;
                }
                if(!i1&&!i2){
                    if(almost_equal(q[i*2+j],p0[i])){
                        //Треугольник лежит в плоскости
                        XY p0_2d, p1_2d, p2_2d;
                        switch (i) {
                        case 0:
                            p0_2d = {t.p0[1],t.p0[2]};
                            p1_2d = {t.p1[1],t.p1[2]};
                            p2_2d = {t.p2[1],t.p2[2]};
                            min = {middle[1]-size[1],middle[2]-size[2]};
                            max = {middle[1]+size[1],middle[2]+size[2]};
                            break;
                        case 1:
                            p0_2d = {t.p0[2],t.p0[0]};
                            p1_2d = {t.p1[2],t.p1[0]};
                            p2_2d = {t.p2[2],t.p2[0]};
                            min = {middle[2]-size[2],middle[0]-size[0]};
                            max = {middle[2]+size[2],middle[0]+size[0]};
                            break;
                        case 2:
                            p0_2d = {t.p0[0],t.p0[1]};
                            p1_2d = {t.p1[0],t.p1[1]};
                            p2_2d = {t.p2[0],t.p2[1]};
                            min = {middle[0]-size[0],middle[1]-size[1]};
                            max = {middle[0]+size[0],middle[1]+size[1]};
                            break;
                        default:
                            break;
                        }
                        if(intersection_line_segment_with_rectangle(min,max,p0_2d,p1_2d)){
                            return true;
                        }
                        if(intersection_line_segment_with_rectangle(min,max,p0_2d,p2_2d)){
                            return true;
                        }
                        if(intersection_line_segment_with_rectangle(min,max,p1_2d,p2_2d)){
                            return true;
                        }
                        return false;
                    }
                    else {
                        continue;
                    }
                }
                else{
                    if(i1&&i2){
                        if((u0 < 0 || u0 > 1)&&(v0 < 0 || v0 > 1)){
                            continue;
                        }
                        if(u0 > 1 || u0 < 0){
                            //Пересечение будет со стороной p2-p1
                            //u/u0+v/v0 = 1;
                            //u+v = 1; v = 1-u;
                            //u(1/u0-1/v0)=(1-1/v0)
                            //u = (v0-1)*u0/(v0-u0)
                            //v = 1-(1-v0)*u0/(u0-v0) = v0(u0-1)/(u0-v0)
                            ip1 = p0+e1*(u0*(1-v0)/(u0-v0))+e2*(v0*(u0-1)/(u0-v0));
                        }
                        else{
                            ip1 = (p0+e1*u0);
                        }
                        if(v0 > 1 || v0 < 0){
                            ip2 = p0+e1*(u0*(1-v0)/(u0-v0))+e2*(v0*(u0-1)/(u0-v0));
                        }
                        else{
                            ip2 = (p0+e2*v0);
                        }
                    }
                    if(!i2){
                        if((u0 < 0 || u0 > 1)){
                            continue;
                        }
                        ip1 = (p0+e1*u0);
                        ip2 = (p0+e1*u0+e2*(1-u0));
                    }
                    if(!i1){
                        if((v0 < 0 || v0 > 1)){
                            continue;
                        }
                        ip1 = (p0+e2*v0);
                        ip2 = (p0+e1*(1-v0)+e2*v0);
                    }
                    //printf("ip1 = {%lf,%lf,%lf}, ip2 = {%lf,%lf,%lf} \n",ip1[0],ip1[1],ip1[2],ip2[0],ip2[1],ip2[2]);
                    switch (i) {
                    case 0:
                        ip1_2d={ip1[1],ip1[2]};
                        ip2_2d={ip2[1],ip2[2]};
                        min = {middle[1]-size[1],middle[2]-size[2]};
                        max = {middle[1]+size[1],middle[2]+size[2]};
                        break;
                    case 1:
                        ip1_2d={ip1[0],ip1[2]};
                        ip2_2d={ip2[0],ip2[2]};
                        min = {middle[0]-size[0],middle[2]-size[2]};
                        max = {middle[0]+size[0],middle[2]+size[2]};
                        break;
                    case 2:
                        ip1_2d={ip1[0],ip1[1]};
                        ip2_2d={ip2[0],ip2[1]};
                        min = {middle[0]-size[0],middle[1]-size[1]};
                        max = {middle[0]+size[0],middle[1]+size[1]};
                        break;
                    default:
                        break;
                    }
                    //printf("ip1_2d = {%lf,%lf}, ip2_2d = {%lf,%lf} \n",ip1_2d[0],ip1_2d[1],ip2_2d[0],ip2_2d[1]);
                    //printf("min = {%lf, %lf}, max = {%lf,%lf} \n",min[0],min[1],max[0],max[1]);
                    if(intersection_line_segment_with_rectangle(min,max,ip1_2d,ip2_2d)){
                        return true;
                    }
                }
            }
        }
        return false;
    }
    bool Box::contains_triangle_fast(Triangle t){
        Point min = {std::min(t.p0.x,std::min(t.p1.x,t.p2.x)),std::min(t.p0.y,std::min(t.p1.y,t.p2.y)),std::min(t.p0.z,std::min(t.p1.z,t.p2.z))};
        Point max = {std::max(t.p0.x,std::min(t.p1.x,t.p2.x)),std::max(t.p0.y,std::min(t.p1.y,t.p2.y)),std::max(t.p0.z,std::min(t.p1.z,t.p2.z))};
        return contains_box({(min+max)*0.5,(max-min)*0.5});
    }
    bool Box::contains_box(Box b){
        if(contains_point(b.middle)){
            return true;
        }
        for(int i = 0; i < 3; i++){
            if(b.middle[i]-b.size[i] > middle[i]+size[i] || b.middle[i]+b.size[i] < middle[i]-size[i]){
                return false;
            }
        }
        return true;
    }
    bool Box::intersect(Ray ray, double *t_near, double *t_far){
        // check whether initial point is inside the parallelepiped
        Point min_point = {middle.x-size.x,middle.y-size.y,middle.z-size.z};
        Point max_point = {middle.x+size.x,middle.y+size.y,middle.z+size.z};
        // ray parameter
        *t_near = std::numeric_limits<double>::min(), *t_far = std::numeric_limits<double>::max();
        double t1, t2;
        // directions loop
        for (int i = 0; i < 3; i++) {
            if (!almost_equal_zero(ray.direction[i])) {
                t1 = (min_point[i] - ray.begin[i]) / ray.direction[i];
                t2 = (max_point[i] - ray.begin[i]) / ray.direction[i];
                if (t1 > t2)
                std::swap(t1, t2);
                if (t1 > *t_near)
                *t_near = t1;
                if (t2 < *t_far)
                *t_far = t2;

                if (*t_near > *t_far)
                return false;
                if (*t_far < 0.0)
                return false;
            } // if
            else {
                if ( ray.begin[i] < min_point[i] || ray.begin[i] > max_point[i] )
                    return false;
            }
        } // for
        return (*t_near <= *t_far && *t_far >=0);
    }
    TriangleOctTree::TriangleOctTree(Box b, double min_size):min_size(min_size),boundary(b){
        for(int i = 0; i < 8; i++){
            child[i] = nullptr;
        }
    }
    TriangleOctTree::TriangleOctTree(Box b,const std::vector<Triangle>& t,double min_size):min_size(min_size),boundary(b){
        for(int i = 0; i < 8; i++){
            child[i] = nullptr;
        }
        std::cout << "Boundary size = " <<b.size.x << " " << b.size.y << " " << b.size.z << " middle = " << b.middle.x << " " << b.middle.y << " " << b.middle.z << std::endl;
        //buildtree(t);
        for(auto i : t){
            insert(i);
        }
    }
    TriangleOctTree::~TriangleOctTree(){
        deletetree(this);
    }
    void TriangleOctTree::deletetree(TriangleOctTree* tree){
        std::vector<TriangleOctTree*> stack;
        TriangleOctTree* node;
        for(int i = 0; i < 8; i++){
            if(tree->child[i] != nullptr){
                stack.push_back(tree->child[i]);
                tree->child[i] = nullptr;
            }
        }
        for(;!stack.empty();){
            node = stack.back();
            stack.pop_back();
            for(int i = 0; i < 8; i++){
                if(node->child[i] != nullptr){
                    stack.push_back(node->child[i]);
                    node->child[i] = nullptr;
                }
            }
            delete node;
        }
    }
    void TriangleOctTree::insert(Triangle t){
        Box b[8];
        XYZ s;
        std::vector<TriangleOctTree*> stack;
        TriangleOctTree* node;
        triangles.push_back(t);
        node = this;
        stack.push_back(node);
        for(;!stack.empty();){
            node = stack.back();
            stack.pop_back();
            s.x = node->boundary.size.x/2;
            s.y = node->boundary.size.y/2;
            s.z = node->boundary.size.z/2;
            if(s.x < min_size || s.y < min_size || s.z < min_size){
                node->ptrs_triangles.push_back(triangles.size()-1);
                continue;
            }
            b[0].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y+s.y,node->boundary.middle.z+s.z};
            b[1].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y-s.y,node->boundary.middle.z+s.z};
            b[2].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y-s.y,node->boundary.middle.z-s.z};
            b[3].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y+s.y,node->boundary.middle.z-s.z};
            b[4].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y+s.y,node->boundary.middle.z+s.z};
            b[5].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y-s.y,node->boundary.middle.z+s.z};
            b[6].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y-s.y,node->boundary.middle.z-s.z};
            b[7].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y+s.y,node->boundary.middle.z-s.z};
            for(int i = 0; i < 8; i++){
                b[i].size = s;
            }
            for(int i = 0; i < 8; i++){
                if(b[i].contains_triangle(t)){
                    //std::cout << "size = " <<s.x << " " << s.y << " " << s.z << " middle = " << b[i].middle.x << " " << b[i].middle.y << " " << b[i].middle.z << std::endl;
                    if(node->child[i] == nullptr){
                        node->child[i] = new TriangleOctTree(b[i]);
                    }
                    stack.push_back(node->child[i]);
                }
            }
        }
    }
    void TriangleOctTree::buildtree(const std::vector<Triangle>& triangles){
        deletetree(this);
        Box b[8];
        XYZ s;
        TriangleOctTree* node;
        std::vector<TriangleOctTree*> stack;
        std::vector<int> triangles_by_octants[8];
        this->triangles = triangles;
        for(int i = 0; i < triangles.size(); i++){
            this->ptrs_triangles.push_back(i);
        }
        node = this;
        stack.push_back(node);
        for(;!stack.empty();){
            node = stack.back();
            stack.pop_back();
            s.x = node->boundary.size.x/2;
            s.y = node->boundary.size.y/2;
            s.z = node->boundary.size.z/2;
            if(s.x < min_size || s.y < min_size || s.z <  min_size){
                continue;
            }
            b[0].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y+s.y,node->boundary.middle.z+s.z};
            b[1].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y-s.y,node->boundary.middle.z+s.z};
            b[2].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y-s.y,node->boundary.middle.z-s.z};
            b[3].middle = {node->boundary.middle.x+s.x,node->boundary.middle.y+s.y,node->boundary.middle.z-s.z};
            b[4].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y+s.y,node->boundary.middle.z+s.z};
            b[5].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y-s.y,node->boundary.middle.z+s.z};
            b[6].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y-s.y,node->boundary.middle.z-s.z};
            b[7].middle = {node->boundary.middle.x-s.x,node->boundary.middle.y+s.y,node->boundary.middle.z-s.z};
            for(int i = 0; i < 8; i++){
                b[i].size = s;
                triangles_by_octants[i].clear();
            }
            for(auto t : node->ptrs_triangles){
                for(int i = 0; i < 8; i++){
                    if(b[i].contains_triangle(this->triangles[t])){
                        triangles_by_octants[i].push_back(t);
                    }
                }
            }
            node->ptrs_triangles.clear();
            for(int i = 0; i < 8; i++){
                if(triangles_by_octants[i].empty()){
                    node->child[i] = nullptr;
                }
                else{
                    node->child[i] = new TriangleOctTree(b[i]);
                    node->child[i]->ptrs_triangles = triangles_by_octants[i];
                    stack.push_back(node->child[i]);
                }
            }
        }
    }
    bool TriangleOctTree::intersections_with_ray(Ray ray, double *t, Triangle* triangle){
        //Надо так сделать чтоб в стек кубы добавлялись в порядке обратном пресечению их с лучом;
        //std::vector<double> t_stack;
        std::vector<TriangleOctTree*> stack;
        TriangleOctTree* node = this;
        TriangleOctTree* b[4];
        double t_near,t_far,t1,t2,t3,t_tmp,tmin;
        Point near,far,p,p1,p2,p3;
        bool intersect = false;
        stack.push_back(node);
        for(;!stack.empty();){
            node = stack.back();
            stack.pop_back();
            // Можно ложить t_near, t_far, t1, t2, t3 в стэк потом извлекать делить на два и получать новые значения это наверное будет немного быстрее ,но мне лень так делать!!!
            if(!node->boundary.intersect(ray,&t_near,&t_far)){
                continue;
            }
            if(!node->ptrs_triangles.empty()){
                tmin = std::numeric_limits<double>::max();
                intersect = false;
                for(auto ptr : node->ptrs_triangles){
                    if(triangles[ptr].intersect(ray,&t_tmp)){
                        intersect = true;
                        if(t_tmp < tmin){
                            tmin = t_tmp;
                            *triangle = triangles[ptr];
                        }
                    }
                }
                if(intersect){
                    *t = tmin;
                    return true;
                }
                continue;
            }
            if(ray.direction[0] != 0){
                t1 = (node->boundary.middle[0]-ray.begin[0])/ray.direction[0];
            }
            else{
                t1 = -1;
            }
            if(ray.direction[0] != 0){
                t2 = (node->boundary.middle[1]-ray.begin[1])/ray.direction[1];
            }
            else{
                t2 = -1;
            }
            if(ray.direction[2] != 2){
                t3 = (node->boundary.middle[2]-ray.begin[2])/ray.direction[2];
            }
            else{
                t3 = -1;
            }
            if(t1 > t2){
                std::swap(t1,t2);
            }
            if(t2 > t3){
                std::swap(t2,t3);
            }
            if(t1 > t2){
               std::swap(t1,t2);
            }
            near = ray.begin+ray.direction*t_near;
            far = ray.begin+ray.direction*t_far;
            p1 = ray.begin+ray.direction*t1;
            p2 = ray.begin+ray.direction*t2;
            p3 = ray.begin+ray.direction*t3;
            for(int i = 0; i < 4; i++){b[i] = nullptr;}
            //Хаха жесть
            if(near.x > node->boundary.middle.x && near.y > node->boundary.middle.y && near.z > node->boundary.middle.z){
                if(node->child[0] != nullptr){
                    b[3] = node->child[0];
                }
            }
            else if(near.x > node->boundary.middle.x && near.y < node->boundary.middle.y && near.z > node->boundary.middle.z){
                if(node->child[1] != nullptr){
                    b[3] = node->child[1];
                }
            }
            else if(near.x > node->boundary.middle.x && near.y < node->boundary.middle.y && near.z < node->boundary.middle.z){
                if(node->child[2] != nullptr){
                    b[3] = node->child[2];
                }
            }
            else if(near.x > node->boundary.middle.x && near.y > node->boundary.middle.y && near.z < node->boundary.middle.z){
                if(node->child[3] != nullptr){
                    b[3] = node->child[3];
                }
            }
            else if(near.x < node->boundary.middle.x && near.y > node->boundary.middle.y && near.z > node->boundary.middle.z){
                if(node->child[4] != nullptr){
                    b[3] = node->child[4];
                }
            }
            else if(near.x < node->boundary.middle.x && near.y < node->boundary.middle.y && near.z > node->boundary.middle.z){
                if(node->child[5] != nullptr){
                    b[3] = node->child[5];
                }
            }
            else if(near.x < node->boundary.middle.x && near.y < node->boundary.middle.y && near.z < node->boundary.middle.z){
                if(node->child[6] != nullptr){
                    b[3] = node->child[6];
                }
            }
            else if(near.x < node->boundary.middle.x && near.y > node->boundary.middle.y && near.z < node->boundary.middle.z){
                if(node->child[7] != nullptr){
                    b[3] = node->child[7];
                }
            }
            //
            if(far.x > node->boundary.middle.x && far.y > node->boundary.middle.y && far.z > node->boundary.middle.z){
                if(node->child[0] != nullptr){
                    b[0] = node->child[0];
                }
            }
            else if(far.x > node->boundary.middle.x && far.y < node->boundary.middle.y && far.z > node->boundary.middle.z){
                if(node->child[1] != nullptr){
                    b[0] = node->child[1];
                }
            }
            else if(far.x > node->boundary.middle.x && far.y < node->boundary.middle.y && far.z < node->boundary.middle.z){
                if(node->child[2] != nullptr){
                    b[0] = node->child[2];
                }
            }
            else if(far.x > node->boundary.middle.x && far.y > node->boundary.middle.y && far.z < node->boundary.middle.z){
                if(node->child[3] != nullptr){
                    b[0] = node->child[3];
                }
            }
            else if(far.x < node->boundary.middle.x && far.y > node->boundary.middle.y && far.z > node->boundary.middle.z){
                if(node->child[4] != nullptr){
                    b[0] = node->child[4];
                }
            }
            else if(far.x < node->boundary.middle.x && far.y < node->boundary.middle.y && far.z > node->boundary.middle.z){
                if(node->child[5] != nullptr){
                    b[0] = node->child[5];
                }
            }
            else if(far.x < node->boundary.middle.x && far.y < node->boundary.middle.y && far.z < node->boundary.middle.z){
                if(node->child[6] != nullptr){
                    b[0] = node->child[6];
                }
            }
            else if(far.x < node->boundary.middle.x && far.y > node->boundary.middle.y && far.z < node->boundary.middle.z){
                if(node->child[7] != nullptr){
                    b[0] = node->child[7];
                }
            }
            //
            if(t1 > 0 && t2 > 0 && node->boundary.contains_point(p1) && node->boundary.contains_point(p2)){
                p = ray.begin+ray.direction*((t1+t2)/2);
                for(int i = 0; i < 8; i++){
                    if(node->child[i] == nullptr){
                        continue;
                    }
                    if(node->child[i]->boundary.contains_point(p)){
                        b[2] = node->child[i];
                        break;
                    }
                }
            }
            if(t2 > 0 && t3 > 0 && node->boundary.contains_point(p2) && node->boundary.contains_point(p3)){
                p = ray.begin+ray.direction*((t2+t3)/2);
                for(int i = 0; i < 8; i++){
                    if(node->child[i] == nullptr){
                        continue;
                    }
                    if(node->child[i]->boundary.contains_point(p)){
                        b[1] = node->child[i];
                        break;
                    }
                }
            }
            for(int i = 0; i < 4; i++){
                if(b[i]!=nullptr){
                    stack.push_back(b[i]);
                }
            }
        }
        return false;
    }
}
