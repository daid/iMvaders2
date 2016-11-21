[VERTEX]
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;

varying highp vec2 v_uv;

void main()
{
    gl_Position = vec4(position, 1.0);
    v_uv = (inverse(projection_matrix) * vec4(position, 1.0)).xy;
}

[FRAGMENT]
uniform sampler2D texture_map;
uniform vec4 color;

varying highp vec2 v_uv;

out vec4 out_color;

void main()
{
    out_color = texture2D(texture_map, v_uv) * color;
}