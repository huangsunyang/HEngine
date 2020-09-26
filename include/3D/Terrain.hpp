#include "GLObject/Drawable.hpp"
#include "shape2d.hpp"

class Terrain: public HPolygon {
public:
    Terrain() {
        loadVertex({
            -1, 0, +1,
            -1, 0, -1,
            +1, 0, +1,
            +1, 0, +1,
            -1, 0, -1,
            +1, 0, -1,
        });
        setShader({
            "Package/shader/common_light_no_tex.vs",
            // "Package/shader/terrain.tesc",
            "Package/shader/common_light_no_tex.fs",
        });
    }
};
