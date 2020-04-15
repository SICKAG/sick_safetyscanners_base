// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2020, SICK AG, Waldkirch
*  Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.

*/

// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!
 * \file exceptions.h
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

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
    explicit timeout_error(const std::string &msg, boost::posix_time::time_duration timeout) {
        std::stringstream ss;
        ss << msg << "[timeout: " << timeout.total_seconds() + timeout.total_milliseconds() * 1e-3 << "seconds ]";
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