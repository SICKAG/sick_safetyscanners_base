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
 * \file ParseIntrusionData.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEINTRUSIONDATA_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEINTRUSIONDATA_H

#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/DerivedValues.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>

#include <vector>

namespace sick {
namespace data_processing {

/*!
 * \brief Parser to read the intrusion data from a udp sequence.
 */
class ParseIntrusionData
{
private:
  /*!
   * \brief Constructor of the parser.
   */
  ParseIntrusionData();

public:
  /*!
   * \brief Parse a udp sequence to read the intrusion data if enabled.
   *
   * \param buffer The incoming sequence.
   * \param data The already parsed data to check if intrusion data is enabled.
   *
   * \returns The parsed intrusion data.
   */
  static datastructure::IntrusionData parseUDPSequence(const datastructure::PacketBuffer& buffer,
                                                       datastructure::Data& data);

private:
  static void setDataInIntrusionData(const uint16_t num_scan_points,
                                     std::vector<uint8_t>::const_iterator data_ptr,
                                     datastructure::IntrusionData& intrusion_data);
  static void setDataInIntrusionDatums(
      uint16_t num_scan_points,
      std::vector<uint8_t>::const_iterator data_ptr,
      std::vector<sick::datastructure::IntrusionDatum>& intrusion_datums);
  static uint16_t setSizeInIntrusionDatum(const uint16_t& offset,
                                          std::vector<uint8_t>::const_iterator data_ptr,
                                          sick::datastructure::IntrusionDatum& datum);
  static uint16_t setFlagsInIntrusionDatum(const uint16_t offset,
                                           const uint16_t num_scan_points,
                                           std::vector<uint8_t>::const_iterator data_ptr,
                                           sick::datastructure::IntrusionDatum& datum);
  static bool checkIfPreconditionsAreMet(const datastructure::Data& data);
  static bool checkIfIntrusionDataIsPublished(const datastructure::Data& data);
  static bool checkIfDataContainsNeededParsedBlocks(const datastructure::Data& data);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEINTRUSIONDATA_H
