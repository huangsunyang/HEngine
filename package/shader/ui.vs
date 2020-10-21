#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec4 vs_color;
out vec2 vs_texCoord;

uniform mat4 m_matrix;
uniform vec4 color;

void main(void) {
    gl_Position = m_matrix * position;
    vs_color = color;
    vs_texCoord = texCoord;
}