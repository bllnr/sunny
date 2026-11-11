#pragma once

#include "raylib.h"

class VerticalLayout {
    public:
        VerticalLayout(float x, float y, float width);

        Rectangle next(float height = kRowHeight);

        void gap(float px = kSectionGap);
        void heading(const char* text);

        float cursor_y() const { return y_; }

        static constexpr float kRowHeight = 20;
        static constexpr float kRowGap = 4;
        static constexpr float kSectionGap = 12;

    private:
        float x_;
        float y_;
        float width_;
        bool first_heading_ = true;
};

Rectangle slider(Rectangle r);