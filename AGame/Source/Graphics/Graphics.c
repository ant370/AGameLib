
#include "Graphics.h"
#include "Math.h"
#include "glew.h"
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
    GraphicsShaderProgram program;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    //Get error output for vertex shader 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &program.errorVertFlag); 
    if(!program.errorVertFlag)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, program.errorVert); 
        return;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, NULL);
    glCompileShader(fragmentShader);
    
    //Get error output for fragment shader 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &program.errorFragFlag); 
    if(!program.errorFragFlag)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, program.errorFrag); 
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
    if(!success)
    { 
        glGetProgramInfoLog(shaderProgram, 512, NULL, program.errorProgram);
        printf(program.errorProgram);

    }


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

    return VAO;
}

float l = 0.1f;
unsigned int tex = -1;

void createAndDrawTriangle()
{ 

    GraphicsGeom *geom = GraphicsGeomCreateTriangle(); 
    geom = GraphicsGeomSphere(3);

	char * v = IOLoadFile("Shaders/vert.glsl");
	char * f = IOLoadFile("Shaders/frag.glsl");

    if (tex == -1)
    {
        tex = GraphicsGenerateCheckerTexture(10);
    }


    l+= 0.1f;
    Matrix m = MatrixId(); 

    m = MatrixLookAt( (Vec4) {0.0f,0.0f,0.0f,0.0f},
           (Vec4)  { 4.0f * sin(0.1f *l), 3.0f* cos(0.1f *l), 1.0f  ,0.0f},
                       (Vec4) {0.0f,1.0f,0.0f,0.0f});
      
    Matrix prof = MatrixProj(1.0f, 10000.0f, 1.0f, 45.0f);
 
    glViewport(0,0,800,800);
  

    GraphicsShaderProgram prog = GraphicsCreateShader(v,f);
 
    unsigned int vao = GraphicsCreateVAOFromGeom(geom);

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS); 
    glClearDepthf(1.0f); 
    glDisable(GL_CULL_FACE);

    // Accept fragment if it closer to the camera than the former one 
    glUseProgram(prog.programId);
    glBindTexture(GL_TEXTURE_2D, tex);

    GraphicsShaderSetUniformMatrix(prog.programId, "project", m);
    GraphicsShaderSetUniformMatrix(prog.programId, "transform", prof);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, geom->numTris * 3, GL_UNSIGNED_INT,0); 

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
 