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
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/datastructure/CommSettings.h"
#include <sstream>

namespace sick
{
namespace datastructure
{

std::ostream &operator<<(std::ostream &os, const CommSettings &settings)
{
  std::string channel_enabled{settings.enabled ? "true" : "false"};
  os << "channel = " << static_cast<int>(settings.channel) << "\r\n"
     << "enabled = " << channel_enabled << "\r\n"
     << "publishing_frequency = " << settings.publishing_frequency << "\r\n"
     << "e_interface_type = " << settings.e_interface_type << "\r\n"
     << "start_angle = " << settings.start_angle << "\r\n"
     << "end_angle = " << settings.end_angle << "\r\n"
     << "host_ip = " << settings.host_ip.to_string() << "\r\n"
     << "host_udp_port = " << settings.host_udp_port << "\r\n"
     << "features (as int) = " << settings.features << "\r\n"
     << "features.ApplicationData = " << sick::SensorDataChannels::isFlagSet(settings.features, sick::SensorDataChannels::APPLICATION_DATA) << "\r\n"
     << "features.DerivedValues = " << sick::SensorDataChannels::isFlagSet(settings.features, sick::SensorDataChannels::DERIVED_SETTINGS) << "\r\n"
     << "features.GeneralSystemState = " << sick::SensorDataChannels::isFlagSet(settings.features, sick::SensorDataChannels::GENERAL_SYSTEM_STATE) << "\r\n"
     << "features.IntrusionData = " << sick::SensorDataChannels::isFlagSet(settings.features, sick::SensorDataChannels::INTRUSION_DATA) << "\r\n"
     << "features.MeasurementData = " << sick::SensorDataChannels::isFlagSet(settings.features, sick::SensorDataChannels::MEASUREMENT_DATA) << "\r\n";
  return os;
}

} // namespace datastructure
} // namespace sick
