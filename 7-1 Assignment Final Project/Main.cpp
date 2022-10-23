#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <vector>           // import the vector type
#include <map>              // map
#include <string>

#define STB_IMAGE_IMPLEMENTATION  // required for stb_image.h
#include <stb_image.h>      // image loading header

#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std; // Standard namespace

constexpr auto COLOR_MAP_SIZE = 6;

// Unnamed namespace
namespace
{
    glm::vec3 gColorMap[COLOR_MAP_SIZE] = {
            glm::vec3(1.0f, 0.0f, 0.0f), // red
            glm::vec3(0.0f, 0.0f, 1.0f), // blue
            glm::vec3(0.0f, 1.0f, 1.0f), // cyan
            glm::vec3(0.0f, 1.0f, 0.0f), // green
            glm::vec3(1.0f, 1.0f, 0.0f), // yellow
            glm::vec3(1.0f, 0.0f, 1.0f), // magenta
    };

    enum WindowProjection {
        Perspective = 0,
        Orthographic = 1
    };

    struct WindowParams {
        string Title = "Assignment 7-1 Cory Remick";
        GLuint Width = 800;
        GLuint Height = 600;
        GLFWwindow* windowPtr = nullptr;  // handle to window pointer
        glm::mat4 Projection;  // projection matrix
        WindowProjection ProjectionMode = WindowProjection::Perspective;
    };

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao                      = 0;                     // Handle for the vertex array object
        GLuint vbos[7]                  = {0, 0, 0, 0, 0, 0, 0}; // Handles for the vertex buffer objects to hold shape data
        // vertex data 
        GLuint Mode = GL_TRIANGLES;                 // Default Draw mode, can be overriden in call to draw
        GLuint AttributeDataType = GL_FLOAT;	    // type of data points, i.e. GL_FLOAT
        GLuint nTorusVertices           = 0;        // Number of vertices in Torus buffer
        GLuint nCylinderSideVertices    = 0;        // Number of vertices in Cylinder sides
        GLuint nCylinderTopOrBottonVertices = 0;    // Number of vertices in Top or Botton cover
        GLuint nPlaneVertices       = 0;            // Number of vertices in the plane
        GLuint MatchBoxVertices     = 0;            // Number of vertices in the matchbox
        GLuint CandleBoxVertices    = 0;            // Number of vertices in the candle box
        GLuint CandleCylinderSideVertices           = 0;   // Number of vertices in the candle cylinder
        GLuint CandleCylinderTopOrBottomVertices    = 0;   // Number of vertices in the candle cylinder
        GLuint SprayBaseCylinderTopOrBottomVertices = 0;   // Number of vertices in the candle cylinder
        GLuint SprayBaseCylinderSideVertices        = 0;   // Number of vertices in the candle cylinder
        GLuint SprayTopCylinderTopOrBottomVertices  = 0;   // Number of vertices in the candle cylinder
        GLuint SprayTopCylinderSideVertices         = 0;   // Number of vertices in the candle cylinder
        GLuint ValuesPerVertex      = 3;	        // number of values per vertex attribute
        GLuint ValuesPerColor       = 3;	        // number of values per color attribute
        GLuint ValuesPerTexture     = 2;	        // number of values per texture attribute
        GLuint ValuesPerNormal      = 3;	        // number of values per normal attribute
        // storing different strides allow switching in attributes one shape at a time
        GLuint PlaneStride              = 11 * sizeof(GLfloat);        // length in bytes between vertices
        GLuint TorusStride              = 11 * sizeof(GLfloat);        // length in bytes between vertices
        GLuint CylinderStride           = 11 * sizeof(GLfloat);        // length in bytes between vertices
        GLuint CandleBoxStride          = 11 * sizeof(GLfloat);        // length in bytes between vertices
        GLuint MatchBoxStride           = 11 * sizeof(GLfloat);        // length in bytes between vertices
        GLuint CandleCylinderStride     = 11 * sizeof(GLfloat);        // length in bytes between vertices
        GLuint SprayCylinderStride      = 11 * sizeof(GLfloat);        // length in bytes between vertices
        // texture info
        GLuint CandleHolderTextureId    = 0;	// OpenGL Id of Texture
        GLuint CandleCylinderTextureId  = 0;	// OpenGL Id of Texture
        GLuint CandleTextureId          = 0;	// OpenGL Id of Texture
        GLuint NewsPaperTextureId       = 0;	// OpenGL Id of Texture
        GLuint CandleBoxTextureId       = 0;	// OpenGL Id of Texture
        GLuint MatchBoxTextureId        = 0;	// OpenGL Id of Texture
        GLuint SprayCylinderTextureId   = 0;    // OpenGL Id of Texture
        int SprayBaseCylinderTextureWidth       = 0;	// Texture Width
        int SprayBaseCylinderTextureHeight      = 0;	// Texture Height
        int SprayBaseCylinderTextureChannels    = 0;	// Texture Channels
        int SprayTopCylinderTextureWidth    = 0;	// Texture Width
        int SprayTopCylinderTextureHeight   = 0;	// Texture Height
        int SprayTopCylinderTextureChannels = 0;	// Texture Channels
        int CandleCylinderTextureWidth    = 0;	// Texture Width
        int CandleCylinderTextureHeight   = 0;	// Texture Height
        int CandleCylinderTextureChannels = 0;	// Texture Channels
        int CandleHolderTextureWidth    = 0;	// Texture Width
        int CandleHolderTextureHeight   = 0;	// Texture Height
        int CandleHolderTextureChannels = 0;	// Texture Channels
        int CandleBoxTextureWidth       = 0;	// Texture Width
        int CandleBoxTextureHeight      = 0;	// Texture Height
        int CandleBoxTextureChannels    = 0;	// Texture Channels
        int MatchBoxTextureWidth        = 0;	// Texture Width
        int MatchBoxTextureHeight       = 0;	// Texture Height
        int MatchBoxTextureChannels     = 0;	// Texture Channels
        int CandleTextureWidth          = 0;	// Texture Width
        int CandleTextureHeight         = 0;	// Texture Height
        int CandleTextureChannels       = 0;	// Texture Channels
        int NewsPaperTextureWidth       = 0;	// Texture Width
        int NewsPaperTextureHeight      = 0;	// Texture Height
        int NewsPaperTextureChannels    = 0;	// Texture Channels
        // shading programs, different programs can be applied to different shapes
        Shader* defaultProgram = nullptr;
        Shader* textureProgram = nullptr;
        Shader* lightingProgram = nullptr;
    };

    struct CameraParams {
        glm::vec3 Position = glm::vec3(2.07f, 24.02f, -24.52f);
        glm::vec3 Front = glm::vec3(-0.018f, -0.67f, 0.74f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        float Pitch = -42.0f;
        float Yaw = 91.0f;
        float Fov = 45.0f; // in degrees
        float CameraSpeed = 12.5f;
    };

    struct Time {
        float DeltaTime = 0.0;
        float LastTime = 0.0;
    };

    WindowParams gWindow; // Main GLFW window parameters
    CameraParams gCamera; // camera
    Time gTime;           // time data

    struct MouseParams {
        float LastX = (float)gWindow.Width / 2;
        float LastY = (float)gWindow.Height / 2;
        bool FirstMouse = true;
    };

    MouseParams gMouse;   // mouse data
}

