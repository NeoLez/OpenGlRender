#pragma once
#include <glfw/glfw3.h>
#include <iostream>

class Window {
public:
    int screenWidth = 0, screenHeight = 0;
    GLFWwindow* windowHandle;

    Window(int screenWidth, int screenHeight) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        windowHandle = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        if (windowHandle == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(windowHandle);

        glfwSetWindowUserPointer(windowHandle, this);

        glfwSetFramebufferSizeCallback(windowHandle, framebuffer_size_callback);

        //glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (instance) {
            instance->onFramebufferSizeChanged(width, height);
        }
    }

    void onFramebufferSizeChanged(int width, int height) {
        glViewport(0, 0, width, height);
        screenWidth = width;
        screenHeight = height;
    }
};