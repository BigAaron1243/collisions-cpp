
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <math.h>

#define sq3o2 0.866025404f


class Particle {
    public:
        double x, y, r;
        double dx = 0;
        double dy = 0;

        int edges;
        std::vector<GLfloat> RVL;
        std::vector<GLfloat> xl;
        std::vector<GLfloat> yl;

        std::vector<GLfloat> get_draw_data();
        Particle(int edgesInit, double rInit, double xInit, double yInit); 
        void update(double scale);
};

Particle::Particle(int edgesInit, double rInit, double xInit, double yInit) {
    x = xInit;
    y = yInit;
    r = rInit;
    edges = edgesInit;
    double angle = (2 * M_PI) / edges;
    for (int i = 0; i < edges; i++) {
        RVL.insert(RVL.end(), {
            cos(i * angle), sin(i * angle), 0.f,
            cos((i + 1) * angle), sin((i + 1) * angle), 0.f,
            0.f, 0.f, 0.f 
        });
    }
    for (int i = 0; i < edges * 3; i++) {
        //xl.insert(xl.end(), {1, 0, 0});
        //yl.insert(xl.end(), {0, 1, 0});
        xl.push_back(1);
        xl.push_back(0);
        xl.push_back(0);

        yl.push_back(0);
        yl.push_back(1);
        yl.push_back(0);
    }
}

std::vector<GLfloat> Particle::get_draw_data() {
    std::vector<GLfloat> outPtr(edges * 9);
    /*GLfloat relative_pos[] = { 
        sq3o2, 0.5f, 0.0f, 0.f, 1.f, 0.0f, 0.f, 0.f, 0.0f, 
        0.f, 1.f, 0.0f, -sq3o2, 0.5f, 0.0f, 0.f, 0.0f, 0.0f,
        -sq3o2, 0.5f, 0.0f, -sq3o2, -0.5f, 0.0f, 0.f, 0.0f, 0.0f,
        -sq3o2, -0.5f, 0.0f, 0, -1.f, 0.0f, 0.f, 0.0f, 0.0f,
        0.f, -1.f, 0.0f, sq3o2, -0.5f, 0.0f, 0.f, 0.0f, 0.0f,
        sq3o2, -0.5f, 0.0f, sq3o2, 0.5f, 0.0f, 0.f, 0.0f, 0.0f,
    };*/
    //outPtr = clown;
    for (int i = 0; i < edges * 9; i++) {
        outPtr[i] = RVL[i] * r + xl[i] * x + yl[i] * y;
    }
    //memcpy(outPtr, &clown, 9 * sizeof(GLfloat));
    return outPtr;
}

void Particle::update(double scale) {
    x += dx * scale;
    y += dy * scale;
}

struct vattr
{
    float x, y;
    float r, g, b;
}; 


struct posxy {
    float x, y;
};

bool addPart = 0;

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

static const GLfloat g_color_buffer_data[] = {
    //1.f, 0.f, 0.f,
    //0.f, 1.f, 0.f,
    //0.f, 0.f, 1.f,
    //1.f, 1.f, 0.f,
    //0.f, 1.f, 1.f,
    //1.f, 0.f, 1.f,
};

static const char* vertex_shader_text =
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"layout(location = 1) in vec3 vertexColor;\n"
"out vec3 fragmentColor;\n"
"void main() {\n"
"   gl_Position.xyz = vertexPosition_modelspace;\n"
"   gl_Position.w = 1.0;\n"
"   fragmentColor = vec3((vertexColor.y + 1) /2, 0.f, ((vertexColor.y - 1) / 2) * -1);\n"
"}\n";



static const char* fragment_shader_text =
"#version 330 core\n"
"in vec3 fragmentColor;\n"
"out vec3 color;\n"
"void main() {\n"
"    color = fragmentColor;\n"
"}\n";


void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        addPart = true;
}

std::vector<struct posxy> particles;

