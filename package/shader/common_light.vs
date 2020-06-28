#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 coord;

out vec4 o_position;
out vec3 o_normal;
out vec2 o_coord;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix;


void main(void) {
    gl_Position = mvp_matrix * position;
    o_normal = normalize((transpose(m_matrix) * normal).xyz);
    o_position = position;
    o_coord = coord;
}
