#pragma once
#include "sb7/vmath.h"

using vmath::vec2;

enum class TouchEvent {
    BEGAN,
    MOVED,
    END,
    CANCEl
};


struct Touch {
    TouchEvent event;
    vec2 pos;
};