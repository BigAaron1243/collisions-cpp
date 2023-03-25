

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include "loader.h"

int main(int argc, char *argv[]) {
    lInitGLFW();
    std::cout << "Got 'ere\n";
    lTerminateGLFW();
}
