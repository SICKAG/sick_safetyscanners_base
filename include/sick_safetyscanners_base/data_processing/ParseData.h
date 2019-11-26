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
 * \file ParseData.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATA_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATA_H

#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/data_processing/ParseApplicationData.h>
#include <sick_safetyscanners_base/data_processing/ParseDataHeader.h>
#include <sick_safetyscanners_base/data_processing/ParseDerivedValues.h>
#include <sick_safetyscanners_base/data_processing/ParseGeneralSystemState.h>
#include <sick_safetyscanners_base/data_processing/ParseIntrusionData.h>
#include <sick_safetyscanners_base/data_processing/ParseMeasurementData.h>
#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>


namespace sick {
namespace data_processing {

/*!
 * \brief Parses the udp data packets depending on which data will be received.
 */
class ParseData
{
private:
  /*!
   * \brief Constructor of the parser.
   */
  ParseData();

public:
  /*!
   * \brief Parses the udp data transferred in the packet buffer. It will be parsed into the data
   * reference.
   *
   * \param buffer The incoming data buffer.
   *
   * \returns Parsed data
   */
  static sick::datastructure::Data parseUDPSequence(const sick::datastructure::PacketBuffer& buffer);

  static bool parseTCPSequence(const datastructure::PacketBuffer& buffer,
                               sick::datastructure::Data& data);

private:
  static void setDataBlocksInData(const datastructure::PacketBuffer& buffer,
                                  datastructure::Data& data);
  static void setDataHeaderInData(const datastructure::PacketBuffer& buffer,
                                  datastructure::Data& data);
  static void setDerivedValuesInData(const datastructure::PacketBuffer& buffer,
                                     datastructure::Data& data);
  static void setMeasurementDataInData(const datastructure::PacketBuffer& buffer,
                                       datastructure::Data& data);
  static void setGeneralSystemStateInData(const datastructure::PacketBuffer& buffer,
                                          datastructure::Data& data);
  static void setIntrusionDataInData(const datastructure::PacketBuffer& buffer,
                                     datastructure::Data& data);
  static void setApplicationDataInData(const datastructure::PacketBuffer& buffer,
                                       datastructure::Data& data);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATA_H
