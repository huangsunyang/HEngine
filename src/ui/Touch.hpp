#ifndef __TOUCH_HPP__
#define __TOUCH_HPP__

#include "glm/glm.hpp"
#include <string>
#include <vector>


using glm::vec2;
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