/* User-defined Function prototypes to:
*  initialize the program, set the window size,
*  redraw graphics on the window when resized,
*  and render graphics on the screen
*/
bool UInitialize(WindowParams params, GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);
void URender(GLMesh mesh);
void UMouse(GLFWwindow* window, double xpos, double ypos);
void UScroll(GLFWwindow* window, double xoffset, double yoffset);
void UCreateTorus(GLMesh& mesh, const GLfloat torusRadius, const GLfloat tubeRadius, const GLuint torusSegments, const GLuint tubePoints);
void UCreateCylinderSides(const GLuint& cylinderSegments, const float& cylinderSegmentAngleStep, const GLfloat& cylinderRadius, const GLfloat& cylinderHeight, std::vector<GLfloat>& cylinderVertices, const GLfloat& textureXStep);
void UCreateCylinderTop(std::vector<GLfloat>& cylinderVertices, const GLfloat& cylinderHeight, const GLuint& cylinderSegments, const float& cylinderSegmentAngleStep, const GLfloat& cylinderRadius, const GLfloat& textureXStep);
void UCreateCylinderBottom(std::vector<GLfloat>& cylinderVertices, const GLfloat& cylinderHeight, const GLuint& cylinderSegments, const float& cylinderSegmentAngleStep, const GLfloat& cylinderRadius, const GLfloat& textureXStep);
void ULoadTexture(std::string path, GLuint& textureId, int& textureWidth, int& textureHeight, int& textureChannels);
void DrawSurface(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);
void DrawCandleHolders(GLMesh& mesh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);
void DrawVotiveCandles(GLMesh& mesh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);
void DrawCandleBox(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);
void DrawMatchBox(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);
void DrawCandleCylinder(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);
void DrawSprayCylinder(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat specularIntensity);

