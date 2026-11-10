#include <stdexcept>
#include <cmath>

#include "core/core.hpp"

DateTime::DateTime(int day, int hour, int min) : day_(day), hour_(hour), min_(min) {
    if (!(day > 0) || !(day < 366))
        throw std::invalid_argument("Day must be 1-365");
    else if (!(hour > -1) || !(hour < 24))
        throw std::invalid_argument("Hour must be 0-23");
    else if (!(min > -1) || !(min < 60))
        throw std::invalid_argument("Min must be 0-59");
    
}

Location::Location(double lat_deg, double lon_deg) : lat_deg_(lat_deg), lon_deg_(lon_deg) {
    if (!(std::abs(lat_deg) <= 90))
        throw std::invalid_argument("Latitude must be -90 to 90");
    else if (!(std::abs(lon_deg) <= 180))
        throw std::invalid_argument("Longitude must be -180 to 180");
    }