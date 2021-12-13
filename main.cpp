#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/spriteShader.h"
#include "functions.h"
#include "Window.h"
#include "Sprite.h"

#include <iostream>


/**
 *  The main program.
 */
int main()
{
    // Initialization of GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed." << '\n';
        std::cin.get();

        return EXIT_FAILURE;
    }

    // Create window
    Window windowManager(600.f, 1.f, false);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Gameloop vars
    GLuint      shader = CompileShader(spriteVertexShaderSrc, spriteFragmentShaderSrc);
    floatRect   texRect     (0.f,   0.f,    1.f,    1.f);
    intRect     spriteRect  (0,     0,      1,      1);
    Sprite      sprite      ("./../assets/example.png", shader, texRect, spriteRect);

    float t = 0.f; // Total time elapsed since start of program
    windowManager.setAspectRatio(1.f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    // Start gameloop
    GLFWwindow* window = windowManager.getWindow();
    while (!glfwWindowShouldClose(window)) {
        // time
        float dt = glfwGetTime() - t;
        t += dt;

        // Keys
        glfwPollEvents();
        bool    k_left  = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS),
                k_right = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS),
                k_up    = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS),
                k_down  = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS),
                k_f     = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);

        // Update windowManager
        windowManager.update(k_f);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Update
        sprite.draw();

        // Exit program when ESC is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwTerminate();
            return EXIT_SUCCESS;
        }

        // Flip screen
        glfwSwapBuffers(window);

        // Limit framerate to 60 fps
        while (glfwGetTime() < t + 1.0 / 60) {
        }

    }

    // Return
    glUseProgram(0);
    glfwTerminate();
    return EXIT_SUCCESS;
}

