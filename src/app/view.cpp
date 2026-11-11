#include "app/view.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "raylib.h"
#include "model/model.hpp"
#include "solar/sun_position.hpp"

void draw_sun_arrow() {};

void draw_north_arrow() {};

void draw_building(const AppState& state, CanvasView& view) {
    for (const Wall& wall : state.building.walls()) {
        Vector2 a = view.world_to_screen(wall.start());
        Vector2 b = view.world_to_screen(wall.end());

        std::array sun_pos = calc_azim_elev(
            state.day, state.min, 
            state.location.lat_deg(), 
            state.location.lon_deg(), 
            state.utc_offset_h
        );
        
        bool lit = is_lit(wall.normal_azimuth_deg(), sun_pos[0], sun_pos[1]); 
        Color c = lit ? YELLOW : DARKGRAY;
        DrawLineEx(a, b, 4.0f, c);
    }
};