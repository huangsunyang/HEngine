#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 coord;
layout (location = 2) in vec4 normal;

out vec4 o_position;
out vec4 o_lightSpaceScreenPos;
out vec3 o_normal;
out vec2 o_coord;


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

uniform mat4 m_matrix;

void main(void) {
    gl_Position = proj_matrix * view_matrix * m_matrix * position;
    o_lightSpaceScreenPos = light_proj_matrix * light_view_matrix * m_matrix * position;
    o_normal = normalize((transpose(m_matrix) * normal).xyz);
    o_position = position;
    o_coord = coord;
}
