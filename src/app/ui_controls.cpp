#include "app/ui_controls.hpp"

#include "raylib.h"
#include "raygui.h"
#include <cmath>
#include <algorithm>

#include "app/app_state.hpp"
#include "app/ui_state.hpp"
#include "solar/sun_position.hpp"
#include "app/layout.hpp"
#include "app/map.hpp"

using namespace std;

void map_interaction_check(AppState& state, Rectangle map) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), map)) {
        double lat, lon;
        map_to_latlon(GetMousePosition(), map, lat, lon);
        state.location.set_lat_deg(lat);
        state.location.set_lon_deg(lon);    
    }
}

// note: passes a reference to app state, instead of a copy
void draw_controls(AppState& state, UiState& ui_state, Texture2D world_map) {
    VerticalLayout layout(10, 20, ui_state.sidebar_width - 20);

    layout.heading("Time");
    float day_f = static_cast<float>(state.day);
    GuiSliderBar(slider(layout.next()), "Day", TextFormat("%d", state.day), &day_f, 1, 365);
    state.day = static_cast<int>(std::lround(day_f));

    float min_f = static_cast<float>(state.min);
    GuiSliderBar(slider(layout.next()), "Min",
                    TextFormat("%02d:%02d", state.min / 60, state.min % 60),
                    &min_f, 0.0f, 1439.0f);
    state.min = static_cast<int>(std::lround(min_f));

    layout.heading("Place");
    float lat_f = static_cast<float>(state.location.lat_deg());
    GuiSliderBar(slider(layout.next()), "Latitude",
                    TextFormat("%.1f degrees", state.location.lat_deg()),
                    &lat_f, -90.0f, 90.0f);
    state.location.set_lat_deg(std::round(lat_f));

    float lon_f = static_cast<float>(state.location.lon_deg());
    GuiSliderBar(slider(layout.next()), "Longitude",
                    TextFormat("%.1f degrees", state.location.lon_deg()),
                    &lon_f, -180.0f, 180.0f);
    state.location.set_lon_deg(std::round(lon_f));

    double map_width = 400;
    Rectangle map = layout.next(map_width / 2.0f); 
    DrawTexturePro(world_map,
                   Rectangle{0, 0, (float)world_map.width, float(world_map.height)},
                   map,
                   Vector2{0, 0}, 0.0f, WHITE);

    map_interaction_check(state, map);

    Vector2 dot = latlon_to_map(state.location.lat_deg(), state.location.lon_deg(), map);
    DrawCircleV(dot, 4.0f, RED);
    DrawCircleLinesV(dot, 4.0f, WHITE);

    layout.heading("Building");
    float width_f = static_cast<float>(state.building.width_m());
    GuiSliderBar(slider(layout.next()), "Width",
                    TextFormat("%.1f m", state.building.width_m()),
                    &width_f, 1.0f, 20.0f);
    state.building.set_width_m(std::round(width_f * 2.0f) / 2.0f);

    float length_f = static_cast<float>(state.building.length_m());
    GuiSliderBar(slider(layout.next()), "Length",
                    TextFormat("%.1f m", state.building.length_m()),
                    &length_f, 1.0f, 20.0f);
    state.building.set_length_m(std::round(length_f * 2.0f) / 2.0f);

    float rot_f = static_cast<float>(state.building.rot_deg());
    GuiSliderBar(slider(layout.next()), "Rotation",
                    TextFormat("%.0f degrees", state.building.rot_deg()),
                    &rot_f, 0.0f, 360.0f);
    state.building.set_rot_deg(std::round(rot_f));

}

void wall_selection_check(AppState& state, UiState& ui_state, CanvasView& view) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, view.area)) {
            auto walls = state.building.walls();
            for (int i = 0; i < 4; ++i) {
                Vector2 a = view.world_to_screen(walls[i].start());
                Vector2 b = view.world_to_screen(walls[i].end());
                if (CheckCollisionPointLine(mouse, a, b, 8)) {
                    ui_state.selected_wall = i;
                    break;
                }
            }
        }
    }
}

void draw_selected_wall_overlay(AppState& state, UiState& ui_state, CanvasView& view) {
    if (ui_state.selected_wall) {
        Wall w = state.building.walls()[*ui_state.selected_wall];

        Rectangle box{ view.area.x + 20, view.area.y + 20, 260, ui_state.overlay_height};

        VerticalLayout layout(box.x + 20, box.y + 35, box.width - 40);

        if (GuiWindowBox(box, TextFormat("Wall %d", *ui_state.selected_wall))) {
            ui_state.selected_wall.reset();
        } else {
            // time a wall is lit in a day
            int lit_time = sun_mins_wall(
                state.day, 
                state.location.lat_deg(),
                state.location.lon_deg(),
                state.utc_offset_h,
                w.normal_azimuth_deg());
            int lit_hours = lit_time / 60;
            int lit_mins  = lit_time % 60;

            // total length of the day
            int day_length = day_length_mins(
                state.day, 
                state.location.lat_deg(),
                state.location.lon_deg(),
                state.utc_offset_h
                );
            int day_hours = day_length / 60;
            int day_mins  = day_length % 60;
            
            SunPosition sun_pos = calc_azim_elev(
                state.day, state.min, 
                state.location.lat_deg(), 
                state.location.lon_deg(), 
                state.utc_offset_h
            );
            bool lit_now = is_lit(w.normal_azimuth_deg(), sun_pos.azimuth_deg, sun_pos.elevation_deg);

            layout.heading("Wall");
            GuiLabel(layout.next(), TextFormat("Facing: %.0f deg", w.normal_azimuth_deg()));

            layout.heading("Light Statistics");
            GuiLabel(layout.next(), TextFormat("Sun on this wall today: %d:%02d (h:mm)", lit_hours, lit_mins));
            GuiLabel(layout.next(), TextFormat("Day length: %d:%02d (h:mm)", day_hours, day_mins));
            GuiLabel(layout.next(), TextFormat("Lit now: %s", lit_now ? "yes" : "no"));

            layout.heading("Sun Position");
            GuiLabel(layout.next(), TextFormat("Azimuth: %.0f deg", sun_pos.azimuth_deg));
            GuiLabel(layout.next(), TextFormat("Elevation: %.0f deg", sun_pos.elevation_deg));

            ui_state.overlay_height = layout.cursor_y() - box.y + 10;
        }

    }
}