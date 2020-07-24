#ifndef __TOUCH_HPP__
#define __TOUCH_HPP__

#include "sb7/vmath.h"
#include <string>
#include <vector>


using vmath::vec2;
using std::string;
using std::vector;


enum class TouchEvent {
    BEGAN,
    MOVED,
    END,
    CANCEl
};


string enumToString(TouchEvent);


struct Touch {
    TouchEvent event;
    vec2 pos;
};

#endif
