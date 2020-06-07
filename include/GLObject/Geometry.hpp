#pragma once
#include "GLObject/Model.hpp"

class Box: public Model {
public:
    Box(float length, float width, float height) {
        // from bottom to up, from first area to fourth area
        loadVertexIndice({
            length / 2, width / 2, -height / 2,
            -length / 2, width / 2, -height / 2,
            -length / 2, -width / 2, -height / 2,
            length / 2, -width / 2, -height / 2,
             length / 2, width / 2, height / 2,
            -length / 2, width / 2, height / 2,
            -length / 2, -width / 2, height / 2,
            length / 2, -width / 2, height / 2
        }, {
            2, 1, 0,
            0, 3, 2,
            0, 4, 7,
            7, 3, 0,
            6, 5, 1,
            1, 2, 6,
            6, 7, 3,
            3, 2, 6,
            5, 4, 0,
            0, 1, 5,
            4, 5, 6,
            6, 7, 4
        });
    };

protected:
    float length;
    float width;
    float height;
};