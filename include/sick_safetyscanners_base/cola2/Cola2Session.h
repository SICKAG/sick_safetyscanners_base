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

#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/communication/AsyncTCPClient.h>
#include <sick_safetyscanners_base/communication/TCPClient.h>

#include <sick_safetyscanners_base/cola2/CloseSession.h>
#include <sick_safetyscanners_base/cola2/Command.h>
#include <sick_safetyscanners_base/cola2/CreateSession.h>

#include <sick_safetyscanners_base/data_processing/ParseTCPPacket.h>
#include <sick_safetyscanners_base/data_processing/TCPPacketMerger.h>

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

// enum class SessionState : uint8_t
// {
//     CLOSED,
//     OPENED
// };

class Cola2Session
{
public:
  //TODO pass general TCPClient?
  explicit Cola2Session(communication::TCPClientPtr tcp_client);
  Cola2Session() = delete;
  Cola2Session(const Cola2Session &) = delete;
  Cola2Session &operator=(const Cola2Session &) = delete;

  // Synchronous interface
  void executeCommand(CommandMsg &cmd, ulong timeout_ms = 2000);
  void tick();

  void open();
  void close();

  // Asynchronous interface
  void executeCommandAsync(CommandMsg &cmd);
  // void sendCommandAsync(Command &cmd);

  boost::optional<uint32_t> getSessionID() const;

private:
  uint16_t getNextRequestID();
  void setSessionID(uint32_t session_id);

  uint16_t m_request_id_{0};
  boost::mutex m_execution_mutex_{};
  boost::optional<uint32_t> m_session_id_{boost::none};
  // std::unique_ptr<sick::data_processing::TCPPacketMerger> m_packet_merger_ptr_;
  // Currently set in CreateSession command
  // boost::optional<uint8_t> m_session_time_out_sec{boost::none};
  communication::TCPClientPtr m_tcp_client_ptr_{};
  // SessionState m_state_{SessionState::CLOSED};

  bool addToPacketMerger(sick::datastructure::PacketBuffer &buf);
};

// TODO remove old stuff

// /*!
//  * \brief Forward declaration of command class.
//  */
// class CommandMsg;

// /*!
//  * \brief Forward declaration of create session class.
//  */
// class CreateSession;

// /*!
//  * \brief Establishes a cola2 session with a sensor and enables execution of commands in this
//  * session.
//  */
// class Cola2Session
// {
// public:
//   /*!
//    * \brief Typedef for a pointer containing a command to be executed.
//    */
//   typedef std::shared_ptr<sick::cola2::CommandMsg> CommandPtr;

//   /*!
//    * \brief Constructor of the cola2 session.
//    *
//    * \param async_tcp_client Pointer to an instance of a TCP-client. Will be used to establish a
//    * connection to the sensor.
//    */
//   // explicit Cola2Session(const std::shared_ptr<communication::AsyncTCPClient>& async_tcp_client);
//   Cola2Session();

//   /*!
//    * \brief Triggers the disconnection of the tcp socket.
//    */
//   // void doDisconnect();

//   /*!
//    * \brief Executes the command passed to the function.
//    *
//    * \param command The command to be executed.
//    *
//    * \returns If the execution was successful.
//    */
//   bool executeCommand(const CommandPtr &command);

//   /*!
//    * \brief Returns the current session ID.
//    *
//    * \returns The current session ID.
//    */
//   uint32_t getSessionID() const;

//   /*!
//    * \brief Sets the current session ID.
//    *
//    * \param session_id The new session ID.
//    */
//   void setSessionID(const uint32_t &session_id);

//   /*!
//    * \brief Returns the next request ID. The request ID is used to match the return packages of the
//    * sensor to the right command.
//    *
//    * \returns A new request ID.
//    */
//   uint16_t getNextRequestID();

//   /*!
//    * \brief Closes a session with the sensor. Executes the close session command.
//    *
//    * \returns If closing the session was successful.
//    */
//   bool close();

//   /*!
//    * \brief Opens a session with the sensor. Executes the create session command.
//    *
//    * \returns If opening a session was successful.
//    */
//   bool open();

// private:
//   // std::shared_ptr<sick::communication::AsyncTCPClient> m_async_tcp_client_ptr;
//   // std::unique_ptr<sick::data_processing::ParseTCPPacket> m_parser_ptr;

//   sick::data_processing::TCPPacketMerger m_packet_merger_ptr;
//   sick::data_processing::ParseTCPPacket m_tcp_parser_ptr;

//   std::map<uint16_t, CommandPtr> m_pending_commands_map;

//   std::mutex m_execution_mutex;

//   uint32_t m_session_id{0};
//   uint16_t m_last_request_id{0};

//   void processPacket(const sick::datastructure::PacketBuffer &packet);

//   // TODO what is the caller supposed to do on false return values?
//   bool addCommand(uint16_t request_id, const CommandPtr &command);
//   bool findCommand(uint16_t request_id, CommandPtr &command) const;
//   bool removeCommand(uint16_t request_id);

//   bool
//   startProcessingAndRemovePendingCommandAfterwards(const sick::datastructure::PacketBuffer &packet);
//   bool addPacketToMerger(const sick::datastructure::PacketBuffer &packet);
//   bool checkIfPacketIsCompleteAndOtherwiseListenForMorePackets();
//   bool sendTelegramAndListenForAnswer(const CommandPtr &command);
// };

} // namespace cola2
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H
