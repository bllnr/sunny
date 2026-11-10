#include <cmath>
#include <iostream>
#include <stdexcept>
#include <array>

#include "model/model.hpp"
#include "core/angles.hpp"

using namespace std;

namespace {
    double outward_normal_azimuth(Point2 start, Point2 end) {
        double dx = end.x_east_m - start.x_east_m;
        double dy = end.y_north_m - start.y_north_m;
        double azimuth = azimuth_deg(-dy, dx); 
        return azimuth;
    }; 

    Point2 rotated_point(double x, double y, double deg) { 
        double rad = deg_to_rad(deg); 
        double x_east_m = x*cos(rad) + y*sin(rad); 
        double y_north_m = -x*sin(rad) + y*cos(rad); 
        return Point2(x_east_m, y_north_m); 
    }
}

std::array<Wall, 4> Building::walls() const {
    // compute the four corners
    // center point is in the middle of the building
    // x is east, y is north
    Point2 c0 = rotated_point(-length_m_/2.0, -width_m_/2.0, rot_deg());
    Point2 c1 = rotated_point(-length_m_/2.0, width_m_/2.0, rot_deg());
    Point2 c2 = rotated_point(length_m_/2.0, width_m_/2.0, rot_deg());
    Point2 c3 = rotated_point(length_m_/2.0, -width_m_/2.0, rot_deg());

    // connect in fixed order
    return {
        Wall(c0, c1, height_m_), 
        Wall(c1, c2, height_m_),
        Wall(c2, c3, height_m_),
        Wall(c3, c0, height_m_),
        }; 
}

Wall::Wall(Point2 start, Point2 end, double height_m)
    : start_(start), end_(end), height_m_(height_m),
    normal_azimuth_deg_(outward_normal_azimuth(start, end))
    {}

Building::Building(double width_m, double length_m, double height_m, double rot_deg)
    : width_m_(width_m), length_m_(length_m), height_m_(height_m),
      rot_deg_(normalize_deg(rot_deg))
{
    if (!(width_m > 0) || !(length_m > 0) || !(height_m > 0))
        throw std::invalid_argument("Building dimensions must be positive");
}; 




    
    