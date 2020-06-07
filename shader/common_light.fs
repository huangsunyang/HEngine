#version 410 core

in vec4 o_position;
in vec3 o_normal;

out vec4 color;

uniform vec3 light_dir;

void main(void) {
    float light = clamp(dot(o_normal, normalize(light_dir)), 0, 1);
    color = vec4(light, light, light, 1);
}