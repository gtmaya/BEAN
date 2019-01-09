#version 430

//FragColor
layout (location=0) out vec4 FragColor;

//Light info
uniform vec3 lightPos[14];

//Shader parameters
uniform float diffAmount;
uniform vec3 materialDiff;
uniform float alpha;
uniform sampler2D diffuseMap;
uniform bool hasDiffMap;
uniform mat4 MV;

// Vectors from the vertex shader.
in vec3 fragNormal;
in vec4 fragPosition;
in vec2 fragTexCoord;
in vec3 worldPos;
in vec3 viewSpacePos;

void main()
{
  vec3 n = normalize(fragNormal);
  float totalLight = 0.f;
  vec3 materialDiffuseMap;
  if (hasDiffMap) {materialDiffuseMap = texture(diffuseMap, fragTexCoord).rgb;}
  else            {materialDiffuseMap = vec3(1.f);}

  for(int i = 0; i < 14; i++)
  {
    vec3 transformedLightPos = lightPos[i] * 100.f;
    float distance = length(transformedLightPos - worldPos.xyz);
    float attenuation = 1.f / (distance * distance);
    vec3 s = normalize(transformedLightPos - worldPos.xyz);
    float diffuseIntensity = 0.35f * max(dot(s, n), 0.0f);
    totalLight += diffuseIntensity * diffAmount;
  }
  if (totalLight < 0.6f)      {totalLight = 0.6f;}
  else if (totalLight < 0.75f)  {totalLight = 0.75f;}
  else if (totalLight < 0.8f) {totalLight = 0.8f;}
  else                         {totalLight = 1.f;}

  FragColor = vec4(totalLight * materialDiff * materialDiffuseMap, alpha);
}
