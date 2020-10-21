#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 coord;

out vec2 TexCoord;
out vec4 vs_color;

void main(void) {
    gl_Position = position;
    TexCoord = coord;
    vs_color = position;
}
