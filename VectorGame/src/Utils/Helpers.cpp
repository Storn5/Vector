#include <fstream>
#include <sstream>
#include "Utils/Helpers.h"
#include "ShaderFile.h"

double secondsBetween(const std::chrono::steady_clock::time_point& point1, const std::chrono::steady_clock::time_point& point2)
{
    return (point2 - point1).count() / 1000000000.0;
}

void logErrorLine(const char* function, const char* file, int line)
{
    fprintf(stderr, "File %s:%d\n%s\n", file, line, function);
}

ShaderFile parseShaderFile(const std::string& filename)
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