#include <vector>
#include <algorithm>

#include "raylib.h"

Vector2 latlon_to_map(double lat, double lon, Rectangle map) {
    return {map.x + float((lon + 180.0) / 360.0) * map.width,
            map.y + float((90.0 - lat) / 180.0) * map.height
    };
}

void map_to_latlon(Vector2 p, Rectangle map, double& lat, double& lon) {
    lon = std::clamp((p.x - map.x) / map.width * 360.0 - 180.0, -180.0, 180.0);
    lat = std::clamp(90.0 - (p.y - map.y) / map.height * 180.0,  -90.0,  90.0);
}