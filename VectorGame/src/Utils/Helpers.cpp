#include "Utils/Helpers.h"

double secondsBetween(const std::chrono::steady_clock::time_point& point1, const std::chrono::steady_clock::time_point& point2)
{
    return (point2 - point1).count() / 1000000000.0;
}

void logErrorLine(const char* function, const char* file, int line)
{
    fprintf(stderr, "File %s:%d\n%s\n", file, line, function);
}