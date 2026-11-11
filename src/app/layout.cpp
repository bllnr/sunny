#include "app/layout.hpp"
#include "raygui.h"

VerticalLayout::VerticalLayout(float x, float y, float width) 
    : x_(x), y_(y), width_(width) {}

Rectangle VerticalLayout::next(float height) {
    Rectangle r{x_, y_, width_, height};
    y_ += height + kRowGap;
    return r;
}

void VerticalLayout::gap(float px) {y_ += px;}

void VerticalLayout::heading(const char* text) {
    if (!first_heading_) gap();
    first_heading_ = false;
    GuiLine(next(), text);
}

Rectangle slider(Rectangle r) {
    float kLabelW = 60;
    float kValueW = 80;

    Rectangle s{r.x + kLabelW, r.y, r.width - kLabelW - kValueW, r.height};
    return s;
}