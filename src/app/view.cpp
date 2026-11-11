#include "app/view.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "raylib.h"
#include "model/model.hpp"
#include "solar/sun_position.hpp"

namespace {
    Color wall_color(double elev) {
        Color wc = Color{0,0,0,0};

        if (elev >= 80)
            wc = Color{245, 244, 233, 255};
        else if (elev >= 60 )
            wc = Color{244, 237, 203, 255};
        else if (elev >= 40)
            wc = Color{242, 229, 165, 255};
        else if (elev >= 20)
            wc = Color{240, 196, 131, 255};
        else if (elev > 0) 
            wc = Color{237, 156, 90, 255};
        else
            wc = DARKGRAY;
        
        return wc;
    }
}

void draw_sun_arrow(const AppState&, CanvasView&) {};

void draw_north_arrow(CanvasView&) {
};

void draw_building(const AppState& state, const UiState& ui_state, const CanvasView& view) {
    SunPosition sun_pos = calc_azim_elev(
            state.day, state.min, 
            state.location.lat_deg(), 
            state.location.lon_deg(), 
            state.utc_offset_h
        );
    
    for (int i = 0; const Wall& wall : state.building.walls()) {
        Vector2 a = view.world_to_screen(wall.start());
        Vector2 b = view.world_to_screen(wall.end());

        if (ui_state.selected_wall && *ui_state.selected_wall == i) {
            DrawLineEx(a, b, 12.0f, SKYBLUE);
            DrawCircleV(a, 6.0f, SKYBLUE);
            DrawCircleV(b, 6.0f, SKYBLUE);
        }
        
        bool lit = is_lit(wall.normal_azimuth_deg(), sun_pos.azimuth_deg, sun_pos.elevation_deg);

        // warmer color closer to sunrise/sunset
        Color c = lit ? wall_color(sun_pos.elevation_deg) : DARKGRAY;
        DrawLineEx(a, b, 4.0f, c);
        ++i;
    }
};