#version 410 core

layout(quads, equal_spacing, cw) in;
in vec4 tesc_position[];
in vec2 tesc_texcoord[];

out vec3 tese_normal;

uniform sampler2D s0;
uniform float dmap_depth;
uniform mat4 m_matrix;
uniform float diff;

struct Light {
    vec4 pos;
    vec4 dir;
    vec4 color;
};

layout (std140) uniform ConstantBlock {
    uniform mat4 view_matrix;
    uniform mat4 proj_matrix;
    uniform uvec4 light_num_info;
    uniform Light[10] light_info;
};

void main(void)
{
	vec4 pos1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 pos2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 position = mix(pos1, pos2, gl_TessCoord.y);

	vec2 texcoord1 = mix(tesc_texcoord[0], tesc_texcoord[1], gl_TessCoord.x);
	vec2 texcoord2 = mix(tesc_texcoord[2], tesc_texcoord[3], gl_TessCoord.x);
	vec2 tex = mix(texcoord1, texcoord2, gl_TessCoord.y);
	position.y += texture(s0, tex).r * dmap_depth;
	gl_Position = proj_matrix * view_matrix * m_matrix * position;

	vec2 x_diff = vec2(diff, 0.0f);
	vec2 z_diff = vec2(0.0f, diff);
	float x_height_diff = texture(s0, tex + x_diff).r * dmap_depth - position.y;
	float z_height_diff = texture(s0, tex + z_diff).r * dmap_depth - position.y;
	vec3 vx = normalize(vec3(diff, x_height_diff, 0));
	vec3 vy = normalize(vec3(0, z_height_diff, diff));

	tese_normal = normalize((transpose(m_matrix) * vec4(cross(vx, vy), 0.0)).xyz);
}