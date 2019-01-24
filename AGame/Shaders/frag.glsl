#version 330 core
out vec4 FragColor;

in vec4 outText;
in vec4 outNormal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform vec4 cameraPos;
uniform vec4 directLightPos; 
uniform vec4 directLightColor; 

uniform sampler2D ourTexture;
uniform sampler2D shadowMap;

struct PointLight {    
    vec3 position;
    vec3 color;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS]; 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float gamma = 2.2;
vec3 Uncharted2ToneMapping(vec3 color)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	float exposure = 3.;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
	color = pow(color, vec3(1. / gamma));
	return color;
}

 

float ShadowCalculation(vec4 fragPosLightSpace)
{

    float bias = 0.006;


    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = (projCoords * 0.5) + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z ; 
    // check whether current frag pos is in shadow
    float shadow = currentDepth - 0.0004 > closestDepth  ? 1.0 : 0.0; 
    
    
    if(projCoords.z > 1.0)
        shadow = 0.0;

    int count = 1;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -count; x <= count; ++x)
    {
        for(int y = -count; y <= count; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= ((count*2+1)*(count*2 + 1));


    return shadow ;
}  

void main()
{   
    vec4 Normal = normalize(outNormal);

    // Ambient and directional light
    float ambientStrength = 0.25;
    float sunStrength = 0.2;

    vec3 ambient = ambientStrength * vec3(1.0,1.0,1.0); 
    float diff = max(dot(Normal, directLightPos), 0.0); 
    vec3 diffuse = diff * sunStrength * vec3(directLightColor);
    vec4 objectColor = texture(ourTexture, outText.xy); 
    vec3 result = (ambient + diffuse) * vec3(objectColor);

    vec3 viewDir = normalize(cameraPos.xyz - FragPos.xyz);

    float shadow = ShadowCalculation(FragPosLightSpace);     
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse)) * result; 

    //point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        lighting += (CalcPointLight(pointLights[i], Normal.xyz, FragPos.xyz, viewDir) * objectColor.xyz);   
 
    

    FragColor = vec4(lighting, 1.0);  
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0); 

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);  

    // combine results 
    vec3 diffuse  = light.color  * diff;  
    
    return (diffuse);
} 
