#version 410 core

in vec4 o_position;
in vec3 o_normal;

out vec4 color;

void main(void) {
    color = clamp(dot(o_normal, vec3(-1,1,-1)), 0, 1) * o_position;
}