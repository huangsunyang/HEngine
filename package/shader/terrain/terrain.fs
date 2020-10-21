#version 410 core

out vec4 color;

in vec3 tese_normal;

struct Light {
    vec4 pos;
    vec4 dir;
    vec4 color;
};


layout (std140) uniform ConstantBlock {
    uniform mat4 view_matrix;
    uniform mat4 proj_matrix;
    uniform mat4 light_view_matrix;
    uniform mat4 light_proj_matrix;
    uniform uvec4 light_num_info;
    uniform Light[10] light_info;
};

#define LIGHT_NUM light_num_info.x

void main(void) {
    float light = 0;
    for (int i = 0; i < LIGHT_NUM; i++) {
        light += dot(tese_normal, normalize(light_info[i].pos.xyz));
    }
    light = clamp(light, 0, 1);
    color = vec4(light, light, light, 1);
}