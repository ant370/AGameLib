#version 330 core
out vec4 FragColor;
in vec2 TextCoords; 

uniform sampler2D tex;

void main()
{             
    vec4 acol = texture(tex, TextCoords);
    if(acol.a < 0.01)
        discard;
    FragColor = acol; 
}  