[VERTEX]
#version 400

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

varying vec2 v_uv1;
varying vec2 v_uv2;
varying vec2 v_uv3;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    gl_Position = gl_Vertex;
    v_uv1 = (inverse(projection_matrix * camera_matrix * object_matrix) * gl_Vertex).xy / 50.0;
    mat4 camera_matrix2 = camera_matrix;
    camera_matrix2[3][0] *= 0.5;
    camera_matrix2[3][1] *= 0.5;
    v_uv2 = (inverse(projection_matrix * camera_matrix2 * object_matrix * rotationMatrix(vec3(0, 0, 1), 1.0f)) * gl_Vertex).xy / 50.0;
    camera_matrix2[3][0] *= 0.5;
    camera_matrix2[3][1] *= 0.5;
    v_uv3 = (inverse(projection_matrix * camera_matrix2 * object_matrix * rotationMatrix(vec3(0, 0, 1), 2.0f)) * gl_Vertex).xy / 50.0;
}

[FRAGMENT]
#version 110
uniform sampler2D texture_map;
uniform vec4 color;

varying vec2 v_uv1;
varying vec2 v_uv2;
varying vec2 v_uv3;

void main()
{
    gl_FragColor = texture2D(texture_map, v_uv1) * color + texture2D(texture_map, v_uv2) * color * 0.5 + texture2D(texture_map, v_uv3) * color * 0.25;
}
