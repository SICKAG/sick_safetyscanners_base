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
 * \file Cola2Session.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H
#define SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H

#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"

#include "sick_safetyscanners_base/communication/AsyncTCPClient.h"
#include "sick_safetyscanners_base/communication/TCPClient.h"

#include "sick_safetyscanners_base/cola2/CloseSession.h"
#include "sick_safetyscanners_base/cola2/Command.h"
#include "sick_safetyscanners_base/cola2/CreateSession.h"

#include "sick_safetyscanners_base/data_processing/ParseTCPPacket.h"
#include "sick_safetyscanners_base/data_processing/TCPPacketMerger.h"

#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <chrono>
#include <limits>
#include <map>
#include <mutex>

namespace sick
{
namespace cola2
{

class CommandMsg;
class CreateSession;

class Cola2Session
{
public:
  explicit Cola2Session(communication::TCPClientPtr tcp_client);
  Cola2Session() = delete;
  Cola2Session(const Cola2Session &) = delete;
  Cola2Session &operator=(const Cola2Session &) = delete;

  void executeCommand(CommandMsg &cmd, long int timeout_ms = 2000);

  boost::optional<uint32_t> getSessionID() const;
  uint16_t getNextRequestID();
  void setSessionID(uint32_t session_id);

private:
  uint16_t m_request_id_;
  boost::optional<uint32_t> m_session_id_;
  communication::TCPClientPtr m_tcp_client_ptr_;

  void open();
  void close();
  void sendRequest(CommandMsg &cmd);
  bool isOpen();
  sick::datastructure::PacketBuffer waitAndProcessResponse(CommandMsg &cmd, int64_t timeout_ms);
};

} // namespace cola2
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H
