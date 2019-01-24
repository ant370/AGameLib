#version 330 core 
layout (location = 0) in vec4 aPos; 
layout (location = 1) in vec4 aText;
layout (location = 2) in vec4 aNormal;

uniform mat4 transform;
uniform mat4 project;
uniform mat4 model;

uniform vec4 directLightPos; 
uniform vec4 directLightColor;  
uniform mat4 lightView;
uniform mat4 lightProject;

out vec4 outText;
out vec4 outNormal;
out vec3 FragPos; 
out vec4 FragPosLightSpace;



uniform sampler2D ourTexture;
uniform sampler2D shadowMap;


void main() 
{   
    FragPos =  (vec4(aPos.xyz,1.0) * model).xyz;
    outText = aText;
    outNormal = inverse(model) * aNormal ; 
    
    FragPosLightSpace = vec4(aPos.xyz,1.0) * model  * lightView * lightProject;
    gl_Position =  vec4(aPos.x, aPos.y, aPos.z, 1.0) * model *  transform * project;
}