// Functioned called to render a frame
void URender(GLMesh mesh)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transforms the camera: move the camera 
    glm::mat4 view = glm::lookAt(
        gCamera.Position,
        gCamera.Position + gCamera.Front,
        gCamera.Up
    );

    glBindVertexArray(mesh.vao);

    // draw newspaper surface upon which the other objects rest
    DrawSurface(mesh, view, gWindow.Projection, gCamera.Position, 0.1f, 0.4f, 2.0f);
    
    // draw box 
    DrawCandleBox(mesh, view, gWindow.Projection, gCamera.Position, 0.2f, 0.1f, 2.0f);
    
    // 1. Scales the object
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    // 2. Rotates shape
    glm::mat4 rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    glm::mat4 translation = glm::translate(glm::vec3(1.0f, 3.0f, 0.3f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    DrawCandleHolders(mesh, model, view, gWindow.Projection, gCamera.Position, 0.1f, 0.8f, 32.0f);

    // no votive inside center holder

    // 1. Scales the object
    scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    // 2. Rotates shape
    rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    translation = glm::translate(glm::vec3(11.0f, 3.0f, 0.3f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    DrawCandleHolders(mesh, model, view, gWindow.Projection, gCamera.Position, 0.1f, 0.8f, 32.0f);

    DrawVotiveCandles(mesh, model, view, gWindow.Projection, gCamera.Position, 0.1f, 0.8f, 32.0f);

    // 1. Scales the object
    scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    // 2. Rotates shape
    rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    translation = glm::translate(glm::vec3(-9.0f, 3.0f, 0.3f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // draw the glass candle holders
    DrawCandleHolders(mesh, model, view, gWindow.Projection, gCamera.Position, 0.1f, 0.8f, 32.0f);

    DrawVotiveCandles(mesh, model, view, gWindow.Projection, gCamera.Position, 0.1f, 0.8f, 32.0f);

    DrawMatchBox(mesh, view, gWindow.Projection, gCamera.Position, 0.1f, 0.8f, 32.0f);

    DrawCandleCylinder(mesh, view, gWindow.Projection, gCamera.Position, 0.1f, 1.0f, 128.0f);

    DrawSprayCylinder(mesh, view, gWindow.Projection, gCamera.Position, 0.1f, 1.0f, 128.0f);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow.windowPtr);    // Flips the the back buffer with the front buffer every frame.
}

void DrawCandleHolders(GLMesh& mesh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // Activate the VBOs contained within the mesh's VAO
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[1]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, 0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)(mesh.ValuesPerVertex * sizeof(float)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(float)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(float)));
    glEnableVertexAttribArray(0);     // position
    glEnableVertexAttribArray(1);     // color
    glEnableVertexAttribArray(2);     // texture
    glEnableVertexAttribArray(3);     // normal

    // activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.CandleHolderTextureId); // bind texture id to render unit

    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(10.0f, 5.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.0f);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh.nTorusVertices);

    // deactivate program, texture, and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawVotiveCandles(GLMesh& mesh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // activate vbo 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[2]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.CylinderStride, (GLvoid*)0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.CylinderStride, (GLvoid*)((mesh.ValuesPerVertex) * sizeof(GLfloat)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.CylinderStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(GLfloat)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);  // position
    glEnableVertexAttribArray(1);  // color
    glEnableVertexAttribArray(2);  // texture
    glEnableVertexAttribArray(3);  // normal

    //activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.CandleTextureId); // bind texture id to render unit

    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position  

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // activate fill light for candle, key light made surface look wrong
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(10.0f, 5.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 1.0f, 1.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 1.0f);

    // cylinder sides
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh.nCylinderSideVertices); // Draws the triangle

    // cylinder top
    glDrawArrays(GL_TRIANGLE_FAN, mesh.nCylinderSideVertices, mesh.nCylinderTopOrBottonVertices); // Draws the triangle

    // cylinder bottom
    glDrawArrays(GL_TRIANGLE_FAN, mesh.nCylinderSideVertices + mesh.nCylinderTopOrBottonVertices, mesh.nCylinderTopOrBottonVertices); // Draws the triangle

    // deactivate program, texture, and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawCandleBox(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // 1. Scales the object
    glm::mat4 scale = glm::scale(glm::vec3(2.5f, 2.0f, 2.5f));
    // 2. Rotates shape
    glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    glm::mat4 translation = glm::translate(glm::vec3(-15.3f, -1.5f, -6.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // activate vbo 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[3]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, 0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)(mesh.ValuesPerVertex * sizeof(float)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(float)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(float)));
    glEnableVertexAttribArray(0);     // position
    glEnableVertexAttribArray(1);     // color
    glEnableVertexAttribArray(2);     // texture
    glEnableVertexAttribArray(3);     // normal

    // activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.CandleBoxTextureId); // bind texture id to render unit

    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(10.0f, 5.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.0f);

    // draw shape
    glDrawArrays(GL_TRIANGLES, 0, mesh.CandleBoxVertices);

    // deactivate program, texture and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawMatchBox(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // 1. Scales the object
    glm::mat4 scale = glm::scale(glm::vec3(2.6f, 1.5f, 3.0f));
    // 2. Rotates shape
    glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // 3. move object
    glm::mat4 translation = glm::translate(glm::vec3(10.0f, -1.5f, 10.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // activate vbo 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[4]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, 0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)(mesh.ValuesPerVertex * sizeof(float)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(float)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(float)));
    glEnableVertexAttribArray(0);     // position
    glEnableVertexAttribArray(1);     // color
    glEnableVertexAttribArray(2);     // texture
    glEnableVertexAttribArray(3);     // normal

    // activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.MatchBoxTextureId); // bind texture id to render unit

    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position
 
    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(10.0f, 5.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.0f);


    // draw shape
    glDrawArrays(GL_TRIANGLES, 0, mesh.MatchBoxVertices);

    // deactivate program, texture and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawCandleCylinder(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // 1. Scales the object
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 2.0f, 3.0f));
    // 2. Rotates shape
    glm::mat4 rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 13.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.CandleCylinderTextureId); // bind texture id to render unit

    // activate vbo 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[5]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, 0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)(mesh.ValuesPerVertex * sizeof(float)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(float)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(float)));
    glEnableVertexAttribArray(0);     // position
    glEnableVertexAttribArray(1);     // color
    glEnableVertexAttribArray(2);     // texture
    glEnableVertexAttribArray(3);     // normal

    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(0.0f, 7.0f, 11.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.3f);

    // draw shape
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh.CandleCylinderSideVertices);

    // cylinder top
    glDrawArrays(GL_TRIANGLE_FAN, mesh.CandleCylinderSideVertices, mesh.CandleCylinderTopOrBottomVertices); // Draws the triangle

    // cylinder bottom
    glDrawArrays(GL_TRIANGLE_FAN, mesh.CandleCylinderSideVertices + mesh.CandleCylinderTopOrBottomVertices, mesh.CandleCylinderTopOrBottomVertices); // Draws the triangle

    // deactivate program, texture and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawSprayCylinder(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // 1. Scales the object
    glm::mat4 scale = glm::scale(glm::vec3(0.8f, 0.8f, 6.0f));
    // 2. Rotates shape
    glm::mat4 rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    glm::mat4 translation = glm::translate(glm::vec3(-10.0f, 1.5f, 10.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.SprayCylinderTextureId); // bind texture id to render unit

    // activate vbo 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[5]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, 0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)(mesh.ValuesPerVertex * sizeof(float)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(float)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(float)));
    glEnableVertexAttribArray(0);     // position
    glEnableVertexAttribArray(1);     // color
    glEnableVertexAttribArray(2);     // texture
    glEnableVertexAttribArray(3);     // normal
 
    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(-10.0f, 8.0f, 10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.3f);

    GLint offset = 0;

    // draw shape
    glDrawArrays(GL_TRIANGLE_STRIP, offset, mesh.CandleCylinderSideVertices);      // Draws the triangle

    offset = mesh.CandleCylinderSideVertices;

    // cylinder top
    glDrawArrays(GL_TRIANGLE_FAN, offset, mesh.CandleCylinderTopOrBottomVertices); // Draws the triangle

    offset += mesh.CandleCylinderTopOrBottomVertices;

    // cylinder bottom
    glDrawArrays(GL_TRIANGLE_FAN, offset, mesh.CandleCylinderTopOrBottomVertices); // Draws the triangle

    offset += mesh.CandleCylinderTopOrBottomVertices;


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // 1. Scales the object
    scale = glm::scale(glm::vec3(0.5f, 0.5f, 2.5f));
    // 2. Rotates shape
    rotation = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    translation = glm::translate(glm::vec3(-10.0f, 5.8f, 10.0f));
    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(10.0f, 5.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.0f);

    offset = 0;

    // draw shape
    glDrawArrays(GL_TRIANGLE_STRIP, offset, mesh.CandleCylinderSideVertices);      // Draws the triangle

    offset = mesh.CandleCylinderSideVertices;

    // cylinder top
    glDrawArrays(GL_TRIANGLE_FAN, offset, mesh.CandleCylinderTopOrBottomVertices); // Draws the triangle

    offset += mesh.CandleCylinderTopOrBottomVertices;

    // cylinder bottom
    glDrawArrays(GL_TRIANGLE_FAN, offset, mesh.CandleCylinderTopOrBottomVertices); // Draws the triangle

    offset += mesh.CandleCylinderTopOrBottomVertices;

    glPopMatrix();

    // deactivate program, texture, and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void DrawSurface(GLMesh& mesh, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPos, const GLfloat ambientStrength, const GLfloat specularStrength, const GLfloat highlightSize)
{
    // 1. Scales the object
    glm::mat4 scale = glm::scale(glm::vec3(30.0f, 1.0f, 20.0f));
    // 2. Rotates shape
    glm::mat4 rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 3. move object
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, -1.5f, 0.0f));
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // activate vbo 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
    glVertexAttribPointer(0, mesh.ValuesPerVertex, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, 0);
    glVertexAttribPointer(1, mesh.ValuesPerColor, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)(mesh.ValuesPerVertex * sizeof(float)));
    glVertexAttribPointer(2, mesh.ValuesPerTexture, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor) * sizeof(float)));
    glVertexAttribPointer(3, mesh.ValuesPerNormal, mesh.AttributeDataType, GL_FALSE, mesh.PlaneStride, (GLvoid*)((mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture) * sizeof(float)));
    glEnableVertexAttribArray(0);     // position
    glEnableVertexAttribArray(1);     // color
    glEnableVertexAttribArray(2);     // texture
    glEnableVertexAttribArray(3);     // normal

    // activate texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.NewsPaperTextureId); // bind texture id to render unit

    // activate shader program
    mesh.lightingProgram->use();
    mesh.lightingProgram->setModelMatrix(model);
    mesh.lightingProgram->setViewMatrix(view);
    mesh.lightingProgram->setProjectionMatrix(gWindow.Projection);
    mesh.lightingProgram->setTextureUnit(0);

    mesh.lightingProgram->setUniformValue("ambientStrength", ambientStrength);      // ambient lighting strength
    mesh.lightingProgram->setUniformValue("specularIntensity", specularStrength);	// specular lighting strength
    mesh.lightingProgram->setUniformValue("highlightSize", highlightSize);	        // specular highlight power
    mesh.lightingProgram->setUniformValue("viewPosition", cameraPos);	            // view position

    // create diffuse lights
    // key light 
    //100% yellow 255, 214, 170
    mesh.lightingProgram->setUniformValue("diffLights[0].position", glm::vec3(10.0f, 25.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[0].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[0].intensity", 1.0f);

    // fill light, not used intensity zero
    mesh.lightingProgram->setUniformValue("diffLights[1].position", glm::vec3(10.0f, 5.0f, -10.0f));
    mesh.lightingProgram->setUniformValue("diffLights[1].color", glm::vec3(1.0f, 0.839215686f, 0.666666667f));
    mesh.lightingProgram->setUniformValue("diffLights[1].intensity", 0.0f);

    // draw shape
    glDrawArrays(GL_TRIANGLES, 0, mesh.nPlaneVertices);

    // deactivate program, texture, and vbo
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// create torus
// number of segments to draw around torus
// number of points around tube
// radius (R) of torus
// radius (r) of tube around torus
void UCreateTorus(GLMesh& mesh, const GLfloat torusRadius, const GLfloat tubeRadius, const GLuint torusSegments, const GLuint tubePoints) {

    // A torus is given by the paramteric equations:
    // x = (R + r cos(v))cos(u)
    // y = (R + r cos(v))sin(u)
    // z = r sin(v)
    // u, v => [0, 2pi]

    const float torusSegmentAngleStep = glm::two_pi<float>() / torusSegments;   // calculate angle in radians to increment by
    const float tubeAngleStep = glm::two_pi<float>() / tubePoints;             // calculate angle in radians to increment by

    // Position, Color, texture data
    vector<GLfloat> torusVertices;  // temporary buffer to hold vertex attributes

    // create vertex position and color attributes
    for (size_t i = 0; i < torusSegments; i++) { // iterate segments around torus

        const float currentSegmentAngle = i * torusSegmentAngleStep;  // current angle (u) for around torus

        // calculate sine and cosine of main seqment angle
        for (size_t j = 0; j <= tubePoints; j++) { // iterate points around tube at segment
        
            const float currentTubeAngle = j * tubeAngleStep; // current angle (v) around tube

            const float sinTubePoint = sin(currentTubeAngle);
            const float cosTubePoint = cos(currentTubeAngle);

            for (size_t k = 0; k < 2; k++) { // for the triangle strip compute current and then next position

                const float u_2 = currentSegmentAngle + (k * torusSegmentAngleStep);
                const float x = (torusRadius + (tubeRadius * cosTubePoint)) * cos(u_2);
                const float y = (torusRadius + (tubeRadius * cosTubePoint)) * sin(u_2);
                const float z = tubeRadius * sinTubePoint;

                torusVertices.push_back(x); // position x
                torusVertices.push_back(y); // position y
                torusVertices.push_back(z); // position z

                // color isn't needed because of textures, only included for testing purposes
                const glm::vec3 color = gColorMap[j % COLOR_MAP_SIZE];

                torusVertices.push_back(color.r); // red
                torusVertices.push_back(color.g); // green
                torusVertices.push_back(color.b); // blue

                // texture coordinates
                // compute texture coords
                float textureXCoord = u_2 / glm::two_pi<float>();
                float textureYCoord = currentTubeAngle / glm::two_pi<float>();

                if (textureXCoord > 1.0f)
                    textureXCoord = 1.0f;
                if (textureYCoord > 1.0f)
                    textureYCoord = 1.0f;

                // add tex coords
                torusVertices.push_back(textureXCoord);
                torusVertices.push_back(textureYCoord);

                // compute normal 
                float normalX = cos(currentTubeAngle) * cos(u_2);
                float normalY = cos(currentTubeAngle) * sin(u_2);
                float normalZ = sin(currentTubeAngle);

                torusVertices.push_back(normalX);
                torusVertices.push_back(normalY);
                torusVertices.push_back(normalZ);
            }
        }
    }
    mesh.TorusStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.nTorusVertices = torusVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal); // number of torusVertices to render

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * torusVertices.size(), torusVertices.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // load texture 
    stbi_set_flip_vertically_on_load(true);
    ULoadTexture("Data\\pexels-hoang-le-978462.jpg", mesh.CandleHolderTextureId, mesh.CandleHolderTextureWidth, mesh.CandleHolderTextureHeight, mesh.CandleHolderTextureChannels);

    torusVertices.clear();
}

// create cylinder
void UCreateCylinder(GLMesh& mesh, const GLfloat cylinderRadius, const GLfloat cylinderHeight, const GLuint cylinderSegments) {
    vector<GLfloat> cylinderVertices;  // temporary buffer to hold both position and color data

    const auto cylinderSegmentAngleStep = (2 * glm::pi<float>()) / (float)cylinderSegments;  // calculate angle in radians to increment by
 
    GLfloat textureXStep = 1.0f / cylinderSegments;

    UCreateCylinderSides(cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, cylinderHeight, cylinderVertices, textureXStep);

    mesh.CylinderStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.nCylinderSideVertices = cylinderVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal); // number of torusVertices to render

    UCreateCylinderTop(cylinderVertices, cylinderHeight, cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, textureXStep);

    mesh.nCylinderTopOrBottonVertices = (cylinderVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal)) - mesh.nCylinderSideVertices; // number of Vertices to render

    UCreateCylinderBottom(cylinderVertices, cylinderHeight, cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, textureXStep);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cylinderVertices.size(), cylinderVertices.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU
    cylinderVertices.clear();

    ULoadTexture("Data\\white-texture-background.jpg", mesh.CandleTextureId, mesh.CandleTextureWidth, mesh.CandleTextureHeight, mesh.CandleTextureChannels);
}

