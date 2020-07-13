#include "utils.hpp"
#include <sstream>

std::string Utils::ResourcePath(std::string resourceName)
{
    std::stringstream path;
    
    #if !ANDROID
        path << "assets" << "/";
    #endif

    path << resourceName;
    return path.str();
}