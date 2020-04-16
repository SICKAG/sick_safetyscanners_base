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
 * \file Types.h
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

/*!
 * \brief Type definition for scan data callbacks.
 * 
 */
using ScanDataCb = std::function<void(const sick::datastructure::Data &)>;

/*!
 * \brief Type definition for packet handler callbacks.
 * 
 */
using PacketHandler = std::function<void(const sick::datastructure::PacketBuffer &)>;

/*!
 * \brief Type alias for the currently used IP4 address type.
 * 
 */
using ip_address_t = boost::asio::ip::address_v4;

/*!
 * \brief Type alias for the currently used TCP/UDP port type.
 * 
 */
using port_t = uint16_t;

/*!
 * \brief Type alias for the currently used.
 * 
 */
using time_duration_t = boost::posix_time::time_duration;

/*!
 * \brief Type alias for the sensor feature flags.
 * 
 */
using SensorFeatures = uint16_t;
} // namespace types

// Namespace containing sensor feature flags as constants.
namespace SensorDataFeatures
{
constexpr uint16_t ALL = 0b11111;
constexpr uint16_t NONE = 0;
constexpr uint16_t GENERAL_SYSTEM_STATE = 1 << 0;
constexpr uint16_t DERIVED_SETTINGS = 1 << 1;
constexpr uint16_t MEASUREMENT_DATA = 1 << 2;
constexpr uint16_t INTRUSION_DATA = 1 << 3;
constexpr uint16_t APPLICATION_DATA = 1 << 4;

/*!
 * \brief Helper function to check if a certain bit flag is set.
 * 
 * \param bitset The bitset expressed as integer.
 * \param flag The bitflag expressed as integer. 
 * \return true The given flag is set in the bitset.
 * \return false The given flag is not set in the bitset.
 */
constexpr bool isFlagSet(uint16_t bitset, uint16_t flag)
{
    return (bitset & flag) == flag;
};

/*!
 * \brief Converts boolean indicators (for the sensor features to be streamed) into a bitset (expressed as integer).
 * 
 * \param general_system_state Indicator for the general system state channel.
 * \param derived_settings Turn on/off derived values.
 * \param measurement_data Turn on/off measurement data.
 * \param intursion_data Turn on/off safety field intrusion data.
 * \param application_data Turn on/off application data.
 * \return constexpr uint16_t A bitset containing indication flags for each feature channel.
 */
constexpr uint16_t toFeatureFlags(
    bool general_system_state,
    bool derived_settings,
    bool measurement_data,
    bool intursion_data,
    bool application_data)
{
    // In C++11 constexpr functions are restricted to a function-body that contains a single return-statement only.
    // bool * int gets promoted to either 1 * int or 0 * int, so this is works to avoid if-statements.
    return (general_system_state * sick::SensorDataFeatures::GENERAL_SYSTEM_STATE) +
           (derived_settings * sick::SensorDataFeatures::DERIVED_SETTINGS) +
           (measurement_data * sick::SensorDataFeatures::MEASUREMENT_DATA) +
           (intursion_data * sick::SensorDataFeatures::INTRUSION_DATA) +
           (application_data * sick::SensorDataFeatures::APPLICATION_DATA);
}

} // namespace SensorDataFeatures
} // namespace sick

#endif // SICK_SAFETYSCANNERS_TYPES_H