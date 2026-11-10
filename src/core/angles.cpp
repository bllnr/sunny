#include <numbers>
#include <cmath>

#include "core/angles.hpp"

using namespace std;

double deg_to_rad(double deg) {
  double rad = deg * numbers::pi / 180; 
  return rad; 
}

double rad_to_deg(double rad) {
  double deg = rad * 180 / numbers::pi;  
  return deg; 
}

double normalize_deg(double deg) {
    deg = std::fmod(deg, 360);
    if (deg < 0) deg += 360;
    if (deg >= 360) deg -= 360;   
    return deg; 
}; 

double azimuth_deg(double east, double north) {
    double normal_azimuth = atan2(east, north);
    normal_azimuth = rad_to_deg(normal_azimuth); 
    normal_azimuth = normalize_deg(normal_azimuth); 
    return normal_azimuth; 
}