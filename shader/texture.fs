#version 430 core

in vec2 TexCoord;
in vec4 vs_color;

out vec4 color;

uniform sampler2D s;
uniform sampler2D s1;


void main(void) {
    color = mix(texture(s, TexCoord), texture(s1, TexCoord), 0.5);
    // color = vec4(TexCoord, 1, 1);
    // color = vs_color;
    // color = texelFetch(s, ivec2(gl_FragCoord.xy), 0);
}
