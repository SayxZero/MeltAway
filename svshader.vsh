#version 330
in highp vec4 a_position;

uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;
uniform int u_elementType;

void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;
    gl_Position = u_projectionMatrix * mv_matrix * a_position;
    gl_Position.z = gl_Position.z - abs(gl_Position.z * u_elementType * 0.00001);
}
