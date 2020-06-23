#version 410 core

in vec4 o_position;
in vec3 o_normal;
in vec2 o_coord;

out vec4 color;

uniform vec3 light_dir;
uniform sampler2D s0;

void main(void) {
    float light = clamp(dot(o_normal, normalize(light_dir)), 0, 1);
    color = texture(s0, o_coord);
}