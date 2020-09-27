#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 coord;

out vec4 o_position;
out vec3 o_normal;
out vec2 o_texcoord;


struct Light {
    vec4 pos;
    vec4 dir;
    vec4 color;
};


layout (std140) uniform ConstantBlock {
    uniform mat4 view_matrix;
    uniform mat4 proj_matrix;
    uniform uvec4 light_num_info;
    uniform Light[10] light_info;
};

uniform mat4 m_matrix;
uniform int len;

void main(void) {
    int x = gl_InstanceID % len;
    int z = gl_InstanceID / len;

    o_texcoord = (vec2(0.5, 0.5) + position.xz + vec2(x, z)) / float(len);
    o_position = position + vec4(x, 0, z, 0);
    gl_Position = o_position;
}