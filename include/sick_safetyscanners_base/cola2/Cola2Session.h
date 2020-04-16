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
 * \file    Cola2Session.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H
#define SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H

#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"

#include "sick_safetyscanners_base/communication/TCPClient.h"

#include "sick_safetyscanners_base/cola2/CloseSession.h"
#include "sick_safetyscanners_base/cola2/Command.h"
#include "sick_safetyscanners_base/cola2/CreateSession.h"

#include "sick_safetyscanners_base/data_processing/ParseTCPPacket.h"
#include "sick_safetyscanners_base/data_processing/TCPPacketMerger.h"

#include <boost/optional.hpp>

namespace sick {
namespace cola2 {

class Command;
class CreateSession;

/*!
 * \brief A class representing a COLA2 session to handle send, receive and process telegrams
 * synchronously in the format as specified by the SICK Cola2 protocol. This class is not
 * thread-safe and works strictly sequential (one command after another).
 *
 */
class Cola2Session
{
public:
  /*!
   * \brief Constructor of the Cola 2 Session object.
   *
   * \param tcp_client Pointer on a TCPClient object.
   */
  explicit Cola2Session(communication::TCPClientPtr tcp_client);

  // Deleted default, copy and copy-assignment constructors.
  Cola2Session()                    = delete;
  Cola2Session(const Cola2Session&) = delete;
  Cola2Session& operator=(const Cola2Session&) = delete;

  /*!
   * \brief A blocking operation sends a COLA2 command as telegram to the connected sensor. Throws
   * an exception if the timeout is exceeded.
   *
   * \param cmd The command to send via COLA2 to the sensor.
   * \param timeout The timeout on a single send/receive socket operation. This is not the overall
   * timeout limit but guarantees the synchronous send/receive operations not to wait forever.
   */
  void sendCommand(Command& cmd,
                   sick::types::time_duration_t timeout = boost::posix_time::seconds(5));

  /*!
   * \brief Get the current session ID, if available.
   *
   * \return boost::optional<uint32_t> Returns the sessionID wrapped as optional value.
   */
  boost::optional<uint32_t> getSessionID() const;

  /*!
   * \brief Get the next possible requestID. This increments the internal counter by one.
   *
   * \return uint16_t Returns an incremented request ID.
   */
  uint16_t getNextRequestID();

  /*!
   * \brief Set the Session I D object
   *
   * \param session_id
   */
  void setSessionID(uint32_t session_id);

private:
  uint16_t m_request_id;
  boost::optional<uint32_t> m_session_id;
  communication::TCPClientPtr m_tcp_client_ptr;

  /*!
   * \brief Opens a COLA2 session.
   *
   */
  void open();

  /*!
   * \brief Closes the current COLA2 session. If no session is opened, the function will return
   * silently without raising an exception.
   *
   */
  void close();

  /*!
   * \brief Creates and transmits a telegram in COLA2 format to the sensor.
   *
   * \param cmd The command to be processed.
   */
  void assembleAndSendTelegram(Command& cmd);

  /*!
   * \brief Indicates whether a COLA2 session is currently opened.
   *
   * \return true The COLA2 session is open.
   * \return false The COLA2 session is closed.
   */
  bool isOpen() const;

  /*!
   * \brief Assembles a packetBuffer object by merging TCP packets from one or multiple response
   * telegrams of the sensor.
   *
   * \param cmd The command to be processed.
   * \param timeout The timeout on a single send/receive socket operation. This is not the overall
   * timeout limit but guarantees the synchronous send/receive operations not to wait forever.
   * \return sick::datastructure::PacketBuffer The merged reply telegrams of the sensor.
   */
  sick::datastructure::PacketBuffer receiveAndProcessResponse(Command& cmd,
                                                              sick::types::time_duration_t timeout);
};

} // namespace cola2
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H
