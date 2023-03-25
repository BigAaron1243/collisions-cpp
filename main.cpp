
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include "loader.h"

int main(int argc, char *argv[]) {
    GLFWwindow* window = lInitGLFW();   
    glfwMakeContextCurrent(window);
    gladLoadGL();
    while (!glfwWindowShouldClose(window)) {
        
    }
    lTerminateGLFW(window);
}
