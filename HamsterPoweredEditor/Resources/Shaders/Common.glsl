
in vec3 FragColor;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Position;
in vec3 v_TransformedNormal;
in int v_TexIndex;

uniform float CurrentTime;
uniform float AlphaThreshold = 0.25f;
uniform sampler2D u_Textures[32];
uniform bool Wireframe = false;
uniform bool Unlit = false;

uniform vec3 AmbientColour;
uniform float AmbientStrength;
uniform vec3 CameraPosition;

out vec4 FinalColor;