void ULoadTexture(std::string path, GLuint& textureId, int& textureWidth, int& textureHeight, int& textureChannels) {
    unsigned char* image = stbi_load(path.c_str(), &textureWidth, &textureHeight, &textureChannels, 0);
    if (image != nullptr)
    {
        // gen texture buffer
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId); //activate buffer

        // wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // pixel filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (textureChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        }
        else if (textureChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        }

        // Release the image
        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void UCreateCylinderBottom(std::vector<GLfloat>& cylinderVertices, const GLfloat& cylinderHeight, const GLuint& cylinderSegments, const float& cylinderSegmentAngleStep, const GLfloat& cylinderRadius, const GLfloat& textureXStep)
{
    // add bottom cover

    // center bottom vertex
    cylinderVertices.push_back(0.0f);                   // x
    cylinderVertices.push_back(0.0f);                   // y
    cylinderVertices.push_back(cylinderHeight / 2.0f); // z  // I tried a positive Z for top but it ended up on bottom, so I swapped the signs

    cylinderVertices.push_back(gColorMap[0].r); // red
    cylinderVertices.push_back(gColorMap[0].g); // green
    cylinderVertices.push_back(gColorMap[0].b); // blue

    cylinderVertices.push_back(0.5f); // texture x
    cylinderVertices.push_back(0.5f); // texture y

    cylinderVertices.push_back(0.0f);   //normal x
    cylinderVertices.push_back(1.0f);   //normal y
    cylinderVertices.push_back(0.0f);   //normal z

    for (size_t i = 0; i <= cylinderSegments; i++) { // iterate segments around cylinder

        float currentSegmentAngle = i * cylinderSegmentAngleStep;

        float x = cos(currentSegmentAngle) * cylinderRadius;
        float y = sin(currentSegmentAngle) * cylinderRadius;
        float z = cylinderHeight / 2.0f; // I tried a positive Z for top but it ended up on bottom, so I swapped the signs

        cylinderVertices.push_back(x);
        cylinderVertices.push_back(y);
        cylinderVertices.push_back(z);

        glm::vec3 color = gColorMap[i % COLOR_MAP_SIZE];

        cylinderVertices.push_back(color.r); // red
        cylinderVertices.push_back(color.g); // green
        cylinderVertices.push_back(color.b); // blue

        cylinderVertices.push_back(textureXStep * i); // texture x
        cylinderVertices.push_back(0.0f); // texture y

        cylinderVertices.push_back(0.0f);   //normal x
        cylinderVertices.push_back(1.0f);   //normal y
        cylinderVertices.push_back(0.0f);   //normal z
    }
}

void UCreateCylinderTop(std::vector<GLfloat>& cylinderVertices, const GLfloat& cylinderHeight, const GLuint& cylinderSegments, const float& cylinderSegmentAngleStep, const GLfloat& cylinderRadius, const GLfloat& textureXStep)
{
    // add top cover

    // center top vertex
    cylinderVertices.push_back(0.0f);  // x
    cylinderVertices.push_back(0.0f);  // y
    cylinderVertices.push_back(-cylinderHeight / 2.0f); // z  // I tried a positive Z for top but it ended up on bottom, so I swapped the signs

    cylinderVertices.push_back(gColorMap[0].r); // red
    cylinderVertices.push_back(gColorMap[0].g); // green
    cylinderVertices.push_back(gColorMap[0].b); // blue

    cylinderVertices.push_back(0.5f); // texture x
    cylinderVertices.push_back(0.5f); // texture y

    cylinderVertices.push_back(0.0f);   //normal x
    cylinderVertices.push_back(-1.0f);   //normal y
    cylinderVertices.push_back(0.0f);   //normal z

    for (size_t i = 0; i <= cylinderSegments; i++) { // iterate segments around cylinder

        float currentSegmentAngle = i * cylinderSegmentAngleStep;

        float x = cos(currentSegmentAngle) * cylinderRadius;
        float y = sin(currentSegmentAngle) * cylinderRadius;
        float z = -cylinderHeight / 2.0f;  // I tried a positive Z for top but it ended up on bottom, so I swapped the signs

        cylinderVertices.push_back(x);
        cylinderVertices.push_back(y);
        cylinderVertices.push_back(z);

        glm::vec3 color = gColorMap[i % COLOR_MAP_SIZE];

        cylinderVertices.push_back(color.r); // red
        cylinderVertices.push_back(color.g); // green
        cylinderVertices.push_back(color.b); // blue

        cylinderVertices.push_back((GLfloat)(textureXStep * i)); // texture x
        cylinderVertices.push_back(1.0f); // texture y

        cylinderVertices.push_back(0.0f);   //normal x
        cylinderVertices.push_back(-1.0f);   //normal y
        cylinderVertices.push_back(0.0f);   //normal z
    }
}

void UCreateCylinderSides(const GLuint& cylinderSegments, const float& cylinderSegmentAngleStep, const GLfloat& cylinderRadius, const GLfloat& cylinderHeight, std::vector<GLfloat>& cylinderVertices, const GLfloat& textureXStep)
{
    // add cylinder sides
    for (size_t i = 0; i <= cylinderSegments; i++) { // iterate segments around cylinder

        glm::vec3 color = gColorMap[i % COLOR_MAP_SIZE];

        float currentSegmentAngle = i * cylinderSegmentAngleStep;

        float x = cos(currentSegmentAngle) * cylinderRadius;
        float y = sin(currentSegmentAngle) * cylinderRadius;
        float z_top = -cylinderHeight / 2.0f;
        float z_bottom = cylinderHeight / 2.0f;

        cylinderVertices.push_back(x);
        cylinderVertices.push_back(y);
        cylinderVertices.push_back(z_top);

        cylinderVertices.push_back(color.r); // red
        cylinderVertices.push_back(color.g); // green
        cylinderVertices.push_back(color.b); // blue

        cylinderVertices.push_back((GLfloat)(textureXStep * i));   // texture x
        cylinderVertices.push_back(1.0f);                  // texture y

        cylinderVertices.push_back(cos(currentSegmentAngle)); //normal x
        cylinderVertices.push_back(0.0f);   //normal y
        cylinderVertices.push_back(sin(currentSegmentAngle)); //normal z

        cylinderVertices.push_back(x);
        cylinderVertices.push_back(y);
        cylinderVertices.push_back(z_bottom);

        cylinderVertices.push_back(color.r); // red
        cylinderVertices.push_back(color.g); // green
        cylinderVertices.push_back(color.b); // blue

        cylinderVertices.push_back((GLfloat)(textureXStep * i)); // texture x
        cylinderVertices.push_back(0.0f); // texture y

        cylinderVertices.push_back(cos(currentSegmentAngle)); //normal x
        cylinderVertices.push_back(0.0f);   //normal y
        cylinderVertices.push_back(sin(currentSegmentAngle)); //normal z
    }
}

// create cylinder
void UCreateCandleCylinder(GLMesh& mesh, const GLfloat cylinderRadius, const GLfloat cylinderHeight, const GLuint cylinderSegments) {
    vector<GLfloat> cylinderVertices;  // temporary buffer to hold both position and color data

    const auto cylinderSegmentAngleStep = (2 * glm::pi<float>()) / (float)cylinderSegments;  // calculate angle in radians to increment by

    GLfloat textureXStep = 1.0f / cylinderSegments;

    UCreateCylinderSides(cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, cylinderHeight, cylinderVertices, textureXStep);

    mesh.CandleCylinderStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.CandleCylinderSideVertices = cylinderVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal); // number of torusVertices to render

    UCreateCylinderTop(cylinderVertices, cylinderHeight, cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, textureXStep);

    mesh.CandleCylinderTopOrBottomVertices = (cylinderVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal)) - mesh.CandleCylinderSideVertices; // number of Vertices to render

    UCreateCylinderBottom(cylinderVertices, cylinderHeight, cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, textureXStep);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cylinderVertices.size(), cylinderVertices.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU
    cylinderVertices.clear();

    ULoadTexture("Data\\copper.jpg", mesh.CandleCylinderTextureId, mesh.CandleCylinderTextureWidth, mesh.CandleCylinderTextureHeight, mesh.CandleCylinderTextureChannels);
}

