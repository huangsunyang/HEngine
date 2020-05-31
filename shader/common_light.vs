#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

out vec4 o_position;
out vec3 o_normal;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix_it;

void main(void) {
    gl_Position = mvp_matrix * position;
    o_normal = normalize((m_matrix_it * normal).xyz);
    o_position = position;
}
