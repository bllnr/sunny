#pragma once

/// @brief 
class Location {
public:
    Location(double lat_deg, double lon_deg);
    double lat_deg() const {return lat_deg_; }; 
    double lon_deg() const {return lon_deg_; }; 
private:
    double lat_deg_;
    double lon_deg_;
}; 

/// @brief 
class DateTime {
public:
    DateTime(int day, int hour, int minute); 
    int day() const {return day_;}
    int hour() const {return hour_;}
    int min() const {return min_;}
private:
    int day_; // 1-365
    int hour_; // 0-23
    int min_; // 0-59
}; 