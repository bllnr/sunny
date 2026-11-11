#pragma once
#include <optional>

struct UiState {
    std::optional<int> selected_wall;
    float overlay_height = 200;
    float sidebar_width = 450;
};