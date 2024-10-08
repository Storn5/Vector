#pragma once

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) { logErrorLine(#x, __FILE__, __LINE__); __debugbreak(); }
#else
#define ASSERT(x) x
#endif

#include <chrono>

double secondsBetween(const std::chrono::steady_clock::time_point& point1, const std::chrono::steady_clock::time_point& point2);
void logErrorLine(const char* function, const char* file, int line);