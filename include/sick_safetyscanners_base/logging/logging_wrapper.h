#ifndef SICK_SAFETYSCANNERS_BASE_LOGGING_LOGGINGWRAPPER_H
#define SICK_SAFETYSCANNERS_BASE_LOGGING_LOGGINGWRAPPER_H

#ifdef NO_LOGGING

#define ROS_INFO(...) do{} while(false)
#define ROS_WARN(...) do{} while(false)
#define ROS_ERROR(...) do{} while(false)

#elif NO_ROS_LOGGING
// clang-format off
#include <cstdio>

#define ROS_INFO(...) std::printf(__VA_ARGS__); std::printf("\n"); fflush(stdout)
#define ROS_WARN(...) std::printf(__VA_ARGS__); std::printf("\n"); fflush(stdout)
#define ROS_ERROR(...) std::fprintf(stderr, __VA_ARGS__); std::fprintf(stderr, "\n"); fflush(stderr)

#else

#include <ros/console.h>

//clang-format on

#endif // NO_ROS_LOGGING

#endif // SICK_SAFETYSCANNERS_BASE_LOGGING_LOGGINGWRAPPER_H
