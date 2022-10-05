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