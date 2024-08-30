#pragma once

#include <string>

struct ShaderFile
{
	const std::string vertexShaderSource, fragmentShaderSource;
};

enum class ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};