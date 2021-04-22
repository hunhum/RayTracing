#include"base.h"

bool almost_equal(double x, double y){
    constexpr int ulp = 32;
    return std::fabs(x-y) <= std::numeric_limits<double>::epsilon()*std::fabs(x+y)*ulp;
}
bool almost_equal_zero(double x){
    constexpr double eps = 1.e-10;
    return std::fabs(x) <= eps;
}

double& XY::operator[](int i){
    switch (i){
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    default:
        throw std::out_of_range("ЭЭЭЭЭ");
        break;
    }
}
XY XY::operator+(const XY& qq){
    XY tmp;
    tmp.x = x + qq.x;
    tmp.y = y + qq.y;
    return tmp;
}
XY XY::operator-(const XY& qq){
    XY tmp;
    tmp.x = x - qq.x;
    tmp.y = y - qq.y;
    return tmp;
}
XY XY::operator*(double qq){
    XY tmp;
    tmp.x = x * qq;
    tmp.y = y * qq;
    return tmp;
}

double& XYZ::operator[](int i){
    switch (i){
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    case 2:
        return z;
        break;
    default:
        throw std::out_of_range("ЭЭЭЭЭ");
        break;
    }
}
XYZ XYZ::operator+(const XYZ& qq){
    XYZ tmp;
    tmp.x = x + qq.x;
    tmp.y = y + qq.y;
    tmp.z = z + qq.z;
    return tmp;
}
XYZ XYZ::operator-(const XYZ& qq){
    XYZ tmp;
    tmp.x = x - qq.x;
    tmp.y = y - qq.y;
    tmp.z = z - qq.z;
    return tmp;
}
XYZ XYZ::operator*(double qq){
    XYZ tmp;
    tmp.x = x * qq;
    tmp.y = y * qq;
    tmp.z = z * qq;
    return tmp;
}
bool XYZ::operator==(const XYZ &qq){
    if(!almost_equal(x,qq.x)){
        return false;
    }
    if(!almost_equal(y,qq.y)){
        return false;
    }
    if(!almost_equal(z,qq.z)){
        return false;
    }
    return true;
}
bool XYZ::operator!=(const XYZ &qq){
    return !operator==(qq);
}
double XYZ::lenght(){
    return std::sqrt(x*x+y*y+z*z);
}
double XYZ::dot(const XYZ &qq){
    return x*qq.x+y*qq.y+z*qq.z;
}

double operator*(const XYZ &qq1, const XYZ &qq2){
    return qq1.x*qq2.x+qq1.y*qq2.y+qq1.z*qq2.z;
}
