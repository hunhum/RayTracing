#ifndef BASE_H
#define BASE_H
#include<vector>
#include<limits>
#include<cmath>
#include<iostream>
#include<cmath>
#include<exception>
#include<fstream>

bool almost_equal(double x, double y);
bool almost_equal_zero(double x);

struct XY{
    double x;
    double y;
    double& operator[](int i);
    XY operator+(const XY& qq);
    XY operator-(const XY& qq);
    XY operator*(double qq);
};

struct XYZ{
    double x;
    double y;
    double z;
    XYZ() = default;
    double& operator[](int i);
    bool operator==(const XYZ& qq);
    bool operator!=(const XYZ& qq);
    XYZ operator+(const XYZ& qq);
    XYZ operator-(const XYZ& qq);
    XYZ operator*(double qq);
    double lenght();
    double dot(const XYZ& qq);
};

double operator*(const XYZ& qq1, const XYZ& qq2);

using Point = XYZ;
using Vector = XYZ;

struct Ray{
    Point begin;
    Vector direction;
};

#endif // BASE_H
