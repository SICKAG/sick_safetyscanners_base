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
 * \file ParseDataHeader.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATAHEADER_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATAHEADER_H


#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/DataHeader.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>

namespace sick {
namespace data_processing {

/*!
 * \brief Parser for the data header.
 *
 * Is required before the other complete udp packet parsers. It will return which data packages
 * are enabled and therefore the other parsers can be used accordingly.
 */
class ParseDataHeader
{
private:
  /*!
   * \brief  Constructor of the parser.
   */
  ParseDataHeader();

public:
  /*!
   * \brief Parses the data header from a udp sequence.
   *
   * \param buffer The complete udp packet.
   * \param data The already parsed data used for checks.
   *
   * \returns The parsed data header.
   */
  static datastructure::DataHeader parseUDPSequence(const datastructure::PacketBuffer& buffer,
                                                    datastructure::Data& data);

private:
  static void setVersionIndicatorInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                              datastructure::DataHeader& data_header);
  static void setMajorVersionInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                          datastructure::DataHeader& data_header);
  static void setMinorVersionInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                          datastructure::DataHeader& data_header);
  static void setVersionReleaseInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                            datastructure::DataHeader& data_header);
  static void setSerialNumberOfDeviceInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                  datastructure::DataHeader& data_header);
  static void setSerialNumberOfSystemPlugInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                      datastructure::DataHeader& data_header);
  static void setChannelNumberInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                           datastructure::DataHeader& data_header);
  static void setSequenceNumberInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                            datastructure::DataHeader& data_header);
  static void setScanNumberInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                        datastructure::DataHeader& data_header);
  static void setTimestampDateInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                           datastructure::DataHeader& data_header);
  static void setTimestampTimeInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                           datastructure::DataHeader& data_header);
  static void setGeneralSystemStateBlockOffsetInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                           datastructure::DataHeader& data_header);
  static void setGeneralSystemStateBlockSizeInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                         datastructure::DataHeader& data_header);
  static void setDerivedValuesBlockOffsetInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                      datastructure::DataHeader& data_header);
  static void setDerivedValuesBlockSizeInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                    datastructure::DataHeader& data_header);
  static void setMeasurementDataBlockOffsetInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                        datastructure::DataHeader& data_header);
  static void setMeasurementDataBlockSizeInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                      datastructure::DataHeader& data_header);
  static void setIntrusionDataBlockOffsetInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                      datastructure::DataHeader& data_header);
  static void setIntrusionDataBlockSizeInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                    datastructure::DataHeader& data_header);
  static void setApplicationDataBlockOffsetInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                        datastructure::DataHeader& data_header);
  static void setApplicationDataBlockSizeInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                                      datastructure::DataHeader& data_header);
  static void setDataInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                  datastructure::DataHeader& data_header);
  static void setVersionInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                     datastructure::DataHeader& data_header);
  static void setScanHeaderInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                        datastructure::DataHeader& data_header);
  static void setDataBlocksInDataHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                        datastructure::DataHeader& data_header);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATAHEADER_H
