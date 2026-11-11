#include "app/ui_controls.hpp"

#include "raylib.h"
#include "raygui.h"
#include <cmath>

#include "app/app_state.hpp"

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
void draw_controls(AppState& state) {
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