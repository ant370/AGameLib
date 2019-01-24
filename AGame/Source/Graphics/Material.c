
#include "Graphics.h" 
#include "glew.h"


unsigned int GraphicsGenerateCheckerTexture(int patterns)
{ 
    patterns = 64;

    GLubyte data[64][64][4];
    

    for (int i = 0; i < patterns; i++)
    {
        for (int j = 0; j < patterns; j++)
        {
            int c = ((((i&0x8)==0)^((j&0x8))==0));
            if (c == 0)
                c = 125;
            else
                c = 195; 
            
            if (i == 0 || j == 0 || i == patterns -1 || j == patterns -1)
            {
                c = 80;
            }

            data[i][j][0] = (GLubyte) c;
            data[i][j][1] = (GLubyte) c;
            data[i][j][2] = (GLubyte) c;
            data[i][j][3] = (GLubyte) 255; 
        }
    }


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned int texture;
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, patterns, patterns, 0,  GL_RGBA, GL_UNSIGNED_BYTE, data);
     
    glBindTexture(GL_TEXTURE_2D, 0);  
  
    printf("\nCREATED texture id = %i", texture);
    return texture;
}

unsigned int GraphicsGenerateUniformTexture(int patterns, int value)
{ 
    patterns = 32;

    GLubyte data[32][32][4];
    

    for (int i = 0; i < patterns; i++)
    {
        for (int j = 0; j < patterns; j++)
        { 

            data[i][j][0] = (GLubyte) value;
            data[i][j][1] = (GLubyte) value;
            data[i][j][2] = (GLubyte) value;
            data[i][j][3] = (GLubyte) 255; 
        }
    }


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned int texture;
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, patterns, patterns, 0,  GL_RGBA, GL_UNSIGNED_BYTE, data);
     
    
    printf("\nCREATED texture id = %i", texture);

    glBindTexture(GL_TEXTURE_2D, 0);  
    return texture;
}



unsigned int GraphicsGenerateTextureFromImage(char *data, int width, int height)
{ 
    unsigned int texture; 
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,  GL_RGBA , GL_UNSIGNED_BYTE,  data);

     

    printf("\nCREATED texture id = %i", texture);

    return texture;
}