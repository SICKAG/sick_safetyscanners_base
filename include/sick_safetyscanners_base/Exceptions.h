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
 * \file    Exceptions.h
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_EXCEPTIONS_H
#define SICK_SAFETYSCANNERS_BASE_EXCEPTIONS_H

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <chrono>
#include <exception>
#include <sstream>
#include <stdexcept>

namespace sick {

/*!
 * \brief Represents a generic runtime error.
 *
 */
class runtime_error : public std::runtime_error
{
public:
  /*!
   * \brief The default constructor of a runtime error object. The message string is kept empty.
   *
   */
  explicit runtime_error()
    : runtime_error(""){};

  /*!
   * \brief Constructor of a runtime error object
   *
   * \param what_arg A description of the reason for the failure.
   */
  explicit runtime_error(const std::string& what_arg)
    : std::runtime_error(what_arg){};

  /*!
   * \brief Constructor of a runtime error object
   *
   * \param what_arg A description of the reason for the failure.
   */
  explicit runtime_error(const char* what_arg)
    : std::runtime_error(what_arg){};

  /*!
   * \brief Destructor of the runtime error object
   *
   */
  virtual ~runtime_error() = default;
};

/*!
 * \brief A device configuration error.
 *
 */
class configuration_error : public runtime_error
{
public:
  /*!
   * \brief The default constructor of the configuration error object.
   *
   */
  explicit configuration_error()
    : runtime_error(""){};

  /*!
   * \brief Constructor of the configuration error object.
   *
   * \param what_arg A description of the reason for the failure.
   */
  explicit configuration_error(const std::string& what_arg)
    : runtime_error(what_arg){};

  /*!
   * \brief Constructor of the configuration error object.
   *
   * \param what_arg A description of the reason for the failure.
   */
  explicit configuration_error(const char* what_arg)
    : runtime_error(what_arg){};
};

// class unsupported_device : public runtime_error
// {
// public:
//     explicit unsupported_device() : runtime_error(""){};
//     explicit unsupported_device(const std::string &msg) : runtime_error(msg){};
// };

/*!
 * \brief A timeout error during I/O related operations.
 *
 */
class timeout_error : public runtime_error
{
public:
  /*!
   * \brief Deleted default constructor of the timeout error object.
   *
   */
  explicit timeout_error() = delete;

  /*!
   * \brief Constructor of the timeout error object
   *
   * \param msg A description of the reason for the failure.
   * \param timeout The timeout that has been exceeded represented as timeval-struct. The timeout
   * information is appended as string after the message string.
   */
  explicit timeout_error(const std::string& msg, timeval timeout)
    : runtime_error(msg)
  {
    std::stringstream ss;
    ss << msg << " (timeout was set to " << timeout.tv_sec + timeout.tv_usec * 1e-6 << " sec)";
    msg_ = ss.str();
  }

  /*!
   * \brief Constructor of the timeout error object
   *
   * \param msg A description of the reason for the failure.
   * \param timeout The timeout that has been exceeded represented in
   * boost::posix_time::timeduration. The timeout information is appended as string after the
   * message string.
   */
  explicit timeout_error(const std::string& msg, boost::posix_time::time_duration timeout)
  {
    std::stringstream ss;
    ss << msg << " [timeout: " << timeout.total_milliseconds() * 1e-3
       << "seconds]";
    msg_ = ss.str();
  }

  /*!
   * \brief Returns the error message as char-string.
   *
   * \return const char* Returns the error message.
   */
  virtual const char* what() const noexcept override { return msg_.c_str(); }

private:
  std::string msg_;
};
} // namespace sick
#endif // ifndef SICK_SAFETYSCANNERS_BASE_EXCEPTIONS_H