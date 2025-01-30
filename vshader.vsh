#version 330
in highp vec4 a_position;
in highp vec3 a_normal;
in highp float a_selected;

uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;
uniform int u_elementType;

out highp vec4 v_position;
out highp vec3 v_normal;
out highp float v_selected;

void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;
    if (!(u_elementType == 3 || u_elementType == 4))
        gl_Position = u_projectionMatrix * mv_matrix * a_position;
    if (u_elementType == 1 || u_elementType == 2) {
        gl_Position.z = gl_Position.z - abs(gl_Position.z * u_elementType * 0.00001);
    } else if (u_elementType == 3 || u_elementType == 4) {
        gl_Position = a_position;
        v_position = a_position;
//        v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
//        v_selected = a_selected;
    }
    if (u_elementType != 3 && u_elementType != 4) {
        v_position = mv_matrix * a_position;
        v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
        v_selected = a_selected;
    }


}
