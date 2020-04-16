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

namespace sick {

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