// create cylinder
void UCreateSprayCylinder(GLMesh& mesh, const GLfloat cylinderRadius, const GLfloat cylinderHeight, const GLuint cylinderSegments) {
    vector<GLfloat> cylinderVertices;  // temporary buffer to hold both position and color data

    const auto cylinderSegmentAngleStep = (2 * glm::pi<float>()) / (float)cylinderSegments;  // calculate angle in radians to increment by

    GLfloat textureXStep = 1.0f / cylinderSegments;

    UCreateCylinderSides(cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, cylinderHeight, cylinderVertices, textureXStep);

    mesh.SprayCylinderStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.SprayBaseCylinderSideVertices = cylinderVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal); // number of torusVertices to render

    UCreateCylinderTop(cylinderVertices, cylinderHeight, cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, textureXStep);

    mesh.SprayBaseCylinderTopOrBottomVertices = (cylinderVertices.size() / (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal)) - mesh.CandleCylinderSideVertices; // number of Vertices to render

    UCreateCylinderBottom(cylinderVertices, cylinderHeight, cylinderSegments, cylinderSegmentAngleStep, cylinderRadius, textureXStep);

    ULoadTexture("Data\\chrome.jpg", mesh.SprayCylinderTextureId, mesh.SprayBaseCylinderTextureWidth, mesh.SprayBaseCylinderTextureHeight, mesh.SprayBaseCylinderTextureChannels);
}


