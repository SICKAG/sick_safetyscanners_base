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
 * \file ParseTCPPacket.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSETCPPACKET_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSETCPPACKET_H

#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/DerivedValues.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>

namespace sick {

namespace cola2 {
/*!
 * \brief Forward declaration of Command class.
 */
class Command;
} // namespace cola2

namespace data_processing {


/*!
 * \brief Parser for an incoming TCP packet.
 */
class ParseTCPPacket
{
private:
  /*!
   * \brief Constructor of parser.
   */
  ParseTCPPacket();

public:
  /*!
   * \brief Parse the tcp sequence to get the header information of the cola2 protocol.
   *
   * \param buffer The incoming tcp connection.
   * \param command Reference to the command and set the returned method type and mode and the data.
   *
   * \returns If parsing was successful.
   */
  static bool parseTCPSequence(const datastructure::PacketBuffer& buffer,
                               sick::cola2::Command& command);

  /*!
   * \brief Gets the expected packet length for a buffer.
   *
   * \param buffer The incoming tcp packet.
   *
   * \returns Expected length of the incoming packet buffer.
   */
  static uint32_t getExpectedPacketLength(const datastructure::PacketBuffer& buffer);

  /*!
   * \brief Gets the request ID of the incoming tcp packet.
   *
   * \param buffer The incoming tcp packet.
   *
   * \returns The request ID of the incoming packet buffer.
   */
  static uint16_t getRequestID(const datastructure::PacketBuffer& buffer);

private:
  static uint32_t readSTx(std::vector<uint8_t>::const_iterator data_ptr);
  static uint32_t readLength(std::vector<uint8_t>::const_iterator data_ptr);
  static uint16_t readRequestID(std::vector<uint8_t>::const_iterator data_ptr);
  static uint8_t readHubCntr(std::vector<uint8_t>::const_iterator data_ptr);
  static uint8_t readNoC(std::vector<uint8_t>::const_iterator data_ptr);
  static uint32_t readSessionID(std::vector<uint8_t>::const_iterator data_ptr);
  static uint8_t readCommandType(std::vector<uint8_t>::const_iterator data_ptr);
  static uint8_t readCommandMode(std::vector<uint8_t>::const_iterator data_ptr);
  static uint16_t readErrorCode(std::vector<uint8_t>::const_iterator data_ptr);
  static std::vector<uint8_t> readData(const datastructure::PacketBuffer& buffer);
  static void setCommandValuesFromPacket(const sick::datastructure::PacketBuffer& buffer,
                                         sick::cola2::Command& command);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSETCPPACKET_H
