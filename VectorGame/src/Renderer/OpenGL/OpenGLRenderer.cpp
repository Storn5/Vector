#define _USE_MATH_DEFINES

#ifdef _DEBUG
#define GLCALL(x) testOpenGL();\
    x;\
    ASSERT(testOpenGL())
#else
#define GLCALL(x) x
#endif

#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Utils/Helpers.h"
#include "ShaderFile.h"
#include <sstream>
#include <fstream>

OpenGLRenderer::OpenGLRenderer(const GraphicsConfig& config)
    : m_config(config)
{
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Error while initializing GLEW\n");
        return;
    }

    m_startTime = std::chrono::high_resolution_clock::now();
}

OpenGLRenderer::~OpenGLRenderer()
{
    
}

bool OpenGLRenderer::testOpenGL()
{
    bool isOk = true;
    GLenum glError;
    unsigned char maxAttempts = 10;
    while ((glError = glGetError()) != GL_NO_ERROR && maxAttempts--)
    {
        isOk = false;
        fprintf(stderr, "OpenGL error\n");
        switch (glError)
        {
        case GL_STACK_OVERFLOW:
            fprintf(stderr, "Stack overflow\n"); break;
        case GL_STACK_UNDERFLOW:
            fprintf(stderr, "Stack underflow\n"); break;
        case GL_OUT_OF_MEMORY:
            fprintf(stderr, "There is not enough memory left to execute the command\n"); break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            fprintf(stderr, "The framebuffer object is not complete\n"); break;
        case GL_INVALID_OPERATION:
            fprintf(stderr, "The specified operation is not allowed in the current state\n"); break;
        case GL_INVALID_VALUE:
            fprintf(stderr, "A numeric argument is out of range\n"); break;
        case GL_INVALID_ENUM:
            fprintf(stderr, "An unacceptable value is specified for an enum argument\n"); break;
        }
    }
    return isOk;
}

unsigned int OpenGLRenderer::createShader(const GLenum shaderType, const std::string& shaderSource)
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

unsigned int OpenGLRenderer::createShaderProgram(ShaderFile file)
{
    GLuint program = glCreateProgram();
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, file.vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, file.fragmentShaderSource);
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

ShaderFile OpenGLRenderer::parseShaderFile(const std::string& filename)
{
    std::ifstream filestream(filename);

    if (!filestream)
    {
        fprintf(stderr, "Error parsing shader: file %s cannot be read\n", filename.c_str());
        return ShaderFile
        {
            NULL, NULL
        };
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
                return ShaderFile
                {
                    NULL, NULL
                };
            }
        }
        else
            ss[(int)type] << line << "\n";
    }

    return ShaderFile
    {
        ss[0].str(), ss[1].str()
    };
}

bool OpenGLRenderer::isInitialized()
{
    if (!testOpenGL())
        return false;

    return true;
}

bool OpenGLRenderer::renderFrame()
{
    auto timeDiff = secondsBetween(m_startTime, std::chrono::high_resolution_clock::now());

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
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    auto shaderFile = parseShaderFile("res/Shaders/Shader.glsl");
    GLuint shader = createShaderProgram(shaderFile);
    GLCALL(glUseProgram(shader));

    GLCALL(GLint shaderLocation = glGetUniformLocation(shader, "u_Color"));

    float triangleVertices[10] =
    {
        0.0f, 0.0f,
        0.0f, 0.3f * abs(sin(timeDiff)),
        0.3f * abs(sin(timeDiff)), 0.0f,
        0.0f, -0.3f * abs(sin(timeDiff)),
        -0.3f * abs(sin(timeDiff)), 0.0f
    };
    GLCALL(glBindVertexArray(vao));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 2 * num_vertices * sizeof(float), triangleVertices, GL_DYNAMIC_DRAW));
    GLCALL(glUniform4f(shaderLocation, abs(sin(timeDiff * 2)), abs(cos(timeDiff)), abs(sin((timeDiff / 2) + M_PI)), 1.0f));

    // Render here
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));

    GLCALL(glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, nullptr));

    GLCALL(glDeleteBuffers(1, &indexBuffer));
    GLCALL(glDeleteBuffers(1, &vertexBuffer));
    GLCALL(glDeleteProgram(shader));
    GLCALL(glDisableVertexAttribArray(0));

    return true;
}