
#include "Graphics.h"
#include "Math.h"
#include "glew.h"
#include "Resource.h"
#include <stdio.h>
#include "IO.h"
 
 unsigned int GraphicsCreateVBO()
 {
     unsigned int VBO;
     glGenBuffers(1, &VBO);
     return VBO;
 }

 unsigned int GraphicsVBOTransfer(unsigned int vbo,int size, VertexPositionTexture * verts)
 {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size , verts, GL_STATIC_DRAW); 
 }

 GraphicsShaderProgram GraphicsCreateShader(char *vertexSource, char *fragSource)
 {
 
    GLint error;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    //Get error output for vertex shader 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &error); 
    if(!error)
    {
        //glGetShaderInfoLog(vertexShader, 512, NULL, program.errorVert); 
        return;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, NULL);
    glCompileShader(fragmentShader);
    
    //Get error output for fragment shader 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &error); 
    if(!error)
    {
        //glGetShaderInfoLog(vertexShader, 512, NULL, program.errorFrag); 
        return;
    }

    //Compile Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    GraphicsShaderProgram program; 
    program.fragmentShader = fragmentShader;
    program.vertexShader = vertexShader;
    program.programId = shaderProgram;

    return program;    
}

unsigned int GraphicsCreateVAO(int size, VertexPositionTexture * verts)
{  
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    GraphicsVBOTransfer(GraphicsCreateVBO(), size, verts);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4));

    glEnableVertexAttribArray(0);  

    return VAO;
}
 
 
unsigned int GraphicsCreateVAOFromGeom(GraphicsGeom * geom)
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  

    //Normals 

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    GraphicsVBOTransfer(GraphicsCreateVBO(), sizeof(VertexPositionTexture) * geom->numVerts, geom->verts);

    //Elements
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * geom->numTris * 3 , geom->index, GL_STATIC_DRAW); 

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)( 4 * sizeof(float))); 
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)( 8 * sizeof(float))); 
    glEnableVertexAttribArray(2);  
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)( 12 * sizeof(float))); 
    glEnableVertexAttribArray(3);  

    return VAO;
}
 

void GraphicsShaderSetUniformFloat(unsigned int program, char *name, float val )
{
    glUniform1f(glGetUniformLocation(program, name), val);
}

void GraphicsShaderSetUniformMatrix(unsigned int program, char *name, Matrix m )
{ 
    glUniformMatrix4fv(glGetUniformLocation(program, name),1, GL_FALSE, (float *)&m);
}


void GraphicsShaderSetUniformVec4(unsigned int program, char *name, Vec4 vec )
{  
    glUniform4f(glGetUniformLocation(program, name),vec.x, vec.y,vec.z,vec.w );
}

void GraphicsShaderSetUniformVec3(unsigned int program, char *name, Vec4 vec )
{  
    glUniform3f(glGetUniformLocation(program, name),vec.x, vec.y,vec.z );
}


float mapToScreen(float x, float d)
{
    return ( (-d+2*x)/d );
}

void Graphics2DDrawTexture(GraphicsScene *scene, unsigned int texture, Rectangle from, Rectangle to)
{ 

    to.x = mapToScreen(to.x, scene->screenWwidth); 
    to.y = -mapToScreen(to.y, scene->screenHeight);
    to.width = 2.0f * to.width/scene->screenWwidth; 
    to.height = -2.0f * to.height/scene->screenHeight;

     

    float data[] = {
        to.x,               to.y,               from.x,                 from.y,
        to.x + to.width,    to.y,               from.x + from.width,     from.y,
        to.x + to.width,    to.y + to.height,   from.x + from.width,    from.y + from.height,
        
        to.x,               to.y,               from.x,                 from.y,
        to.x + to.width,    to.y + to.height,   from.x + from.width,    from.y + from.height,
        to.x ,              to.y + to.height,   from.x ,                from.y + from.height
    };

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
   
    glDisable(GL_DEPTH_TEST);

    glUseProgram(scene->basic2dProgram.programId);  
    glUniform1i(glGetUniformLocation(scene->basic2dProgram.programId, "texs"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data) , &data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); 
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); 
 
    //Clean up
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glEnable(GL_DEPTH_TEST);
 
}
 
void Graphics2DDrawString(GraphicsScene *scene, unsigned int texture, GameResource *font, char * str)
{
     // assume orthographic projection with units = screen pixels, origin at top left
   /* glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, ftex);
   glBegin(GL_QUADS);
   while (*text) {
      if (*text >= 32 && *text < 128) {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
         glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y0);
         glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y0);
         glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y1);
         glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y1);
      }
      ++text;
   }
   glEnd(); */
    float x = 0.0f;
    float y = 0.0f;
   while(*str)
   {
        if (*str >= 32 && *str < 128) 
        {
           
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font->info.font.data, 512,512, *str-32, &x,&y,&q,1);
            Rectangle from;
            Rectangle to;
            to.x = q.x0;
            to.y = q.y0 + 50;
            to.width = q.x1 - q.x0;
            to.height = q.y1 - q.y0;

            from.x = q.s0;
            from.y = q.t0;
            from.width = q.s1 - q.s0;
            from.height = q.t1 - q.t0;


            Graphics2DDrawTexture(scene, texture, from, to);
        }
       ++str;
   }
}