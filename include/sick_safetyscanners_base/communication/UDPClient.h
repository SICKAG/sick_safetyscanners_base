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
 * \file UDPClient.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_COMMUNICATION_ASYNCUDPCLIENT_H
#define SICK_SAFETYSCANNERS_BASE_COMMUNICATION_ASYNCUDPCLIENT_H

#include <functional>
#include <iostream>
#include <memory>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"
#include "sick_safetyscanners_base/log.h"
#include "sick_safetyscanners_base/types.h"

namespace sick
{
namespace communication
{

/*!
 * \brief An asynchronous udp client.
 */
class UDPClient
{
public:
  // TODO typedefs fur globale typen auslagern
  /*!
   * \brief Typedef to a reference to a function. Will be used to process the incoming packets.
   */
  // typedef boost::function<void(const sick::datastructure::PacketBuffer &)> PacketHandler;

  /*!
   * \brief Constructor of the asynchronous udp client.
   *
   * \param packet_handler Function to handle incoming packets.
   * \param io_service Instance of the boost io_service.
   * \param local_port The local port, where the udp packets will arrive.
   */

  UDPClient(
      boost::asio::io_service &io_service,
      sick::types::port_t server_port);

  UDPClient() = delete;
  UDPClient(const UDPClient &) = delete;
  UDPClient &operator=(const UDPClient &) = delete;

  /*!
   * \brief The destructor of the asynchronous udp client.
   */
  virtual ~UDPClient();

  // TODO Wat furn Service, bessere Namen
  /*!
   * \brief Start the listening loop for the udp data packets.
   */
  template <typename Callable>
  void run(Callable &&callback)
  {
    m_packet_handler = callback;
    beginReceive();
  }

  void stop();

  /*!
   * \brief Returns the actual port assigned to the local machine
   * \return Local port number
   */
  unsigned short getLocalPort() const;

  bool isConnected() const;
  bool isDataAvailable() const;

  sick::datastructure::PacketBuffer receive(sick::types::time_duration_t timeout);

private:
  boost::asio::io_service &m_io_service;
  boost::asio::ip::udp::endpoint m_remote_endpoint;
  boost::asio::ip::udp::socket m_socket;
  types::PacketHandler m_packet_handler;
  datastructure::PacketBuffer::ArrayBuffer m_recv_buffer;
  boost::asio::deadline_timer m_deadline;

  void checkDeadline();
  void handleReceive(boost::system::error_code ec, std::size_t bytes_recv);
  void beginReceive();

  static void handleReceiveDeadline(
      const boost::system::error_code &ec, std::size_t length,
      boost::system::error_code *out_ec, std::size_t *out_length)
  {
    *out_ec = ec;
    *out_length = length;
  }
};

} // namespace communication
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COMMUNICATION_ASYNCUDPCLIENT_H
