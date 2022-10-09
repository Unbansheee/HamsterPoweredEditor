#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in float TextureID;

out vec2 FragTexCoords;
out float FragTextureID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform bool u_ScreenSpace = false;

uniform mat4 ProjectionMat;

void main()
{
    if (u_ScreenSpace)
    {
        mat4 MVP = ProjectionMat * u_Transform;
        gl_Position = MVP * vec4(Position, 1.0f);
    }
    else
    {
        mat4 MVP = u_ViewProjection * u_Transform;

        gl_Position = MVP * vec4(Position, 1.0f);
    }
    FragTexCoords = TexCoord;
    FragTextureID = TextureID;
}