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
 * \file types.h
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <memory>
#include <boost/asio/ip/address_v4.hpp>
#include "sick_safetyscanners_base/datastructure/Data.h"
#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"
#include "sick_safetyscanners_base/generics.h"

namespace sick
{
namespace types
{

template <typename T>
using CallbackT = std::function<void(T)>;

using ScanDataCb = std::function<void(const sick::datastructure::Data &)>;
using PacketHandler = std::function<void(const sick::datastructure::PacketBuffer &)>;
using ip_address_t = boost::asio::ip::address_v4;
using port_t = uint16_t;
using time_duration_t = boost::posix_time::time_duration;
using SensorFeatures = uint16_t;
} // namespace types

namespace SensorDataChannels
{
constexpr uint16_t ALL = 0b11111;
constexpr uint16_t NONE = 0;
constexpr uint16_t GENERAL_SYSTEM_STATE = 1 << 0;
constexpr uint16_t DERIVED_SETTINGS = 1 << 1;
constexpr uint16_t MEASUREMENT_DATA = 1 << 2;
constexpr uint16_t INTRUSION_DATA = 1 << 3;
constexpr uint16_t APPLICATION_DATA = 1 << 4;

constexpr bool isFlagSet(uint16_t channels, uint16_t flag)
{
    return (channels & flag) > 0;
};

constexpr uint16_t toFeatureFlags(
    bool general_system_state,
    bool derived_settings,
    bool measurement_data,
    bool intursion_data,
    bool application_data)
{
    // In C++11 constexpr functions are restricted to a function-body that contains a single return-statement only.
    // bool * int gets promoted to either 1 * int or 0 * int, so this is works to avoid if-statements.
    return (general_system_state * sick::SensorDataChannels::GENERAL_SYSTEM_STATE) +
    (derived_settings * sick::SensorDataChannels::DERIVED_SETTINGS) +
    (measurement_data * sick::SensorDataChannels::MEASUREMENT_DATA) +
    (intursion_data * sick::SensorDataChannels::INTRUSION_DATA) +
    (application_data * sick::SensorDataChannels::APPLICATION_DATA);
}

} // namespace SensorDataChannels

// enum class SensorDataChannels : uint16_t
// {
//     SYSTEM_STATE,
//     DERIVED_SETTINGS,
//     MEASUREMENT_DATA,
//     INTRUSION_DATA,
//     APPLICATION_DATA,
//     _ // Sentinel to restrict the range of possible domain values
// };

// using SensorFeatures = flag_set<SensorDataChannels>;

} // namespace sick

// ENABLE_BITMASK_OPERATORS(sick::SensorFeatures);

#endif // SICK_SAFETYSCANNERS_TYPES_H