void UCreateCandleBox(GLMesh& mesh) {
    GLfloat vertices[] = {
        // y is up
        // positon attributes (x,y,z)  //color (rgb)        // texture coords (UV)   // normal attribute (x,y,z)
        // left front front leg
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 1 red
        2.0f, 0.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 2 green
        2.0f, 1.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 3 blue
        2.0f, 1.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 4 blue
        0.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 5 green
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 6 red

        // left front left leg
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 1 red
        0.0f, 0.0f,  2.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 2 green
        0.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 3 blue
        0.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 4 blue
        0.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 5 green
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 6 red

        // left front right leg
        2.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 1 red
        2.0f, 0.0f,  2.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 2 green
        2.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 3 blue
        2.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 4 blue
        2.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 5 green
        2.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 6 red

        // left front back leg
        0.0f, 0.0f,  2.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
        2.0f, 0.0f,  2.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
        2.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
        2.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        0.0f, 1.0f,  2.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        0.0f, 0.0f,  2.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // left front top leg omitted
        // left front bottom leg omitted

        // right front front leg
        11.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 1 red
        13.0f, 0.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 2 green
        13.0f, 1.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 3 blue
        13.0f, 1.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 4 blue
        11.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 5 green
        11.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 6 red

        // right front left leg
        11.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 1 red
        11.0f, 0.0f,  2.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 2 green
        11.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 3 blue
        11.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 4 blue
        11.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 5 green
        11.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 6 red

        // right front right leg
        13.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 1 red
        13.0f, 0.0f,  2.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 2 green
        13.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 3 blue
        13.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 4 blue
        13.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 5 green
        13.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 6 red

        // right front back leg
        11.0f, 0.0f,  2.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
        13.0f, 0.0f,  2.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
        13.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
        13.0f, 1.0f,  2.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        11.0f, 1.0f,  2.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        11.0f, 0.0f,  2.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // right front top leg omitted
        // right front bottom leg omitted

        // left rear front leg
        0.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 1 red
        2.0f, 0.0f,  3.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 2 green
        2.0f, 1.0f,  3.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 3 blue
        2.0f, 1.0f,  3.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 4 blue
        0.0f, 1.0f,  3.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 5 green
        0.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 6 red

        // left rear left leg
        0.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 1 red
        0.0f, 0.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 2 green
        0.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 3 blue
        0.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 4 blue
        0.0f, 1.0f,  3.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 5 green
        0.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 6 red

        // left rear right leg
        2.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 1 red
        2.0f, 0.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 2 green
        2.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 3 blue
        2.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 4 blue
        2.0f, 1.0f,  3.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 5 green
        2.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 6 red

        // left rear back leg
        0.0f, 0.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
        2.0f, 0.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
        2.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
        2.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        0.0f, 1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        0.0f, 0.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // left rear top leg omitted
        // left rear bottom leg omitted

        // right rear front leg
        11.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 1 red
        13.0f, 0.0f,  3.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 2 green
        13.0f, 1.0f,  3.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 3 blue
        13.0f, 1.0f,  3.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 4 blue
        11.0f, 1.0f,  3.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 5 green
        11.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 6 red

        // right rear left leg
        11.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 1 red
        11.0f, 0.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 2 green
        11.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 3 blue
        11.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 4 blue
        11.0f, 1.0f,  3.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 5 green
        11.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,           // vert 6 red

        // right rear right leg
        13.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 1 red
        13.0f, 0.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 2 green
        13.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 3 blue
        13.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 4 blue
        13.0f, 1.0f,  3.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 5 green
        13.0f, 0.0f,  3.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 6 red

        // right rear back leg
        11.0f, 0.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
        13.0f, 0.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
        13.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
        13.0f, 1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        11.0f, 1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        11.0f, 0.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // right rear top leg omitted
        // right rear bottom leg omitted

        // front front long edge
        1.0f,  1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 1 red
       12.0f,  1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 2 green
       12.0f,  2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 3 blue
       12.0f,  2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 4 blue
        1.0f,  2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 5 green
        1.0f,  1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,           // vert 6 red

        // front top long edge
        1.0f,  2.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
       12.0f,  2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
       12.0f,  2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
       12.0f,  2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        1.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        1.0f,  2.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // front rear long edge
        1.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
       12.0f,  1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
       12.0f,  2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
       12.0f,  2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        1.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        1.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // front bottom long edge
        1.0f,  1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 1 red
       12.0f,  1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 2 green
       12.0f,  1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,           // vert 3 blue
       12.0f,  1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,           // vert 4 blue
        1.0f,  1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, -1.0f, 0.0f,           // vert 5 green
        1.0f,  1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 6 red

        // front left long edge omitted
        // front right long edge omitted

        // rear front long edge
        1.0f,  1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 1 red
       12.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 2 green
       12.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 3 blue
       12.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 4 blue
        1.0f,  2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 5 green
        1.0f,  1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 6 red

        // rear top long edge
        1.0f,  2.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
       12.0f,  2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
       12.0f,  2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
       12.0f,  2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        1.0f,  2.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        1.0f,  2.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // rear rear long edge
        1.0f,  1.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
       12.0f,  1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
       12.0f,  2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
       12.0f,  2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        1.0f,  2.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        1.0f,  1.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // rear bottom long edge
        1.0f,  1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 1 red
       12.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 2 green
       12.0f,  1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,           // vert 3 blue
       12.0f,  1.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,           // vert 4 blue
        1.0f,  1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, -1.0f, 0.0f,           // vert 5 green
        1.0f,  1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 6 red

        // rear left long edge omitted
        // rear right long edge omitted

        // left left short edge
        0.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 1 red
        0.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 2 green
        0.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 3 blue
        0.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 4 blue
        0.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 5 blue
        0.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 6 red

        // left top short edge
        0.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        0.0f,  2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        1.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        1.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        1.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 blue
        0.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // left right short edge
        1.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 1 red
        1.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 2 green
        1.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 3 blue
        1.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 4 blue
        1.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 5 blue
        1.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 6 red

        // left bottom short edge
        0.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 1 red
        0.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 2 green
        1.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 3 blue
        1.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 4 blue
        1.0f,  1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 5 blue
        0.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,           // vert 6 red

        // left front short edge omitted
        // left back short edge omitted

        // right left short edge
        12.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,             -1.0f, 0.0f, 0.0f,          // vert 1 red
        12.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,             -1.0f, 0.0f, 0.0f,          // vert 2 green
        12.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,             -1.0f, 0.0f, 0.0f,          // vert 3 blue
        12.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,             -1.0f, 0.0f, 0.0f,          // vert 4 blue
        12.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,             -1.0f, 0.0f, 0.0f,          // vert 5 blue
        12.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,             -1.0f, 0.0f, 0.0f,          // vert 6 red

        // right top short edge
        12.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        12.0f,  2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        13.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        13.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        13.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 blue
        12.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // right right short edge
        13.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 1 red
        13.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 2 green
        13.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 3 blue
        13.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 4 blue
        13.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 5 blue
        13.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 6 red

        // right bottom short edge
        12.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 1 red
        12.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 2 green
        13.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 3 blue
        13.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 4 blue
        13.0f,  1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 5 blue
        12.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 6 red

        // right front short edge omitted
        // right back short edge omitted

        // middleleft left short edge
        4.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 1 red
        4.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 2 green
        4.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 3 blue
        4.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 4 blue
        4.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 5 blue
        4.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 6 red

        // middleleft top short edge
        4.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        4.0f,  2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        5.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        5.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        5.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 blue
        4.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // middleleft right short edge
        5.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 1 red
        5.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 2 green
        5.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 3 blue
        5.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 4 blue
        5.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 5 blue
        5.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 6 red

        // middleleft bottom short edge
        4.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 1 red
        4.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 2 green
        5.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 3 blue
        5.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 4 blue
        5.0f,  1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 5 blue
        4.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 6 red

        // middleleft front short edge omitted
        // middleleft back short edge omitted

        // middleright left short edge
        8.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 1 red
        8.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 2 green
        8.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 3 blue
        8.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 4 blue
        8.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,          // vert 5 blue
        8.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              -1.0f, 0.0f, 0.0f,          // vert 6 red

        // middleright top short edge
        8.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        8.0f,  2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        9.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        9.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        9.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 blue
        8.0f,  2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // middleright right short edge
        9.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 1 red
        9.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 2 green
        9.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 3 blue
        9.0f,  2.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 4 blue
        9.0f,  2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 5 blue
        9.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 6 red

        // middleright bottom short edge
        8.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 1 red
        8.0f,  1.0f,  4.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 2 green
        9.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 3 blue
        9.0f,  1.0f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 4 blue
        9.0f,  1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, -1.0f, 0.0f,          // vert 5 blue
        8.0f,  1.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, -1.0f, 0.0f,          // vert 6 red

        // middleright front short edge omitted
        // middleright back short edge omitted

        // left panel 
        1.0f,  1.8f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 1 red
        4.0f,  1.8f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 2 green
        4.0f,  1.8f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 3 blue
        4.0f,  1.8f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 4 blue
        1.0f,  1.8f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 5 green
        1.0f,  1.8f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 6 red

        // middle panel 
        5.0f,  1.8f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 1 red
        8.0f,  1.8f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 2 green
        8.0f,  1.8f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 3 blue
        8.0f,  1.8f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 4 blue
        5.0f,  1.8f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 5 green
        5.0f,  1.8f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 6 red

        // right panel 
        9.0f,  1.8f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 1 red
       12.0f,  1.8f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 2 green
       12.0f,  1.8f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 3 blue
       12.0f,  1.8f,  4.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 4 blue
        9.0f,  1.8f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,          // vert 5 green
        9.0f,  1.8f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,          // vert 6 red

        // left front front corner
        0.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 1 red
        1.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 2 green
        1.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 3 blue
        1.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 4 blue
        0.0f, 2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 5 green
        0.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 6 red

        // left top front corner
        0.0f, 2.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        1.0f, 2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        1.0f, 2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        1.0f, 2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        0.0f, 2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        0.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue

        // left left front corner
        0.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,             -1.0f, 0.0f, 0.0f,           // vert 1 red
        0.0f, 1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,             -1.0f, 0.0f, 0.0f,           // vert 2 green
        0.0f, 2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,             -1.0f, 0.0f, 0.0f,           // vert 3 blue
        0.0f, 2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,             -1.0f, 0.0f, 0.0f,           // vert 4 blue
        0.0f, 2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,             -1.0f, 0.0f, 0.0f,           // vert 5 green
        0.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,             -1.0f, 0.0f, 0.0f,           // vert 6 red

        // left bottom front corner omitted
        // left rear front corner omitted
        // left right front corner omitted

        // right front front corner
        12.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 1 red
        13.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 2 green
        13.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 3 blue
        13.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 4 blue
        12.0f, 2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,          // vert 5 green
        12.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, -1.0f,          // vert 6 red

        // right top front corner
        12.0f, 2.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        12.0f, 2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        13.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        13.0f, 2.0f,  0.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        13.0f, 2.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        12.0f, 2.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // right right front corner
        13.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 1 red
        13.0f, 1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 2 green
        13.0f, 2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 3 blue
        13.0f, 2.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 4 blue
        13.0f, 2.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 5 green
        13.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,            // vert 6 red

        // right bottom front corner omitted
        // right rear front corner omitted
        // right left front corner omitted

        // left rear rear corner
        0.0f, 1.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,          // vert 1 red
        1.0f, 1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,          // vert 2 green
        1.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,          // vert 3 blue
        1.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,          // vert 4 blue
        0.0f, 2.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,          // vert 5 green
        0.0f, 1.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,          // vert 6 red

        // left top rear corner
        0.0f, 2.0f,  4.0f,              1.0f, 0.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        1.0f, 2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        0.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        0.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        1.0f, 2.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        1.0f, 2.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // left left rear corner
        0.0f, 1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,             -1.0f, 0.0f, 0.0f,           // vert 1 red
        0.0f, 1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,             -1.0f, 0.0f, 0.0f,           // vert 2 green
        0.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,             -1.0f, 0.0f, 0.0f,           // vert 3 blue
        0.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,             -1.0f, 0.0f, 0.0f,           // vert 4 blue
        0.0f, 2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,             -1.0f, 0.0f, 0.0f,           // vert 5 green
        0.0f, 1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,             -1.0f, 0.0f, 0.0f,           // vert 6 red

        // left bottom front corner omitted
        // left rear front corner omitted
        // left right front corner omitted

        // right rear rear corner
        12.0f, 1.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,          // vert 1 red
        13.0f, 1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 0.0f, 1.0f,          // vert 2 green
        13.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,          // vert 3 blue
        13.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 0.0f, 1.0f,          // vert 4 blue
        12.0f, 2.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,          // vert 5 green
        12.0f, 1.0f,  5.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 0.0f, 1.0f,          // vert 6 red

        // right top rear corner
        12.0f, 2.0f,  4.0f,              1.0f, 0.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        13.0f, 2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        13.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        13.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        12.0f, 2.0f,  5.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        12.0f, 2.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red

        // right right rear corner
        13.0f, 1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 1 red
        13.0f, 1.0f,  5.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              1.0f, 0.0f, 0.0f,           // vert 2 green
        13.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 3 blue
        13.0f, 2.0f,  5.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 4 blue
        13.0f, 2.0f,  4.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,           // vert 5 green
        13.0f, 1.0f,  4.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              1.0f, 0.0f, 0.0f            // vert 6 red

        // right bottom front corner omitted
        // right rear front corner omitted
        // right left front corner omitted
    };

    mesh.CandleBoxStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.CandleBoxVertices = sizeof(vertices) / mesh.PlaneStride;

    ULoadTexture("Data\\wood.jpg", mesh.CandleBoxTextureId, mesh.CandleBoxTextureWidth, mesh.CandleBoxTextureHeight, mesh.CandleBoxTextureChannels);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void UCreateMatchBox(GLMesh& mesh) {
    GLfloat vertices[] = {
        // positon attributes (x,y,z)  //color (rgb)        // texture coords (UV)   // normal attribute (x,y,z)

        // front 
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 1 red
        2.0f, 0.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 2 green
        2.0f, 1.0f,  0.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 3 blue
        2.0f, 1.0f,  0.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 4 blue
        0.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 5 green
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, -1.0f,           // vert 6 red

        // left 
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 1 red
        0.0f, 0.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 2 green
        0.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 3 blue
        0.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 4 blue
        0.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 5 green
        0.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              -1.0f, 0.0f, 0.0f,           // vert 6 red

        // right
        2.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 1 red
        2.0f, 0.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 2 green
        2.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 3 blue
        2.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 4 blue
        2.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 5 green
        2.0f, 0.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              1.0f, 0.0f, 0.0f,            // vert 6 red

        // back
        0.0f, 0.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 1 red
        2.0f, 0.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 2 green
        2.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 3 blue
        2.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 4 blue
        0.0f, 1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 5 green
        0.0f, 0.0f,  1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 0.0f,              0.0f, 0.0f, 1.0f,           // vert 6 red

        // top
        0.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        2.0f, 1.0f,  0.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        2.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        2.0f, 1.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
        0.0f, 1.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
        0.0f, 1.0f,  0.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f            // vert 6 red

        //bottom omitted
    };

    mesh.MatchBoxStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.MatchBoxVertices = sizeof(vertices) / mesh.MatchBoxStride;

    stbi_set_flip_vertically_on_load(true);
    ULoadTexture("Data\\matchbox.jpg", mesh.MatchBoxTextureId, mesh.MatchBoxTextureWidth, mesh.MatchBoxTextureHeight, mesh.MatchBoxTextureChannels);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void UCreatePlane(GLMesh& mesh)
{
    GLfloat vertices[] = {
        // positon attributes (x,y,z)  //color (rgb)        // texture coords (UV)   // normal attribute (x,y,z)
       -1.0f, 0.0f, -1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 1 red
        1.0f, 0.0f, -1.0f,              0.0f, 1.0f, 0.0f,   1.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 2 green
        1.0f, 0.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 3 blue
        1.0f, 0.0f,  1.0f,              0.0f, 0.0f, 1.0f,   1.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 4 blue
       -1.0f, 0.0f,  1.0f,              0.0f, 1.0f, 0.0f,   0.0f, 0.0f,              0.0f, 1.0f, 0.0f,           // vert 5 green
       -1.0f, 0.0f, -1.0f,              1.0f, 0.0f, 0.0f,   0.0f, 1.0f,              0.0f, 1.0f, 0.0f,           // vert 6 red
    };

    mesh.PlaneStride = sizeof(GLfloat) * (mesh.ValuesPerVertex + mesh.ValuesPerColor + mesh.ValuesPerTexture + mesh.ValuesPerNormal);
    mesh.nPlaneVertices = sizeof(vertices) / mesh.PlaneStride;

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // load texture 
    stbi_set_flip_vertically_on_load(true);
    ULoadTexture("Data\\newspaper.jpg", mesh.NewsPaperTextureId, mesh.NewsPaperTextureWidth, mesh.NewsPaperTextureHeight, mesh.NewsPaperTextureChannels);
}

// Implements the UCreateMesh function
// create a torus to represent a candle holder and cylinder inside the torus to represent a votive
void UCreateMesh(GLMesh& mesh)
{
    const GLuint  torusSegments = 30;       // number of segments to draw around torus
    const GLuint  tubePoints    = 30;       // number of points around tube
    const GLfloat torusRadius   = 2.0f;     // radius (R) of torus
    const GLfloat tubeRadius    = 1.0f;     // radius (r) of tube around torus

    const GLuint  cylinderSegments = 30;    // number of segments to draw around torus
    const GLfloat cylinderRadius   = torusRadius - tubeRadius;
    const GLfloat cylinderHeight   = 0.75f * tubeRadius; // we don't want cylinder as tall as torus height

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);     // activate vertex array

    // send vertex buffer to graphics card memory
    glGenBuffers(6, mesh.vbos);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);    // Activates the buffer
    UCreatePlane(mesh);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[1]);    // Activates the buffer
    UCreateTorus(mesh, torusRadius, tubeRadius, torusSegments, tubePoints);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[2]);    // Activates the cylinder buffer
    UCreateCylinder(mesh, cylinderRadius, cylinderHeight, cylinderSegments);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[3]);    // Activates the cylinder buffer
    UCreateCandleBox(mesh);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[4]);    // Activates the matchbox buffer
    UCreateMatchBox(mesh);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[5]);    // Activates the cylinder buffer
    UCreateCandleCylinder(mesh, 2.0f, 1.0f, cylinderSegments);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[6]);    // Activates the cylinder buffer
    UCreateSprayCylinder(mesh, 1.0f, 1.0f, cylinderSegments);

    glBindBuffer(GL_ARRAY_BUFFER, 0);               // unbind the buffer
}


