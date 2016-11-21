[VERTEX]
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;

varying highp vec2 v_uv1;
varying highp vec2 v_uv2;
varying highp vec2 v_uv3;

void main()
{
    gl_Position = vec4(position, 1.0);
    v_uv1 = (inverse(projection_matrix * camera_matrix) * vec4(position, 1.0)).xy / 100.0;
    mat4 camera_matrix2 = camera_matrix;
    camera_matrix2[3][0] *= 0.5;
    camera_matrix2[3][1] *= 0.5;
    v_uv2 = (inverse(projection_matrix * camera_matrix2) * vec4(position, 1.0)).xy / 100.0;
    camera_matrix2[3][0] *= 0.5;
    camera_matrix2[3][1] *= 0.5;
    v_uv3 = (inverse(projection_matrix * camera_matrix2) * vec4(position, 1.0)).xy / 100.0;
}

[FRAGMENT]
uniform sampler2D texture_map;
uniform vec4 color;

varying highp vec2 v_uv1;
varying highp vec2 v_uv2;
varying highp vec2 v_uv3;

out vec4 out_color;

void main()
{
    out_color = texture2D(texture_map, v_uv1) * color + texture2D(texture_map, v_uv2) * color * 0.5 + texture2D(texture_map, v_uv3) * color * 0.25;
}
