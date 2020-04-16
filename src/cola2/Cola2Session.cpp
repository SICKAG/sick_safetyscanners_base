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
 * \file Cola2Session.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/cola2/Cola2Session.h"
#include "sick_safetyscanners_base/Types.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace sick {
namespace cola2 {

Cola2Session::Cola2Session(communication::TCPClientPtr tcp_client)
  : m_request_id(0)
  , m_session_id(boost::none)
  , m_tcp_client_ptr(std::move(tcp_client))
{
}

boost::optional<uint32_t> Cola2Session::getSessionID() const
{
  return m_session_id;
}

uint16_t Cola2Session::getNextRequestID()
{
  return ++m_request_id;
}

void Cola2Session::setSessionID(uint32_t session_id)
{
  if (m_session_id.is_initialized())
  {
    m_session_id.reset(session_id);
  }
  else
  {
    m_session_id.emplace(session_id);
  }
}

void Cola2Session::open()
{
  m_tcp_client_ptr->connect();
  assert(isOpen());
  CreateSession cmd(*this);
  sendCommand(cmd);
  auto sessID = cmd.getSessionID();
  setSessionID(sessID);
  LOG_DEBUG("Successfully opened Cola2 session with sessionID: %u", sessID);
}

bool Cola2Session::isOpen() const
{
  return m_tcp_client_ptr->isConnected();
}

void Cola2Session::close()
{
  if (!isOpen())
  {
    return;
  }

  CloseSession cmd(*this);
  sendCommand(cmd);
  m_tcp_client_ptr->disconnect();
  auto sessID = cmd.getSessionID();
  LOG_DEBUG("Closed Cola2 session with sessionID: %u", sessID);
}

void Cola2Session::assembleAndSendTelegram(Command& cmd)
{
  cmd.setSessionID(getSessionID().get_value_or(0));
  std::vector<uint8_t> telegram;
  telegram = cmd.constructTelegram(telegram);
  m_tcp_client_ptr->send(telegram);
}

sick::datastructure::PacketBuffer
Cola2Session::receiveAndProcessResponse(Command& cmd, boost::posix_time::time_duration timeout)
{
  sick::data_processing::TCPPacketMerger packet_merger(0);
  sick::data_processing::ParseTCPPacket tcp_packet_parser;

  while (!packet_merger.isComplete())
  {
    sick::datastructure::PacketBuffer packet_buffer = m_tcp_client_ptr->receive(timeout);
    if (packet_merger.isEmpty())
    {
      auto expectedPacketLength = tcp_packet_parser.getExpectedPacketLength(packet_buffer);
      packet_merger.setTargetSize(expectedPacketLength);
    }
    packet_merger.addTCPPacket(packet_buffer);
  }
  sick::datastructure::PacketBuffer response = packet_merger.getDeployedPacketBuffer();
  return response;
}

void Cola2Session::sendCommand(Command& cmd, boost::posix_time::time_duration timeout)
{
  if (!isOpen())
  {
    open();
  }

  assembleAndSendTelegram(cmd);
  auto response = receiveAndProcessResponse(cmd, timeout);
  cmd.processReplyBase(*response.getBuffer());
}

} // namespace cola2
} // namespace sick
