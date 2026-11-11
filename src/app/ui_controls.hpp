#pragma once

#include "app/app_state.hpp"
#include "app/ui_state.hpp"
#include "app/view.hpp"

void map_interaction_check(AppState& state, Rectangle map);

void draw_controls(AppState& state, UiState& ui_state, Texture2D world_map);

void wall_selection_check(AppState& state, UiState& ui_state, CanvasView& view);

void draw_selected_wall_overlay(AppState& state, UiState& ui_state, CanvasView& view);