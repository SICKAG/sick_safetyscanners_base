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
 * \file ParseDatagramHeader.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATAGRAMHEADER_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATAGRAMHEADER_H

#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/DatagramHeader.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>

namespace sick {
namespace data_processing {

/*!
 * \brief Parser for the datagram header.
 *
 * Parses the datagram header to match the udp packets together so the complete datapacket can be
 * used in further parsing steps. Uses the raw data from the udp packets.
 */
class ParseDatagramHeader
{
private:
  /*!
   * \brief Constructor of the parser.
   */
  ParseDatagramHeader();

public:
  /*!
   * \brief Parses the udp sequence to get the identification and the offset for the datagram
   * header.
   *
   * \param buffer The incoming udp datapackets.
   * \param header The parsed datagram header.
   *
   * \returns If parsing the datagram header was successful.
   */
  static bool parseUDPSequence(const datastructure::PacketBuffer& buffer,
                               sick::datastructure::DatagramHeader& header);

private:
  static void setDataInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                              datastructure::DatagramHeader& header);

  static void setDatagramMarkerInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                        datastructure::DatagramHeader& header);
  static void setProtocolInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                  datastructure::DatagramHeader& header);
  static void setMajorVersionInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                      datastructure::DatagramHeader& header);
  static void setMinorVersionInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                      datastructure::DatagramHeader& header);
  static void setTotalLengthInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                     datastructure::DatagramHeader& header);
  static void setIdentificationInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                        datastructure::DatagramHeader& header);
  static void setFragmentOffsetInHeader(std::vector<uint8_t>::const_iterator data_ptr,
                                        datastructure::DatagramHeader& header);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSEDATAGRAMHEADER_H
