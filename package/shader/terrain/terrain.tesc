#version 410 core

layout (vertices = 4) out;

in vec4 o_position[];
in vec2 o_texcoord[];
out vec4 tesc_position[];
out vec2 tesc_texcoord[];

void main(void)
{
	// Only if I am invocation 0 ...
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 20.0;
		gl_TessLevelInner[1] = 20.0;
		gl_TessLevelOuter[0] = 18.0;
		gl_TessLevelOuter[1] = 18.0;
		gl_TessLevelOuter[2] = 18.0;
		gl_TessLevelOuter[3] = 18.0;
	}

	// Everybody copies their input to their output
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tesc_position[gl_InvocationID] = o_position[gl_InvocationID];
	tesc_texcoord[gl_InvocationID] = o_texcoord[gl_InvocationID];
}