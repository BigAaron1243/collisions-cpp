
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"
#include <iostream>
#include <vector>


struct vattr
{
    float x, y;
    float r, g, b;
}; 


bool toggle = 0;

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   0.0f, -1.0f, 0.0f,
   -0.5f,  1.0f, 0.0f,
};

static const GLfloat g_vertex_buffer_data_2[] = {
   1.0f, -1.0f, 0.0f,
   0.0f, -1.0f, 0.0f,
   0.5f,  1.0f, 0.0f,
};

static const char* vertex_shader_text =
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"void main() {\n"
"   gl_Position.xyz = vertexPosition_modelspace;\n"
"   gl_Position.w = 1.0;\n"
"}\n";



static const char* fragment_shader_text =
"#version 330 core\n"
"out vec3 color;\n"
"void main() {\n"
"    color = vec3(0, 1, 0);\n"
"}\n";


void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        toggle = !toggle;
}



int main(int argc, char *argv[]) {


    // Init glfw
    if (!glfwInit())
    {
        std::cerr << "Failed to init glfw\n";
        return -1;
    }
    glfwSetErrorCallback(error_callback);

    // Set window hints
    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    
    // Create window and make it current
    GLFWwindow* window = glfwCreateWindow(640, 480, "Collisions", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open glfw window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Load openGL
    gladLoadGL();

    // Set key callback
    glfwSetKeyCallback(window, key_callback);


    // VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Buffer
    GLuint vertexbuffer;

    // Generate buffer and put identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // This will give vertices to opengl
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_2), g_vertex_buffer_data_2, GL_DYNAMIC_DRAW);

    // Compile shaders
    // Vertex shader
    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(VertexShaderID, 1, &vertex_shader_text, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	glShaderSource(FragmentShaderID, 1, &fragment_shader_text , NULL);
	glCompileShader(FragmentShaderID);

    // Check fragment shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

    // Link program (Shaders)
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    glUseProgram(ProgramID);

    // Set glClearColor
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

    while (!glfwWindowShouldClose(window)) {

        if (toggle) { 
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
        } else {
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_2), g_vertex_buffer_data_2, GL_DYNAMIC_DRAW);
        }
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // vertices buffer
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
        // attribute 0, size, type, normalized?, stride, array buffer offset
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
