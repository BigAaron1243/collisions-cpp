
void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description;
}

GLFWwindow* lInitGLFW() {
    if (!glfwInit())
    {
        return NULL;
    }
    glfwSetErrorCallback(error_callback);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Collisions", NULL, NULL);
    if (!window) {
        return NULL;
    }
    return window;
}

void lTerminateGLFW(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

