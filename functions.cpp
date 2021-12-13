#include "functions.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <set>

/**
 *  Create a shader program.
 *  
 *  This code is taken and adapted from lab02: https://git.gvk.idi.ntnu.no/course/prog2002/labs/lab02/-/blob/master/main.cpp
 * 
 *  @param vertexShaderSrc - The vertex shader
 *  @param fragmentShaderSrc - The fragment shader
 *  @param geometryShaderSrc - The geometry shader (if there is any)
 *  @return A shader program
 */
GLuint CompileShader(const std::string& vertexShaderSrc,
                     const std::string& fragmentShaderSrc,
                     const std::string& geometryShaderSrc/*=""*/) {

    auto vertexSrc   = vertexShaderSrc.c_str();
    auto fragmentSrc = fragmentShaderSrc.c_str();
    auto geometrySrc = geometryShaderSrc.c_str();

    // Create program
    auto shaderProgram = glCreateProgram();

    // Vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    int bufflen;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &bufflen);
    if (bufflen > 1) {
        GLchar* log_string = new char[bufflen + 1];
        glGetShaderInfoLog(vertexShader, bufflen, 0, log_string);

        std::cerr << log_string << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }

    glAttachShader(shaderProgram, vertexShader);

    // Geometry shader (if there is any)
    auto geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    if (geometryShaderSrc != "") {
        glShaderSource(geometryShader, 1, &geometrySrc, nullptr);
        glCompileShader(geometryShader);
        glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &bufflen);
        if (bufflen > 1) {
            GLchar* log_string = new char[bufflen + 1];
            glGetShaderInfoLog(geometryShader, bufflen, 0, log_string);

            std::cerr << log_string << std::endl;
            std::cin.get();
            return EXIT_FAILURE;
        }

        glAttachShader(shaderProgram, geometryShader);
    }

    // Fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &bufflen);
    if (bufflen > 1) {
        GLchar* log_string = new char[bufflen + 1];
        glGetShaderInfoLog(fragmentShader, bufflen, 0, log_string);

        std::cerr << log_string << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }

    glAttachShader(shaderProgram, fragmentShader);

    // Link
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    return shaderProgram;
}


/**
 *  Clean up vao.
 * 
 *  This code is taken from lab02: https://git.gvk.idi.ntnu.no/course/prog2002/labs/lab02/-/blob/master/main.cpp
 * 
 *  @param vao - The vao to be cleaned up
 */
void CleanVAO(GLuint& vao, GLuint* eboId) {
    GLint nAttr = 0;
    std::set<GLuint> vbos;

    if (eboId != nullptr) {
        glDeleteBuffers(1, eboId);
    }

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttr);
    glBindVertexArray(vao);

    for (int iAttr = 0; iAttr < nAttr; ++iAttr)
    {
        GLint vboId = 0;
        glGetVertexAttribiv(iAttr, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vboId);
        if (vboId > 0)
        {
            vbos.insert(vboId);
        }

        glDisableVertexAttribArray(iAttr);
    }

    for (auto vbo : vbos)
    {
        glDeleteBuffers(1, &vbo);
    }

    glDeleteVertexArrays(1, &vao);
}


