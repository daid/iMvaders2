[VERTEX]
#version 400

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

varying vec2 v_uv;

void main()
{
    gl_Position = vec4(gl_Vertex.xyz, 1.0);
    v_uv = (inverse(projection_matrix) * gl_Vertex).xy * 0.5;
}

[FRAGMENT]
#version 110
uniform sampler2D texture_map;
uniform vec4 color;

varying vec2 v_uv;

void main()
{
    gl_FragColor = texture2D(texture_map, v_uv) * color;
}
