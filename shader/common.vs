#version 430 core

layout (location = 0) in vec4 position;
// layout (location = 1) in vec2 texcoord;
// layout (location = 1) in vec4 normal;
out vec4 vs_color;
// out vec2 TexCoord;

uniform mat4 mvp_matrix;
uniform mat4 m_matrix_it;

void main(void) {
    gl_Position = mvp_matrix * position;
    // vec3 normals = normalize((m_matrix_it * normal).xyz);
    // vs_color = clamp(dot(normals, vec3(-1,1,-1)), 0, 1) * vec4(1, 1, 1, 1);
    vs_color = vec4(1, 1, 1, 1) * position;
    vs_color.r = clamp(vs_color.r, 0, 1);
    vs_color.g = clamp(vs_color.g, 0, 1);
    vs_color.b = clamp(vs_color.b, 0, 1);
    vs_color.a = clamp(vs_color.a, 0, 1);
    // TexCoord = texcoord;
}