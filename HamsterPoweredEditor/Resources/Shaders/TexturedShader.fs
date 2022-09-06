#version 460 core

in vec3 FragColor;
in vec2 v_TexCoord;

uniform float CurrentTime;
uniform float AlphaThreshold = 0.25f;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

out vec4 FinalColor;

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


