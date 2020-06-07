#version 410 core

in vec4 o_position;
in vec3 o_normal;

out vec4 color;

void main(void) {
    float light = clamp(dot(o_normal, vec3(-1,1,-1)), 0, 1);
    color = vec4(light, light, light, 1);
}