/// <summary>
/// Application Entry Point
/// </summary>
// main function. Entry point to the OpenGL program
int main(int argc, char* argv[])
{
    GLMesh mesh;

    // initialize OpenGL and create window
    if (!UInitialize(gWindow, &gWindow.windowPtr))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateMesh(mesh); // Calls the function to create the Vertex Buffer Object

    // Create the shader program
    Shader* defaultShader = new Shader();
    Shader* textureShader = new Shader(Shader::TextureVertexShaderSource, Shader::TextureFragmentShaderSource);
    Shader* planeShader = new Shader(Shader::LightingVertexShaderSource, Shader::LightingFragmentShaderSource);

    if (defaultShader->getProgramId() == 0  || textureShader->getProgramId() == 0 || planeShader->getProgramId() == 0)
        return EXIT_FAILURE;

    mesh.defaultProgram = defaultShader;
    mesh.textureProgram = textureShader;
    mesh.lightingProgram = planeShader;

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // initialize window perspective projection 
    gWindow.Projection = glm::perspective(glm::radians(gCamera.Fov), (GLfloat)gWindow.Width / (GLfloat)gWindow.Height, 0.1f, 100.0f);
    gWindow.ProjectionMode = WindowProjection::Perspective;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow.windowPtr))
    {
        // input
        UProcessInput(gWindow.windowPtr);

        // Render this frame
        URender(mesh);

        glfwPollEvents();

        double currentTime = glfwGetTime();
        gTime.DeltaTime = (float)currentTime - gTime.LastTime;
        gTime.LastTime = (float)currentTime;
    }

    // Release mesh data
    UDestroyMesh(mesh);

    // Release shader program
    delete mesh.lightingProgram;
    delete mesh.textureProgram;
    delete mesh.defaultProgram;

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Initialize GLFW, GLEW, and create a window
bool UInitialize(WindowParams params, GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(params.Width, params.Height, params.Title.c_str(), NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // capture mouse
    glfwSetCursorPosCallback(*window, UMouse);
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // setup scroll wheel
    glfwSetScrollCallback(*window, UScroll);

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    const float cameraSpeed = gCamera.CameraSpeed * gTime.DeltaTime; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);         // exit
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.Position += cameraSpeed * gCamera.Front;  // move camera forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.Position -= cameraSpeed * gCamera.Front;  // move camera back
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.Position -= glm::normalize(glm::cross(gCamera.Front, gCamera.Up)) * cameraSpeed;  // slide camera left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.Position += glm::normalize(glm::cross(gCamera.Front, gCamera.Up)) * cameraSpeed;  // slide camera right

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.Position += cameraSpeed * gCamera.Up; // move camera up
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.Position -= cameraSpeed * gCamera.Up; // move camera down

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // shade faces
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        // toggle projection mode 
        if (gWindow.ProjectionMode == WindowProjection::Perspective) {
            // Creates a orthographic projection
            gWindow.Projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
            gWindow.ProjectionMode = WindowProjection::Orthographic;
        }
        else {
            // create a persective projection
            gWindow.Projection = glm::perspective(glm::radians(gCamera.Fov), (GLfloat)gWindow.Width / (GLfloat)gWindow.Height, 0.1f, 100.0f);
            gWindow.ProjectionMode = WindowProjection::Perspective;
        }
    }

    //cout << "Camera Pos: " << gCamera.Position.x << ", " << gCamera.Position.y << ", " << gCamera.Position.z;
}

