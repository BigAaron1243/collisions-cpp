

struct vattr
{
    float x, y;
    float r, g, b;
}; 

struct vattr vertices[9] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f },
    { 1.f, 1.f, 0.f, 1.f, 1.f},
    { 1.f, 0.f, 1.f, 0.f, 1.f},
    { 0.f, 1.f, 1.f, 1.f, 0.f},
    { 0.f, 0.2f, 1.f, 1.f, 0.f},
    { 0.2f, 0.f, 1.f, 0.f, 1.f},
    { -1.f, 1.f, 0.f, 1.f, 1.f}
};


struct vattr vertices2[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f },
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";


static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

void lUpdateBuffers(vattr *invertices) { 
    glBufferData(GL_ARRAY_BUFFER, sizeof(invertices), invertices, GL_DYNAMIC_DRAW);
}

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        lUpdateBuffers(vertices2);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
    }
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
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, key_callback);
    return window;
}

void lTerminateGLFW(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}


