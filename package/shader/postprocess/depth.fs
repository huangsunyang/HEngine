#version 430 core

in vec4 vs_color;
in vec2 vs_texCoord;

out vec4 color;

uniform sampler2D s0;

float linearize_depth(float d, float zNear, float zFar) {
    float z_n = 2.0 * d - 1.0;
    float z_l = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    return (z_l - zNear) / (zFar - zNear);
}

void main(void) {
    float depth = linearize_depth(texture(s0, vs_texCoord).r, 0.1, 100.0);
    color = vec4(depth, depth, depth, 1.0);
}
