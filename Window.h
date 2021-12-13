#ifndef __WINDOW_H
#define __WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/**
 *  A simple class for handling the window.
 */
class Window {
private:
    float       width,
                aspectRatio;
    bool        fullscreen;
    int         key_counter;
    GLFWwindow* window;

public:
    Window(float width, float aspectRatio, bool fullscreen=false);

    void        setAspectRatio(float ratio) { aspectRatio = ratio; }
    GLFWwindow* getWindow()                 { return window; }
    
    void init_windows();
    void update(bool k_f);
};

#endif 