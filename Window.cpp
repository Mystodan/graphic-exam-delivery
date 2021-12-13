#include "Window.h"


/**
 *  Standard constructor.
 *  @param aspectRatio - The aspectratio of the (windowed) screen.
 */
Window::Window(float width, float aspectRatio, bool fullscreen/*=false*/) {
    this->width       = width;
    this->fullscreen  = fullscreen;
    this->aspectRatio = 1.f;
    key_counter = 0;
    init_windows();
}


/**
 *  Initializes the windows (fullscreen and normal mode).
 *  If an error is encountered the program is crashed.
 */
void Window::init_windows() {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, width/aspectRatio, "Assignment 1", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "GLFW failed on window creation." << '\n';
        std::cin.get();
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
}


/**
 *  Updates the window.
 *  @param f - Whether or not the button for fullscreen is pressed.
 */
void Window::update(bool k_f) {
    if (k_f) key_counter++; else key_counter = 0;
    if (key_counter == 1) {
        // Toggle fullscreen
        fullscreen = k_f ? (fullscreen ? false : true) : fullscreen;

        // Update video mode
        if (fullscreen)
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1900, 1200, 0);
        else 
            glfwSetWindowMonitor(window, nullptr, width/10.f, width/aspectRatio/10.f, width, width/aspectRatio, 0);
    }
        
    // Change context (if legible)
    if (k_f) glfwMakeContextCurrent(window);
    glEnable(GL_MULTISAMPLE); // Enable anti-aliasing

    // Resize window
    int windowX, windowY;
    glfwGetFramebufferSize(window, &windowX, &windowY);
    float   height = width / aspectRatio;

    // Set aspect ratio
    if (fullscreen) {
        glViewport(windowX/2.f-windowY*aspectRatio/2.f, 0, windowY*aspectRatio, windowY);
        
    } else {
        glfwSetWindowAspectRatio(window, width, height);
        glViewport(0, 0, windowX, windowY);
    }
}