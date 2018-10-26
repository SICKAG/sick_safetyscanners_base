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
 * \file TCPPacketMerger.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef TCPPACKETMERGER_H
#define TCPPACKETMERGER_H

#include <sick_microscan3_ros_driver/datastructure/PacketBuffer.h>

#include <sick_microscan3_ros_driver/data_processing/ParseDatagramHeader.h>

namespace sick {
namespace data_processing {

class TCPPacketMerger
{
public:
  TCPPacketMerger();

  bool isComplete() const;
  bool isEmpty() const;

  bool addTCPPacket(const sick::datastructure::PacketBuffer& buffer);
  sick::datastructure::PacketBuffer getDeployedPacketBuffer();
  uint32_t getTargetSize() const;
  void setTargetSize(const uint32_t& targetSize);

private:
  bool m_is_complete;
  sick::datastructure::PacketBuffer m_deployed_packet_buffer;

  std::vector<sick::datastructure::PacketBuffer> m_buffer_vector;
  uint32_t m_targetSize;

  bool addToMap(const sick::datastructure::PacketBuffer& newPacket);
  bool deployPacketIfComplete();

  uint32_t getCurrentSize() const;
  bool deployPacket();
};

} // namespace data_processing
} // namespace sick

#endif