void UMouse(GLFWwindow* window, double xpos, double ypos)
{
    if (gMouse.FirstMouse)
    { // initialize once
        gMouse.LastX = (float)xpos;
        gMouse.LastY = (float)ypos;
        gMouse.FirstMouse = false;
    }

    // compute offset and update last pos
    float xoffset = (float)xpos - gMouse.LastX;
    float yoffset = gMouse.LastY - (float)ypos;
    gMouse.LastX = (float)xpos;
    gMouse.LastY = (float)ypos;

    // adjust for sensitivity and delta time
    float sensitivity = 20.0f;
    xoffset *= sensitivity * gTime.DeltaTime;
    yoffset *= sensitivity * gTime.DeltaTime;

    // update values
    gCamera.Yaw += xoffset;
    gCamera.Pitch += yoffset;

    // clamp pitch between -89 and +89
    if (gCamera.Pitch > 89.0f)
        gCamera.Pitch = 89.0f;
    if (gCamera.Pitch < -89.0f)
        gCamera.Pitch = -89.0f;

    // calc direction
    glm::vec3 direction;
    direction.x = cos(glm::radians(gCamera.Yaw)) * cos(glm::radians(gCamera.Pitch));
    direction.y = sin(glm::radians(gCamera.Pitch));
    direction.z = sin(glm::radians(gCamera.Yaw)) * cos(glm::radians(gCamera.Pitch));

    // update camera facing
    gCamera.Front = glm::normalize(direction);

    //cout << "Camera Front: " << gCamera.Front.x << ", " << gCamera.Front.y << ", " << gCamera.Front.z;
    //cout << "Yaw: " << gCamera.Yaw << " Pitch: " << gCamera.Pitch;
}

void UScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.CameraSpeed += static_cast<float>(yoffset);  //scroll up increases movement speed, scroll down slows down speed

    // clamp camera speed between 12 and 60
    if (gCamera.CameraSpeed < 12.0f)
        gCamera.CameraSpeed = 12.0f;
    if (gCamera.CameraSpeed > 60.0f)
        gCamera.CameraSpeed = 60.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    gWindow.Width = width;
    gWindow.Height = height;
}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbos);
}