#include "raylib.h"

#include <cmath>

#include "solar/sun_position.hpp"
#include "model/model.hpp"
#include "app/ui_controls.hpp"
#include "app/app_state.hpp"
#include "app/view.hpp"

using namespace std;

namespace {
    Color bg_color(AppState& state) {
        double elev = calc_azim_elev(
            state.day,
            state.min,
            state.location.lat_deg(),
            state.location.lon_deg(),
            state.utc_offset_h
            )[1];
        
        Color dark_blue = Color{16, 16, 54, 255};
        Color dark_green = Color{144, 160, 147, 255};
        Color mid_dark_green = Color{177, 192, 180, 255};
        Color mid_green = Color{209, 224, 213, 255};
        Color light_green = Color{237, 240, 238, 255};
        
        Color bg = Color{0,0,0,0};
        
        if (elev > 80)
            bg = light_green;
        else if (elev > 60)
            bg = mid_green;
        else if (elev > 40)
            bg = mid_dark_green;
        else if (elev > 20)
            bg = dark_green;
        else
            bg = dark_blue;

        return bg;
    };
}

int main() {
    Building building = Building(12, 8, 3, 0.0);
    Location location = Location(0.0, 0.0);
    int day = 100;
    int min = 12*60;
    AppState state = AppState(building, location, day, min, 0);

    InitWindow(1000, 600, "Sunny");
    SetTargetFPS(60);

    constexpr float kPanelWidth = 450;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Rectangle canvas{
            kPanelWidth, 0,
            GetScreenWidth() - kPanelWidth, 
            (float)GetScreenHeight()
            };
        
        DrawRectangleRec(canvas, bg_color(state));
        
        double diag = hypot(state.building.width_m(), state.building.length_m());
        float px_per_m = 0.65f * std::min(canvas.width, canvas.height) / static_cast<float>(diag);
        CanvasView view{ canvas, px_per_m };

        BeginScissorMode(
            (int)canvas.x,
            (int)canvas.y,
            (int)canvas.width,
            (int)canvas.height
            );

            draw_building(state, view);
            
        EndScissorMode();
        
        draw_controls(state); 

        DrawFPS(700, 20);
        EndDrawing();
    }

    CloseWindow();
}