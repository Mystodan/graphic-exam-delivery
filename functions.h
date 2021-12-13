#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

/**
 *  Enum type for direction.
 */
enum direction {
    left, right, up, down
};

GLuint CompileShader(const std::string& vertexShader,
                     const std::string& fragmentShader,
                     const std::string& geometryShader = "");

void CleanVAO(GLuint& vao, GLuint* eboId = nullptr);

#endif // !__FUNCTIONS_H