#version 460 core

in vec2 FragTexCoords;

uniform sampler2D TextTexture;
uniform vec3 TextColor;
uniform bool Wireframe = false;

out vec4 FinalColor;

void main()
{
    float Alpha = texture(TextTexture, FragTexCoords).r;
    //FinalColor = vec4(1, 1, 1, 1);
    if (Wireframe)
    {
        FinalColor= vec4(1, 1, 1, 1);
    }
    else
    {
        FinalColor = vec4(TextColor, Alpha);
    }
    
    //FinalColor = vec4(TextColor, 1);
}