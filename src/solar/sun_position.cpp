#include <numbers>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "solar/sun_position.hpp"
#include "core/angles.hpp"

using namespace std;

/// @brief Computes the angle of a point in time from 0-360 degrees
/// Assumes leap year.
/// @param day_of_year The index of the day (1-365 where Jan 1 is 1)
/// @param hour Hour 0-24
/// @param minute Minute 0-60
/// @return Angle in degrees
double day_position(int day, int hour, int min) {                            
  double day_pos = 2*numbers::pi / 365 * (day - 1 + (hour - 12) / 24.0 + min / (60.0 * 24)); 
  return rad_to_deg(day_pos); 
}

/// @brief Gap between clock time and actual sun time (minutes)
/// @param day_pos Angle for a point in time. 0 is the beggining of the year, 180 is the midpoint.
/// @return Gap in minutes
double equation_of_time(double day_pos) {
  day_pos = deg_to_rad(day_pos); 
  double equation_of_time = 229.18 * (0.000075 
    + 0.001868*cos(day_pos) 
    - 0.032077*sin(day_pos) 
    - 0.014615*cos(2*day_pos) 
    - 0.040849*sin(2*day_pos)); 
  return equation_of_time; 
}

/// @brief How far north / south of the equator the sun is at a specific time
/// @param day_pos Angle for a point in time. 0 is the beggining of the year, 180 is the midpoint.
/// @return Angle in degrees
double declination(double day_pos) {
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

double clock_minutes_since_midnight(int hour, int min) {
  double clock_minutes_since_midnight = hour * 60 + min; 
  return clock_minutes_since_midnight; 
}

/// @brief Calculates the true solar time
/// @param EoT Gap between clock time and actual sun time (minutes)
/// @param lon Longitude in degrees
/// @param utc_offset_hours Offset from UTC (note: will change with daylight savings time!)
/// @param clock_minutes_since_midnight 
/// @return True solar time
double true_solar_time(double EoT, double lon, double utc_offset_hours, double clock_minutes_since_midnight) {
  double time_offset = EoT + 4 * lon - 60 *utc_offset_hours;
  double solar_time = clock_minutes_since_midnight + time_offset; 
  return solar_time; 
}

/// @brief Calculates the hour angle
/// @param solar_time True solar time
/// @return hour angle in degrees
double hour_angle(double solar_time) {
    double hour_angle = solar_time / 4 - 180; 
    return hour_angle; 
}

/// @brief Calculates the elevation of the sun at a specific time and place
/// @param lat Latitude in degrees
/// @param decl Declination in degrees
/// @param hour_angle Hour angle in degrees
/// @return Elevation in degrees
double elevation(double lat, double decl, double ha) {
    lat = deg_to_rad(lat); 
    ha = deg_to_rad(ha); 
    decl = deg_to_rad(decl); 

    double x = sin(lat) * sin(decl) + cos(lat) * cos(decl) * cos(ha);
    double elevation = asin(clamp(x, -1.0, 1.0));
    return rad_to_deg(elevation);
}

/// @brief Calculate the Sun's position degrees clockwise (from north)
/// @param lat Latitude in degrees 
/// @param decl Declination in degrees
/// @param ha Hour Angle in degrees
/// @return Sun azimuth in degrees
double azimuth(double lat, double decl, double ha) {
    lat = deg_to_rad(lat); 
    decl = deg_to_rad(decl); 
    ha = deg_to_rad(ha); 
    
    double x = sin(ha); 
    double y = cos(ha) * sin(lat) - tan(decl) * cos(lat); 
    double azimuth = atan2(x, y) + numbers::pi; 
    return rad_to_deg(azimuth); 
}

/// @brief Whether the face of a plane is lit by the sun.
/// @param outward_normal_azimuth The normal azimuth of the plane in degrees
/// @param azimuth The azimuth of the sun in degrees
/// @param elevation The elevation of the sun in degrees
/// @return Boolean
bool is_lit(double outward_normal_azimuth, double azimuth, double elevation) {
  outward_normal_azimuth = deg_to_rad(outward_normal_azimuth); 
  azimuth = deg_to_rad(azimuth); 

  // remove a small epsilon due to angle conversion not being exact; 
  // so that walls that are exactly parallel to the sun are not seen as lit
  double x = cos(azimuth - outward_normal_azimuth) - (1e-10); 

  return elevation > 0 && x > 0; 
}

/// @brief Calculates the number of minutes of sun a wall receives
/// If the sun shines on the surface at the start of a minute one full minute is counted.
/// @param day_nr The ordinal number of the day in the year
/// @param lat Latitude in degrees
/// @param lon Longitude in degrees
/// @param utc_offset UTC offset in hours. Note: changes with daylight savings time
/// @param n_azimuth Normal azimuth in degrees of wall
/// @return nr of minutes
int sun_mins_day(int day, double lat, double lon, double utc_offset, double n_azimuth) {
  int lit_mins = 0;
  int hour = 0;
  int min = 0;
  
  for (hour = 0; hour < 24; hour++) {
    for (min = 0; min < 60; min++) {
      double day_pos = day_position(day, hour, min); 
      double since_midnight = clock_minutes_since_midnight(hour, min); 
      double EoT = equation_of_time(day_pos); 
      double solar_time = true_solar_time(EoT, lon, utc_offset, since_midnight); 
      double hour_ang = hour_angle(solar_time); 
      double decl = declination(day_pos); 
      double az = azimuth(lat, decl, hour_ang); 
      double elev = elevation(lat, decl, hour_ang);
      if (is_lit(n_azimuth, az, elev)) lit_mins += 1;
    }
  }
  return lit_mins; 
}

