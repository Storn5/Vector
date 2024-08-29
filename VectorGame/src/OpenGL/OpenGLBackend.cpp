#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include "OpenGL/OpenGLBackend.h"
#include "Utils/Helpers.h"

#ifdef _DEBUG
#define GLCALL(x) testOpenGL();\
    x;\
    ASSERT(testOpenGL())
#else
#define GLCALL(x) x
#endif

OpenGLBackend::OpenGLBackend(const GraphicsConfig& config)
    : m_window(nullptr)
{
    m_config = config;
    // Initialize the library
    if (!glfwInit())
    {
        fprintf(stderr, "Error while initializing GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    m_window = glfwCreateWindow(m_config.xRes, m_config.yRes, m_config.windowName.c_str(), NULL, NULL);
    if (!m_window)
    {
        fprintf(stderr, "Error creating a window in GLFW\n");
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Error while initializing GLEW\n");
        glfwTerminate();
        return;
    }
}

OpenGLBackend::~OpenGLBackend()
{
    glfwTerminate();
}

bool OpenGLBackend::testOpenGL()
{
    bool isOk = true;
    GLenum glError;
    unsigned char maxAttempts = 10;
    while ((glError = glGetError()) != GL_NO_ERROR && maxAttempts--)
    {
        isOk = false;
        fprintf(stderr, "OpenGL error\n");
        if (glError == GL_STACK_OVERFLOW)
            fprintf(stderr, "Stack overflow\n");
        else if (glError == GL_STACK_UNDERFLOW)
            fprintf(stderr, "Stack underflow\n");
        else if (glError == GL_OUT_OF_MEMORY)
            fprintf(stderr, "There is not enough memory left to execute the command\n");
        else if (glError == GL_INVALID_FRAMEBUFFER_OPERATION)
            fprintf(stderr, "The framebuffer object is not complete\n");
        else if (glError == GL_INVALID_OPERATION)
            fprintf(stderr, "The specified operation is not allowed in the current state\n");
        else if (glError == GL_INVALID_VALUE)
            fprintf(stderr, "A numeric argument is out of range\n");
        else if (glError == GL_INVALID_ENUM)
            fprintf(stderr, "An unacceptable value is specified for an enum argument\n");
    }
    return isOk;
}

unsigned int OpenGLBackend::createShader(const GLenum shaderType, const std::string& shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    const char* shaderSourcePtr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourcePtr, nullptr);
    glCompileShader(shader);

    // Error handling
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        GLint errorMsgLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorMsgLength);
        char* errorMsg = (char*)_malloca(errorMsgLength * sizeof(char));
        glGetShaderInfoLog(shader, errorMsgLength, &errorMsgLength, errorMsg);
        fprintf(stderr, "Error while compiling %s shader: %s\n", (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment"), errorMsg);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int OpenGLBackend::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    GLuint program = glCreateProgram();
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (vertexShader == 0 || fragmentShader == 0)
        return 0;

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

unsigned int OpenGLBackend::parseShaderFile(const std::string& filename)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream filestream(filename);

    if (!filestream)
    {
        fprintf(stderr, "Error parsing shader: file %s cannot be read\n", filename.c_str());
        return 0;
    }

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(filestream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else
            {
                fprintf(stderr, "Error parsing shader: invalid shader type\n");
                return 0;
            }
        }
        else
            ss[(int)type] << line << "\n";
    }

    return createShaderProgram(ss[0].str(), ss[1].str());
}

bool OpenGLBackend::isInitialized()
{
    if (m_window == nullptr)
        return false;

    const char* glfwError;
    if (glfwGetError(&glfwError) != GLFW_NO_ERROR)
    {
        fprintf(stderr, "Error after initializing GLFW: %s\n", glfwError);
        return false;
    }
    
    if (!testOpenGL())
        return false;

    return true;
}

void OpenGLBackend::playGameLoop()
{
    unsigned int num_vertices = 6;
    unsigned int vertexIndices[] =
    {
        0, 1, 2,
        0, 3, 4
    };

    GLuint vao, indexBuffer, vertexBuffer;

    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));

    GLCALL(glGenBuffers(1, &indexBuffer));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_vertices * sizeof(unsigned int), vertexIndices, GL_STATIC_DRAW));

    GLCALL(glGenBuffers(1, &vertexBuffer));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), 0));

    GLuint shader = parseShaderFile("res/Shaders/Shader.glsl");
    GLCALL(glUseProgram(shader));

    GLCALL(GLint shaderLocation = glGetUniformLocation(shader, "u_Color"));

    auto startTime = std::chrono::high_resolution_clock::now();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(m_window))
    {
        auto timeDiff = secondsBetween(startTime, std::chrono::high_resolution_clock::now());

        double triangleVertices[10] =
        {
            0.0, 0.0,
            0.0, 0.3 * abs(sin(timeDiff)),
            0.3 * abs(sin(timeDiff)), 0.0,
            0.0, -0.3 * abs(sin(timeDiff)),
            -0.3 * abs(sin(timeDiff)), 0.0
        };
        GLCALL(glBindVertexArray(vao));
        GLCALL(glBufferData(GL_ARRAY_BUFFER, 2 * num_vertices * sizeof(double), triangleVertices, GL_DYNAMIC_DRAW));
        GLCALL(glUniform4f(shaderLocation, abs(sin(timeDiff * 2)), abs(cos(timeDiff)), abs(sin((timeDiff / 2) + M_PI)), 1.0f));

        // Render here
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        GLCALL(glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, nullptr));

        // Swap front and back buffers
        GLCALL(glfwSwapBuffers(m_window));

        // Poll for and process events
        GLCALL(glfwPollEvents());
    }
    GLCALL(glDeleteBuffers(1, &indexBuffer));
    GLCALL(glDeleteBuffers(1, &vertexBuffer));
    GLCALL(glDeleteProgram(shader));
    GLCALL(glDisableVertexAttribArray(0));
}