int main(int argc, char *argv[]) {


    struct posxy pxy;
    pxy.x = 0;
    pxy.y = 0;


    /// vvvvvvv RELATED TO OPENGL vvvvvvv

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
    GLFWwindow* window = glfwCreateWindow(800, 800, "Collisions", NULL, NULL);
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

    // Vertex buffer
    GLuint vertexbuffer;

    // Color buffer
    GLuint colorbuffer;

    // Generate buffer and put identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &colorbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // This will give vertices to opengl
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_2), g_vertex_buffer_data_2, GL_DYNAMIC_DRAW);

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
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    int edges = 32;
    double radius = 0.2;
    std::vector<Particle> pList;
    pList.push_back(Particle(edges, radius, 0, 0));
    //pList.push_back(Particle(edges, radius, -0.5, -0.5));
    //pList.push_back(Particle(edges, radius, 0.5, 0.5));

    double timeNow = glfwGetTime();
    double deltaTime = 0;
    int increment = 0;
    while (!glfwWindowShouldClose(window)) {



        increment++;
        deltaTime = glfwGetTime() - timeNow;
        //std::cout << deltaTime << std::endl;
        //std::cout << timeNow << std::endl;
        timeNow = glfwGetTime();

        if (increment > 10) {
            std::cout << "FPS: " << 1 / deltaTime << std::endl;
            increment = 0;
        }
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        if (addPart) {
            //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
            pList.push_back(Particle(edges, radius, -0.1, 0));
            addPart = false;
        }
        std::vector<GLfloat> drawVertPtr;
        //-- Under here lies the collision detection
        
        for (int i = 0; i < pList.size(); i++) {
            //std::cout << iX << ", " << iY << " | " << cellx << ", " << celly << std::endl;
        }

        //Old + doesnt work + L + ratio
        /*
        for (double x = -1; x < 1; x+=2*radius) {
            for (double y = -1; y < 1; y+=2*radius) {
                
                for (int i = 0; i < pList.size(); i++) {
                    double ezX = pList.at(i).x;
                    double ezY = pList.at(i).y;
                    double cellx = ezX - ezX % (2 * radius);
                    double celly = ezY - ezY % (2 * radius);
                    if (ezX >= x && ezX <= x + 2*radius && ezY >= y && ezY <= y + 2 * radius) {
                        std::cout << ezX << ", " << ezY << " | " << x << "<>" << x + 2 * radius<< ", " <<y << "<>" << y + 2 * radius << std::endl;
                        for (int j = 0; j < pList.size(); j++) {
                            double ezX2 = pList.at(j).x;
                            double ezY2 = pList.at(j).y;
                            if (ezX2 >= x && ezX2 <= x + 2*radius && ezY2 >= y && ezY2 <= y + 2 * radius && i != j) {
                                double hypotenuse = sqrt(pow((ezX - ezX2), 2) + pow((ezY - ezY2), 2));
                                if (hypotenuse < 2*radius) {
                                    double theta = asin(ezY - ezY2 / hypotenuse);
                                    std::cout << "arcsin of y1: " << ezY << "minus y2: " << ezY2 << " / Hypotenuse: " << hypotenuse << " = " << theta <<std::endl;
                                    double h = (hypotenuse - 2 * radius) * sin(theta);
                                    double b = (hypotenuse - 2 * radius) * cos(theta);
                                    std::cout << h;
                                    pList.at(i).x += b / 2;
                                    pList.at(j).x -= b / 2;
                                    pList.at(i).y += h / 2;
                                    pList.at(j).y -= h / 2;
                                    pList.at(i).dy = 0;
                                }   
                            }
                        }
                    }
                }
            }
        }*/
        for (int i = 0; i < pList.size(); i++) {
      
            //-- Apply gravity
            pList[i].dy += -0.01;

            //-- If touching floor, walls, or ceiling, get pushed back, 0 velocity:
            if (pList.at(i).x + pList.at(i).r > 1) {
                pList.at(i).x = 1 - pList.at(i).r;
                pList[i].dx = 0;
                //pList.at(i).addDxDy(-pList.at(i).dx, 0);
            }

            if (pList.at(i).y + pList.at(i).r > 1) {
                pList.at(i).y = 1 - pList.at(i).r;
                //pList.at(i).addDxDy(0, -pList.at(i).dy);
                pList[i].dy = 0;
            }

            if (pList.at(i).x - pList.at(i).r < -1) {
                pList.at(i).x = -1 + pList.at(i).r;
                //pList.at(i).addDxDy(-pList.at(i).dx, 0);
                pList[i].dx = 0;
            }

            if (pList.at(i).y - pList.at(i).r < -1) {
                pList.at(i).y = -1 + pList.at(i).r;
                //pList.at(i).addDxDy(0, -pList.at(i).dy);
                pList[i].dy = 0;
            }
            
            //-- Collision detection
            double iX = pList[i].x;
            double iY = pList[i].y;
            double cRng = 2 * radius;
            for (int j = 0; j < pList.size(); j++) {
                if (i != j) {
                    double jX = pList[j].x;
                    double jY = pList[j].y;
                    if (jX < iX + cRng && jX > iX - cRng && jY < iY + cRng && jY > iY - cRng) {
                        double hypotenuse = sqrt(pow((iX - jX), 2) + pow((iY - jY), 2));
                        if (hypotenuse < 2*radius) {
                            double c = 2 * radius - hypotenuse;
                            double theta = asin((iY - jY) / hypotenuse);
                            //std::cout << "c: " << c << std::endl;
                            double h = (c / 2) * sin(theta);
                            double b = (c / 2) * cos(theta);
                            pList.at(i).x += b;
                            pList.at(j).x -= b;
                            pList.at(i).y += h;
                            pList.at(j).y -= h;
                            pList[i].dy = 0;
                            pList[j].dy = 0;
                            //pList[i].addDxDy(-pList[i].dx, -pList[i].dy);
                            //pList[j].addDxDy(-pList[j].dx, -pList[j].dy);
                        }
                    }
                }
            }
            //std::cout << i << " | x: " << pList[i].x << " y: " << pList[i].y << std::endl;

            //-- Update position as per dx amd dy
            pList[i].update(deltaTime);
        }

        /*for (int i = 0; i < lilpart.get_draw_data().size(); i++) {
            if ((i) % 3 == 0) {
                std::cout << '(' << lilpart.get_draw_data()[i] << ", ";
            } else if ((i -1) % 3 == 0) {
                std::cout << lilpart.get_draw_data()[i] << "), ";
            } else {

            }
        }
        std::cout << std::endl;
        std::cout << std::endl;
        for (auto element : lilpart.get_draw_data()) {
            std::cout << element << ", ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;*/
        for (int i = 0; i < pList.size(); i++) {
            for (auto element : pList[i].get_draw_data()) {
                drawVertPtr.push_back(element);
            }
        }
        GLfloat *drawVertPtrArray = &drawVertPtr[0];
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * edges * 9 * pList.size(), drawVertPtrArray, GL_DYNAMIC_DRAW);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // vertices buffer
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        
        // attribute 0, size, type, normalized?, stride, array buffer offset
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // color buffer
        glEnableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, edges * 3 * pList.size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
