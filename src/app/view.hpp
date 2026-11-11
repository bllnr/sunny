#pragma once

#include "raylib.h"

#include "app/app_state.hpp"
#include "model/model.hpp"

struct CanvasView {
    Rectangle area;
    float px_per_m;

    Vector2 world_to_screen(Point2 p) const {
        float cx = area.x + area.width / 2;
        float cy = area.y + area.height / 2;
        return {cx + float(p.x_east_m) * px_per_m,
                cy - float(p.y_north_m) * px_per_m // north is up
                };
    }
};

void draw_sun_arrow(const AppState& state, CanvasView& view);

void draw_north_arrow(CanvasView& view);

void draw_building(const AppState& state, CanvasView& view);