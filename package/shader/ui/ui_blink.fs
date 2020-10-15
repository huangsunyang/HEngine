#version 430 core

in vec4 vs_color;
in vec2 vs_texCoord;

out vec4 color;

uniform sampler2D s0;
uniform float sys_time;
uniform float start_time = 0.0;
uniform float time = 0.3;
uniform float range = 0.2;
uniform float steep = 50;

void main(void) {
    float cur_time = sys_time - start_time;
    cur_time -= int(cur_time / time) * time;
    float percent = cur_time / time * 2;

    vec4 c = texture(s0, vs_texCoord);
    vec3 c3 = c.rgb;
    if (vs_texCoord.x < percent * vs_texCoord.y + range && vs_texCoord.x > percent * vs_texCoord.y - range) {
        c3 *=  steep;
    }
    color = vec4(c3, c.a);
}