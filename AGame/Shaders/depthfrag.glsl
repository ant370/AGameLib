#version 330 core
out vec4 FragColor;
 

in vec2 Position;
in vec2 Texture; 

void main()
{   
    if(texColor.a < 0.01)
        discard;
    FragColor = vec4(1.0,1.0,1.0,1.0);
}  