#pragma once

#include <vector>

#include "raylib.h"

Vector2 latlon_to_map(double lat, double lon, Rectangle map);

void map_to_latlon(Vector2 p, Rectangle map, double& lat, double& lon);