#version 430 core

in vec2 TexCoord;
in vec4 vs_color;

out vec4 color;

uniform sampler2D s0;
uniform sampler2D s1;
uniform sampler2D s2;


void main(void) {
    color = texture(s0, TexCoord);
}
