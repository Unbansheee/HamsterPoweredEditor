#version 460 core

in vec3 FragColor;
in vec2 v_TexCoord;

uniform float CurrentTime;
uniform float AlphaThreshold = 0.0;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

out vec4 FinalColor;


void main()
{

    FinalColor = vec4(1.f, 1.f, 1.f, 1.f);
}


