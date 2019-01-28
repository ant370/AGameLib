
#include "Graphics.h"
#include "Math.h"
#include "glew.h"
#include <stdio.h>


void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{

  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void GraphicsCreateGBuffer(GraphicsScene *scene)
{
    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    unsigned int gPosition, gNormal, gAlbedoSpec;
    
    // - position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scene->screenWwidth, scene->screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    
    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scene->screenWwidth, scene->screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    
    // - color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scene->screenWwidth, scene->screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    
    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
}
 

void GraphicsRenderCreateShadowBuffer(GraphicsScene *scene)
{
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);  

    const unsigned int SHADOW_WIDTH = 800, SHADOW_HEIGHT = 800;
    scene->shadowBufferHeight = 800;
    scene->shadowBufferWidth = 800;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  



    scene->shadowBuffer = depthMapFBO;
    scene->shadowBufferTexture = depthMap;
    
    GraphicsRenderUseShadowBuffer(scene);
}

void GraphicsRenderUseShadowBuffer(GraphicsScene *scene)
{
    glBindFramebuffer(GL_FRAMEBUFFER, scene->shadowBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, scene->shadowBufferTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void GraphicsRenderShadows(GraphicsScene *scene)
{
 

    glBindFramebuffer(GL_FRAMEBUFFER, scene->shadowBuffer);

    glViewport(0, 0, scene->shadowBufferWidth, scene->shadowBufferHeight);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT); 

    Matrix lightView = MatrixLookAt(
        (Vec4) {0.0f, 0.0f, 0.0f, 0.0f},
        scene->directionalLight.direction,
        (Vec4) { 0.0f, 1.0f, 0.0f, 0.0f} );

    Matrix lightProjection = MatrixOrtho(-20.0f, 20.0f, 20.0f, -20.0f, 0.10f, 150.0f);

    
     
 
    for ( int i = 0; i < scene->objectsCount; i++)
    {
        GraphicsObject *obj = scene->objects[i];
        
        Matrix rotation = MatrixRotation(obj->rotation, obj->rotationAngle);
        Matrix scale = MatrixScale(obj->scale.x, obj->scale.y, obj->scale.z);
        Matrix translate  = MatrixTranslation(obj->location.x, obj->location.y, obj->location.z);

        Matrix model = MatrixMul(translate, rotation);
        model = MatrixMul(model, scale);
 

        glUseProgram(scene->shadowProgram.programId);
    
        GraphicsShaderSetUniformMatrix(scene->shadowProgram.programId, "project", lightProjection);
        GraphicsShaderSetUniformMatrix(scene->shadowProgram.programId, "view", lightView);
        GraphicsShaderSetUniformMatrix(scene->shadowProgram.programId, "model", model);
 
        glBindVertexArray(obj->vao);
        glDrawElements(GL_TRIANGLES, obj->geom->numTris * 3, GL_UNSIGNED_INT,0); 
  
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsRenderDepthTexture(GraphicsScene *scene)
{
    glUseProgram(scene->depthShaderProgram.programId); 
    glUniform1i(glGetUniformLocation(scene->depthShaderProgram.programId, "depthMap"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scene->shadowBufferTexture);
    renderQuad();
}


void GraphicsRenderTexture(GraphicsScene *scene, unsigned int texture)
{
    
    glUseProgram(scene->depthShaderProgram.programId); 
    glUniform1i(glGetUniformLocation(scene->depthShaderProgram.programId, "depthMap"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderQuad();
}
 
void GraphicsRenderScene(GraphicsScene *scene)
{
    if(scene->shadowBuffer == -1)
    {
        GraphicsRenderCreateShadowBuffer(scene);
    }

    //Render Shadows  
    GraphicsRenderShadows(scene);  

    //glEnable(GL_FRAMEBUFFER_SRGB); 
    glViewport(0, 0, 1200,900);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );
    glEnable(GL_FRAMEBUFFER_SRGB); 
    glViewport(0, 0, 1200,900);
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
    Matrix view  = MatrixLookAt(  scene->cameraLookat, scene->cameraLocation,
                       (Vec4) {0.0f,1.0f,0.0f,0.0f});
      
    Matrix projection = MatrixProj(1.0f, 100.0f, 900.0f/1200.0f, 60.0f);

    //lightspace
    
    Matrix lightView = MatrixLookAt(
        (Vec4) {0.0f, 0.0f, 0.0f, 0.0f},
        scene->directionalLight.direction,
        (Vec4) { 0.0f, 1.0f, 0.0f, 0.0f} );

    Matrix lightProjection = MatrixOrtho(-20.0f, 20.0f, 20.0f, -20.0f, 0.10f, 150.0f);

    float aspect = 1200.0f/900.0f;

    projection = MatrixOrtho(-50.0f * aspect,  50.0f* aspect,  50.0f, -50.0f, 0.10f, 150.0f);


    for ( int i = 0; i < scene->objectsCount; i++)
    {
        GraphicsObject *obj = scene->objects[i];
        
        Matrix rotation = MatrixRotation(obj->rotation, obj->rotationAngle);
        Matrix scale = MatrixScale(obj->scale.x, obj->scale.y, obj->scale.z);
        Matrix translate  = MatrixTranslation(obj->location.x, obj->location.y, obj->location.z);

        Matrix model = MatrixMul(translate, rotation);
        model = MatrixMul(model, scale);

        // Accept fragment if it closer to the camera than the former one 
        glUseProgram(obj->material->shader);

        GraphicsShaderSetUniformMatrix(obj->material->shader, "project", projection);
        GraphicsShaderSetUniformMatrix(obj->material->shader, "transform", view);
        GraphicsShaderSetUniformMatrix(obj->material->shader, "model", model);

        
        GraphicsShaderSetUniformMatrix(obj->material->shader, "lightProject", lightProjection);
        GraphicsShaderSetUniformMatrix(obj->material->shader, "lightView", lightView);

        GraphicsShaderSetUniformVec4(obj->material->shader, "directLightPos", scene->directionalLight.direction);
        GraphicsShaderSetUniformVec4(obj->material->shader, "directLightColor", scene->directionalLight.color);
        GraphicsShaderSetUniformVec4(obj->material->shader, "cameraPos", scene->cameraLocation);


        for ( int i = 0; i < 16; i++)
        {
            char buffer[1000];
            snprintf(buffer, 1000,"pointLights[%i].position", i);
            GraphicsShaderSetUniformVec3(obj->material->shader, buffer, scene->pointLights[i].position);
            
            snprintf(buffer, 1000,"pointLights[%i].color", i);
            GraphicsShaderSetUniformVec3(obj->material->shader, buffer, scene->pointLights[i].color);
        
        }
        //GraphicsShaderSetUniformVec3(obj->material->shader, "pointLights[0].position", scene->pointLights[0].position);
        //GraphicsShaderSetUniformVec3(obj->material->shader, "pointLights[0].color", scene->pointLights[0].color);
        //GraphicsShaderSetUniformVec3(obj->material->shader, "pointLights[1].position", scene->pointLights[1].position);
        //GraphicsShaderSetUniformVec3(obj->material->shader, "pointLights[1].color", scene->pointLights[1].color);
 
        
        glUniform1i(glGetUniformLocation(obj->material->shader, "ourTexture"), 0);
        glUniform1i(glGetUniformLocation(obj->material->shader, "shadowMap"), 1);
         
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->material->albido.textureId);  
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, scene->shadowBufferTexture); 

        glBindVertexArray(obj->vao);
        glDrawElements(GL_TRIANGLES, obj->geom->numTris * 3, GL_UNSIGNED_INT,0); 
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);   
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0); 

 
    }
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
