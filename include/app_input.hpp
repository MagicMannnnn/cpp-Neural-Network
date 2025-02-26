#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <settings.hpp>
#include <Settings.hpp>
#include <camera.hpp>
#include <iostream>
#include <text_renderer.hpp>

namespace Global {
    GLFWwindow* window;
    Camera camera(glm::vec3(0.0f, 20.f, 0.0f));
    float mouseLastX = Settings::SCR_WIDTH / 2.0f;
    float mouseLastY = Settings::SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    bool leftClick = false;
    bool rightClick = false;
    bool firstLeftClick = true;

    float deltaTime = 0.0f;

    int seed = 1234;

    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.setFastVelocity(true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);

        camera.setFastVelocity(false);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }


    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            mouseLastX = xpos;
            mouseLastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - mouseLastX;
        float yoffset = mouseLastY - ypos; // reversed since y-coordinates go from bottom to top

        mouseLastX = xpos;
        mouseLastY = ypos;
        if (rightClick) {
            camera.ProcessMouseMovement(xoffset, yoffset);
            glfwSetInputMode(Global::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(Global::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
            
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {

        camera.ProcessMouseScroll(static_cast<float>(yoffset), deltaTime);
    }


    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) { 
            if (action == GLFW_PRESS) {
                   leftClick = true;            
            }
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT) { 
            if (action == GLFW_PRESS) {
                rightClick = true;
            }
            if (action == GLFW_RELEASE) {
                rightClick = false;
            }
        }
    }
}

