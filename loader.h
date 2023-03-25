
void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description;
}

int lInitGLFW() {
    if (!glfwInit())
    {
        return -1;
    }
    glfwSetErrorCallback(error_callback);
    return 0;
}

void lTerminateGLFW() {
    glfwTerminate();
}

