#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vector Shader in GLSL (similar to C)
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char* fragmentShaderSourceYlw = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    return;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD - call after context is set to created window
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Create shader object referenced by some id
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach shader source code to shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if success
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int fragmentShaderYlw;
    fragmentShaderYlw = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYlw, 1, &fragmentShaderSourceYlw, NULL);
    glCompileShader(fragmentShaderYlw);

    // Linking the VertexShader and FragmentShader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if success
    char infoLog2[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog2);
        std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILED\n" << infoLog2 << std::endl;
    }

    // Linking the VertexShader and FragmentShader
    unsigned int shaderProgramYlw;
    shaderProgramYlw = glCreateProgram();
    glAttachShader(shaderProgramYlw, vertexShader);
    glAttachShader(shaderProgramYlw, fragmentShaderYlw);
    glLinkProgram(shaderProgramYlw);

    // Check if success
    char infoLog3[512];
    glGetProgramiv(shaderProgramYlw, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgramYlw, 512, NULL, infoLog3);
        std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILED\n" << infoLog3 << std::endl;
    }

    // After linking to ShaderProgram, no longer need compiled shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex Shader Input
    // Points on Normalized Device Coordinates (NDC)
    float vertices[] = {
        -0.5f  , 0.5f , 0.0f, // top
        -0.75f , -0.5f, 0.0f, // bottom left
        -0.25f , -0.5f, 0.0f,  // bottom right

        0.5f  , 0.5f , 0.0f, // top
        0.75f , -0.5f, 0.0f, // bottom left
        0.25f , -0.5f, 0.0f  // bottom right
    };

    // Generated and assign a unique Vertex Buffer Object (VBO) ID - refers to memory in GFX card
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Binding the newly created buffer/VBO to a buffer type GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copies the previously defined vertex data into the buffer's (VBO) memory on the GFX card
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, 800, 600);

    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands
        // specifies what color we want to clear the screen with
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw the orange triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw the yellow triangle
        glUseProgram(shaderProgramYlw);
        //glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
