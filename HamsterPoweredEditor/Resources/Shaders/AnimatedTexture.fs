
#version 460 core

in vec3 FragColor;
in vec2 v_TexCoord;

uniform float CurrentTime = 0.0;
uniform float AlphaThreshold = 0.25;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

uniform vec2 UVTransform;
uniform vec2 UVScale;


out vec4 FinalColor;

vec3 hueShift(vec3 color, float hue) {
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hue);
    return vec3(color * cosAngle + cross(k, color) * sin(hue) + k * dot(k, color) * (1.0 - cosAngle));
}

void main()
{

    vec2 uv =  v_TexCoord / UVScale + UVTransform;
    vec4 col = texture(Texture0, uv);
    if (col.a <= AlphaThreshold) {
        discard;
    }
    
    FinalColor = col;
}


