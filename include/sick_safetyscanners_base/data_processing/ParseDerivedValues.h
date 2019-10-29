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
 * \file ParseDerivedValues.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDERIVEDVALUES_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDERIVEDVALUES_H

#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/DerivedValues.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>

namespace sick {
namespace data_processing {

/*!
 * \brief Parser for the derived values from the udp data packets.
 */
class ParseDerivedValues
{
private:
  /*!
   * \brief Constructor of the parser.
   */
  ParseDerivedValues();

public:
  /*!
   * \brief Parsed the packet buffer and returns the derived values.
   *
   * \param buffer The incoming packet buffer.
   * \param data The already parsed data. Used for checks if the derived values are enabled.
   *
   * \returns The parsed derived values.
   */
  static datastructure::DerivedValues parseUDPSequence(const datastructure::PacketBuffer& buffer,
                                                       datastructure::Data& data);

private:
  static void setDataInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                     datastructure::DerivedValues& derived_values);
  static void setMultiplicationFactorInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                                     datastructure::DerivedValues& derived_values);
  static void setNumberOfBeamsInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                              datastructure::DerivedValues& derived_values);
  static void setScanTimeInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                         datastructure::DerivedValues& derived_values);
  static void setStartAngleInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                           datastructure::DerivedValues& derived_values);
  static void setAngularBeamResolutionInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                                      datastructure::DerivedValues& derived_values);
  static void setInterbeamPeriodInDerivedValues(std::vector<uint8_t>::const_iterator data_ptr,
                                                datastructure::DerivedValues& derived_values);
  static bool checkIfPreconditionsAreMet(const datastructure::Data& data);
  static bool checkIfDerivedValuesIsPublished(const datastructure::Data& data);
  static bool checkIfDataContainsNeededParsedBlocks(const datastructure::Data& data);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDERIVEDVALUES_H
