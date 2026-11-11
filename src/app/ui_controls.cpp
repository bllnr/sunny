#include "app/ui_controls.hpp"

#include "raylib.h"
#include "raygui.h"
#include <cmath>

#include "app/app_state.hpp"
#include "app/ui_state.hpp"
#include "solar/sun_position.hpp"

using namespace std;

namespace {
    constexpr float kLeft   = 80;
    constexpr float kTop    = 80;
    constexpr float kWidth  = 300;
    constexpr float kHeight = 20;
    constexpr float kGap    = 12;
}

Rectangle row(int i) {
    return Rectangle{ 
        kLeft, kTop + i * (kHeight + kGap), 
        kWidth,
        kHeight
    };
}

// note: passes a reference to app state, instead of a copy
void draw_controls(AppState& state, UiState&) {
    float day_f = static_cast<float>(state.day);
    GuiSliderBar(row(0), "Day",
                    TextFormat("%d", state.day),
                    &day_f, 1.0f, 365.0f);
    state.day = static_cast<int>(std::lround(day_f));

    float min_f = static_cast<float>(state.min);
    GuiSliderBar(row(1), "Min",
                    TextFormat("%02d:%02d", state.min / 60, state.min % 60),
                    &min_f, 0.0f, 1439.0f);
    state.min = static_cast<int>(std::lround(min_f));

    float lat_f = static_cast<float>(state.location.lat_deg());
    GuiSliderBar(row(2), "Latitude",
                    TextFormat("%.1f degrees", state.location.lat_deg()),
                    &lat_f, -90.0f, 90.0f);
    state.location.set_lat_deg(std::round(lat_f));

    float lon_f = static_cast<float>(state.location.lon_deg());
    GuiSliderBar(row(3), "Longitude",
                    TextFormat("%.1f degrees", state.location.lon_deg()),
                    &lon_f, -180.0f, 180.0f);
    state.location.set_lon_deg(std::round(lon_f));

    float width_f = static_cast<float>(state.building.width_m());
    GuiSliderBar(row(4), "Width",
                    TextFormat("%.1f m", state.building.width_m()),
                    &width_f, 1.0f, 20.0f);
    state.building.set_width_m(std::round(width_f * 2.0f) / 2.0f);

    float length_f = static_cast<float>(state.building.length_m());
    GuiSliderBar(row(5), "Length",
                    TextFormat("%.1f m", state.building.length_m()),
                    &length_f, 1.0f, 20.0f);
    state.building.set_length_m(std::round(length_f * 2.0f) / 2.0f);

    float rot_f = static_cast<float>(state.building.rot_deg());
    GuiSliderBar(row(6), "Rotation",
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

        constexpr float kRowH = 20, kRowGap = 2, kSectionGap = 10;
        constexpr int   kRows = 9, kSections = 3;

        float height = 35
                     + kRows * (kRowH + kRowGap)
                     + (kSections - 1) * kSectionGap
                     + 10;

        Rectangle box{ view.area.x + 20, view.area.y + 20, 260, height};

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

            float y = box.y + 35;                      

            auto next = [&](float height) {            
                Rectangle r{ box.x + 10, y, box.width - 20, height };
                y += height + 2;
                return r;
            };

            auto gap = [&](float px) { y += px; };

            GuiLine (next(20), "Wall");
            GuiLabel(next(20), TextFormat("Facing: %.0f deg", w.normal_azimuth_deg()));

            gap(5);
            GuiLine (next(20), "Lighting");
            GuiLabel(next(20), TextFormat("Sun on this wall today: %d:%02d (h:mm)", lit_hours, lit_mins));
            GuiLabel(next(20), TextFormat("Day length: %d:%02d (h:mm)", day_hours, day_mins));
            GuiLabel(next(20), TextFormat("Lit now: %s", lit_now ? "yes" : "no"));

            gap(5);
            GuiLine (next(20), "Sun position");
            GuiLabel(next(20), TextFormat("Azimuth: %.0f deg", sun_pos.azimuth_deg));
            GuiLabel(next(20), TextFormat("Elevation: %.0f deg", sun_pos.elevation_deg));
        }

    }
}