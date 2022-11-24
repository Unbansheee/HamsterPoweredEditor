#version 460

#include "Resources/Shaders/Common.glsl"

uniform vec3 u_Color;

void main()
{
    FinalColor = vec4(u_Color, 1.0);
}