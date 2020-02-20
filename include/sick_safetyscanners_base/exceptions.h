#ifndef SICK_SAFETYSCANNERS_BASE_EXCEPTIONS_H
#define SICK_SAFETYSCANNERS_BASE_EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <sstream>
#include <chrono>

namespace sick
{
class error : public std::runtime_error
{
public:
    explicit error() : runtime_error(""){};
    explicit error(const std::string &what_arg) : std::runtime_error(what_arg){};
    explicit error(const char *what_arg) : std::runtime_error(what_arg){};

    virtual ~error() = default;
};

class configuration_error : public error
{
public:
    explicit configuration_error() : error(""){};
    explicit configuration_error(const std::string &what_arg) : error(what_arg){};
    explicit configuration_error(const char *what_arg) : error(what_arg){};
};

class unsupported_device : public error
{
public:
    explicit unsupported_device() : error(""){};
    explicit unsupported_device(const std::string &msg) : error(msg){};
};

class timeout_error : public error
{
public:
    explicit timeout_error() = delete;
    explicit timeout_error(const std::string &msg, timeval timeout) : error(msg)
    {
        std::stringstream ss;
        ss << msg << " (timeout was set to " << timeout.tv_sec + timeout.tv_usec * 1e-6 << " sec)";
        msg_ = ss.str();
    }

    virtual const char *what() const noexcept override
    {
        return msg_.c_str();
    }

private:
    std::string msg_;
};
} // namespace sick
#endif // ifndef SICK_SAFETYSCANNERS_BASE_EXCEPTIONS_H