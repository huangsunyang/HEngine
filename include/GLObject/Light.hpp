#ifndef __LIGHT__
#define __LIGHT__

#include "GLObject/Geometry.hpp"
#include "sb7/vmath.h"

class Light: public Box {
public:
    Light(): Box(2, 2, 2) {
        setShader({"shader/light.vs", "shader/common.fs"});
    }
};

#endif