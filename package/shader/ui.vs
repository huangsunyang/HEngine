#version 430 core

layout (location = 0) in vec4 position;

out vec4 vs_color;

uniform mat4 mvp_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;
uniform mat4 m_matrix_it;
uniform vec4 color;

void main(void) {
    gl_Position = position;
    vs_color = color;
}