#pragma once

#include "core/core.hpp"      // Location (and DateTime if you use it)
#include "model/model.hpp"    // Building

struct AppState {
    Building building;
    Location location;
    int day;
    int min;
    double utc_offset_h;
};