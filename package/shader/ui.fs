#version 410 core

in vec4 vs_color;
in vec2 vs_texCoord;

out vec4 color;

uniform sampler2D s0;


void main(void) {
    color = vs_color;
}
