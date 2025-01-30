#version 330
uniform highp vec4 u_lightPosition;

in highp vec4 v_position;
in highp vec3 v_normal;
in highp float v_selected;

uniform int u_elementType;
out highp vec4 v_FragColor;

void main(void)
{
    vec3 resultColor = vec3(0.4, 0.4, 0.4);
    float transp = 1.0;
    if (u_elementType == 1 || u_elementType == 2) {
        transp = 1.0;
        resultColor = vec3(255.0 * v_selected / 255.0, 190.0 * v_selected / 255.0, 0.0 * v_selected / 255.0);
    } else if (u_elementType == 3 || u_elementType == 4) {
        transp = 0.3;
        resultColor = vec3(12.0 / 255.0, 136.0 / 255.0, 252.0 / 255.0);
        if (u_elementType == 4) {
            transp = 0.9;
            resultColor = vec3(12.0 / 255.0, 44.0 / 255.0, 252.0 / 255.0);
        }
    } else {
//        transp = 0.3;
        transp = 1.0;
        resultColor = resultColor + (vec3(255.0 / 255.0, 190.0 / 255.0, 0.0 / 255.0) - resultColor) * v_selected * v_selected * 0.7;
        vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
        float diffAspect = pow(dot(v_normal, -lightVect), 0.8);
        resultColor = resultColor * (diffAspect > 0.2 ? diffAspect : 0.2);
    }

    v_FragColor = vec4(resultColor, transp);

}
