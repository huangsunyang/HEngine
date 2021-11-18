#version 410 core

in vec4 o_position;
in vec4 o_lightSpaceScreenPos;
in vec3 o_normal;
in vec2 o_coord;

out vec4 color;

uniform sampler2D s0;
uniform sampler2D shadow_map;

struct Light {
    vec4 pos;
    vec4 dir;
    vec4 color;
};


layout (std140) uniform ConstantBlock {
    uniform mat4 view_matrix;
    uniform mat4 proj_matrix;
    uniform mat4 light_view_matrix;
    uniform mat4 light_proj_matrix;
    uniform uvec4 light_num_info;
    uniform Light[10] light_info;
};

#define LIGHT_NUM light_num_info.x


float shadow_calculation(vec4 lightSpaceFragPos) {
    vec3 projCoords = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closest_depth = texture(shadow_map, projCoords.xy).r;
    return projCoords.z < closest_depth + 0.003 ? 0.0 : 1.0;
}


void main(void) {
    float light = 1.0;
    for (int i = 0; i < LIGHT_NUM; i++) {
        light += dot(o_normal, normalize(light_info[i].pos.xyz));
    }

    float shadow = shadow_calculation(o_lightSpaceScreenPos);
    light = (max(light, 1.0) - shadow);
    color = (vec4(light, light, light, 1) + vec4(0.5, 0.5, 0.5, 0)) * texture(s0, o_coord);
}