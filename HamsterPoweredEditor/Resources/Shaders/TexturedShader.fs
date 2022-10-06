#version 460 core

struct DirLight
{
    vec3 Direction;
    vec3 Color;
    float Intensity;
};

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float Intensity;
    float Radius;
};

in vec3 FragColor;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Position;
in vec3 v_TransformedNormal;

uniform float CurrentTime;
uniform float AlphaThreshold = 0.25f;
uniform sampler2D u_Textures[32];
uniform bool Wireframe = false;
uniform bool Unlit = false;

uniform vec3 AmbientColour;
uniform float AmbientStrength;
uniform vec3 CameraPosition;

uniform PointLight PointLights[8];
uniform int PointLightCount;
uniform DirLight DirLights[8];
uniform int DirLightCount;

out vec4 FinalColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading

    vec3 HalfwayVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, HalfwayVector), 0.0), 500.0f);
    
    return (diff * light.Color + spec * light.Color) * light.Intensity;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float radius)
{
    vec3 lightDir = normalize(light.Position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // blinn-phong specular shading
    
    vec3 HalfwayVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, HalfwayVector), 0.0), 500.0f);


    // attenuation
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 - distance / radius;
    attenuation = clamp(attenuation, 0.0, 1.0);
        
    
    return (diff * light.Color + spec * light.Color) * light.Intensity * attenuation;
    
}

void main()
{
    vec4 texColor = texture(u_Textures[0], v_TexCoord);
    vec4 texColor2 = texture(u_Textures[1], v_TexCoord);
    vec4 mixed = mix(texColor, texColor2, sin(CurrentTime) * 0.5 + 0.5);
    if (mixed.a <= AlphaThreshold && !Wireframe) {
        discard;
    }
    
    if (Wireframe)
    {
        FinalColor = vec4(1, 1, 1, 1);
    }
    else if (Unlit)
    {
        FinalColor = vec4(mixed);
    }
    else
    {
        vec3 ambient = AmbientColour * AmbientStrength;
        vec3 normal = normalize(v_TransformedNormal);
        vec3 viewDir = normalize(CameraPosition - v_Position);
        
        
        vec3 DirectionalLight = vec3(0, 0, 0);
        vec3 PointLight = vec3(0, 0, 0);
        
        for (int i = 0; i < DirLightCount; i++)
        {
            DirectionalLight += CalcDirLight(DirLights[i], normal, viewDir);
        }
        
        
        for (int i = 0; i < PointLightCount; i++)
        {
            PointLight += CalcPointLight(PointLights[i], normal, v_Position, viewDir, PointLights[i].Radius);
        }
        


        vec3 result = (ambient + DirectionalLight + PointLight) * mixed.rgb;

        
        FinalColor = vec4(result, mixed.a);
    }

}


