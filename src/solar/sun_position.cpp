#include <numbers>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "solar/sun_position.hpp"

using namespace std;

double deg_to_rad(double deg) {
  double rad = deg * numbers::pi / 180; 
  return rad; 
}

double rad_to_deg(double rad) {
  double deg = rad * 180 / numbers::pi;  
  return deg; 
}

double day_position(int day_of_year, int hour, int minute) {
  /**
   * @brief Computes the angle of a point in time (day, minute, and hour) from 0-360
   * 
   * 0 is Jan 1 00:00
   * 180 is the midpoint of the year
   * 
   * @param day_of_year number of day 1-365
   * @param hour (12 is noon on the given day)
   * @param minute  
   */
  // day position in the year (uses standard year, not leap year)                                            
  double day_position = 2*numbers::pi / 365 * (day_of_year - 1 + (hour - 12) / 24.0 + minute / 60); 
  return rad_to_deg(day_position); 
}

double equation_of_time(double day_pos) {
  // gap between clock time and actual sun time (minutes)
  day_pos = deg_to_rad(day_pos); 
  double equation_of_time = 229.18 * (0.000075 
    + 0.001868*cos(day_pos) 
    - 0.032077*sin(day_pos) 
    - 0.014615*cos(2*day_pos) 
    - 0.040849*sin(2*day_pos)); 
  return equation_of_time; 
}

double declination(double day_pos) {
  // how far north / south of the equator the sun is on a specific time (radians)
  day_pos = deg_to_rad(day_pos); 
  double declination = 0.006918 
                      - 0.399912*cos(day_pos)
                      + 0.070257*sin(day_pos) 
                      - 0.006758*cos(2*day_pos)
                      + 0.000907*sin(2*day_pos) 
                      - 0.002697*cos(3*day_pos) 
                      + 0.00148*sin(3*day_pos); 
  return rad_to_deg(declination); 
}

double clock_minutes_since_midnight(int hour, int minute) {
  double clock_minutes_since_midnight = hour * 60 + minute; 
  return clock_minutes_since_midnight; 
}

double true_solar_time(double EoT, double longitude, double utc_offset_hours, double clock_minutes_since_midnight) {
  double time_offset = EoT + 4 * longitude - 60 *utc_offset_hours;
  double solar_time = clock_minutes_since_midnight + time_offset; 
  return solar_time; 
}

double hour_angle(double solar_time) {
    double hour_angle = solar_time / 4 - 180; 
    return hour_angle; 
}

double elevation(double latitude, double declination, double hour_angle) {
    latitude = deg_to_rad(latitude); 
    hour_angle = deg_to_rad(hour_angle); 
    declination = deg_to_rad(declination); 

    double x = sin(latitude) * sin(declination) + cos(latitude) * cos(declination) * cos(hour_angle);
    double elevation = asin(clamp(x, -1.0, 1.0));
    return rad_to_deg(elevation);
}

double azimuth(double latitude, double declination, double hour_angle) {
    // sun position degrees clockwise (from north)
    latitude = deg_to_rad(latitude); 
    declination = deg_to_rad(declination); 
    hour_angle = deg_to_rad(hour_angle); 
    
    double x = sin(hour_angle); 
    double y = cos(hour_angle) * sin(latitude) - tan(declination) * cos(latitude); 
    double azimuth = atan2(x, y) + numbers::pi; 
    return rad_to_deg(azimuth); 
}

bool is_lit(double outward_normal_azimuth, double azimuth, double elevation) {
  // if the face of a building is lit by the sun
  outward_normal_azimuth = deg_to_rad(outward_normal_azimuth); 
  azimuth = deg_to_rad(azimuth); 

  // remove a small epsilon due to angle conversion not being exact; 
  // so that walls that are exactly parallel to the sun are not seen as lit
  double x = cos(azimuth - outward_normal_azimuth) - (1e-10); 

  return elevation > 0 && x > 0; 
}

