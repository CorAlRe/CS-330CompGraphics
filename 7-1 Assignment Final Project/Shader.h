#pragma once

#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {

public:
    static const GLchar* DefaultVertexShaderSource;
    static const GLchar* DefaultVertexFragmentShaderSource;
    static const GLchar* TextureVertexShaderSource;
    static const GLchar* TextureFragmentShaderSource;
    static const GLchar* LampVertexShaderSource;
    static const GLchar* LampFragmentShaderSource;
    static const GLchar* LightingVertexShaderSource;
    static const GLchar* LightingFragmentShaderSource;

    // constructor reads and builds the shader
    Shader();
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    Shader(std::string vertexPath, std::string fragmentPath);

    ~Shader();

    // behavior
    void use();

    // accessors
    GLuint getProgramId() { return ID; }

    // mutators
    void setProjectionMatrix(const glm::mat4& projection);
    void setModelMatrix(const glm::mat4& model);
    void setViewMatrix(const glm::mat4& view);
    void setTextureUnit(GLint unit);
    void setUniformValue(std::string name, GLfloat value);
    void setUniformValue(std::string name, glm::vec3 value);

private:
    GLuint ID = 0;

    void CompileProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
};