#version 330
out highp vec4 v_FragColor;

uniform vec4 u_bgColor;
uniform int u_elementType;

void main(void)
{
    vec4 resultColor;
    if (u_elementType == 0) {
        resultColor = u_bgColor;
    }
    else {
        resultColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    v_FragColor = vec4(resultColor);
}
