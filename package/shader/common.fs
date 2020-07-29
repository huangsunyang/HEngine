#version 410 core

in vec4 vs_color;
// in vec2 TexCoord;

out vec4 color;

uniform sampler2D s;
// uniform sampler2D s1;


void main(void) {
    color = vs_color;
    // color = mix(texture(s, TexCoord), texture(s1, TexCoord), 0.2) * vs_color;
    // color = vec4(TexCoord, 0, 1);
    // color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);
}
