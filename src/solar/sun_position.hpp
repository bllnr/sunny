#pragma once

double day_position(int day_of_year, int hour, int minute);
double equation_of_time(double day_position);
double hour_angle(double solar_time);
double declination(double day_position);
double clock_minutes_since_midnight(int hour, int minute);
double true_solar_time(double EoT, double longitude, double utc_offset_hours, double clock_minutes_since_midnight);
double elevation(double latitude, double declination, double hour_angle);
double azimuth(double latitude, double declination, double hour_angle);
bool is_lit(double outward_normal_azimuth, double azimuth, double elevation);
