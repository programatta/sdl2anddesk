#include "trace.hpp"

#if ANDROID
#include <android/log.h>
#else
#include <iostream>
#endif

void Trace::Info(std::string tag, std::string msg)
{
    #if ANDROID
    __android_log_print(ANDROID_LOG_VERBOSE, tag.c_str(), "%s\n", msg.c_str());
    #else
    std::cout << tag << " " << msg << std::endl;
    #endif
}