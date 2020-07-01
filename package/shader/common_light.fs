#version 410 core

in vec4 o_position;
in vec3 o_normal;
in vec2 o_coord;

out vec4 color;

uniform sampler2D s0;

struct Light {
    vec4 pos;
    vec4 dir;
    vec4 color;
};


layout (std140) uniform ConstantBlock {
    uniform mat4 view_matrix;
    uniform mat4 proj_matrix;
    uniform vec4 light_num_info;
    uniform Light[10] light_info;
};

void main(void) {
    float light = clamp(dot(o_normal, normalize(light_info[0].pos.xyz)), 0, 1);
    color = vec4(light) * texture(s0, o_coord);
}