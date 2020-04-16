// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2018, SICK AG, Waldkirch
*  Copyright (C) 2018, FZI Forschungszentrum Informatik, Karlsruhe, Germany
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
 * \file CommSettings.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATASTRUCTURE_COMMSETTINGS_H
#define SICK_SAFETYSCANNERS_BASE_DATASTRUCTURE_COMMSETTINGS_H

#include <bitset>
#include <boost/asio.hpp>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "sick_safetyscanners_base/Types.h"

namespace sick {
namespace datastructure {

/*!
 * \brief The communication settings which is used to configure the sensor.
 *
 */
struct CommSettings
{
  CommSettings() = default;

  // Aggregate Initialization, do not add constructors, default-initializers etc.
  // see https://en.cppreference.com/w/cpp/language/aggregate_initialization
  uint8_t channel{0};
  uint16_t publishing_frequency{1};
  uint8_t e_interface_type{0};
  float start_angle{0.0};
  float end_angle{0.0};
  sick::types::SensorFeatures features{sick::SensorDataFeatures::ALL};
  bool enabled{true};

  sick::types::port_t host_udp_port{0};
  sick::types::ip_address_t host_ip{boost::asio::ip::address_v4::from_string("192.168.1.100")};
};

std::ostream& operator<<(std::ostream& os, const CommSettings& settings);

} // namespace datastructure
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATASTRUCTURE_COMMSETTINGS_H
