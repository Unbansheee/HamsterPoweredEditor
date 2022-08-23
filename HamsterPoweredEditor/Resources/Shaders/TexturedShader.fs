#version 460 core

in vec3 FragColor;
in vec2 v_TexCoord;

uniform float CurrentTime;
uniform float AlphaThreshold = 0.0;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

out vec4 FinalColor;

vec3 hueShift(vec3 color, float hue) {
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hue);
    return vec3(color * cosAngle + cross(k, color) * sin(hue) + k * dot(k, color) * (1.0 - cosAngle));
}

void main()
{
    vec4 texColor = texture(Texture0, v_TexCoord);
    vec4 texColor2 = texture(Texture1, v_TexCoord);
    vec4 mixed = mix(texColor, texColor2, sin(CurrentTime) * 0.5 + 0.5);
    if (mixed.a <= AlphaThreshold) {
        discard;
    }
    FinalColor = mixed;
}


