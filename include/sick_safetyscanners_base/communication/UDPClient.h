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

#include <iostream>

#include <boost/asio.hpp>

#include "sick_safetyscanners_base/Types.h"
#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"

namespace sick {
namespace communication {

/*!
 * \brief A UDP client allowing for synchronous and asynchronous receiving of sensor data.
 *
 */
class UDPClient
{
public:
  /*!
   * \brief Constructor of a UDPClient object
   *
   * \param io_service Instance of the boost::asio io_service
   * \param server_port The local port number on the receiver (this client's) side.
   */
  UDPClient(boost::asio::io_service& io_service, sick::types::port_t server_port);

  /*!
   * \brief Constructor of a UDPClient object
   *
   * \param io_service Instance of the boost::asio io_service
   * \param server_port The local port number on the receiver (this client's) side.
   * \param host_ip The multicast ip to enable the multicast registration.
   * \param interface_ip The used host (client's) interface IP  which is needed to join the
   * multicast group.
   */
  UDPClient(boost::asio::io_service& io_service,
            sick::types::port_t server_port,
            boost::asio::ip::address_v4 host_ip,
            boost::asio::ip::address_v4 interface_ip);

  UDPClient()                 = delete;
  UDPClient(const UDPClient&) = delete;
  UDPClient& operator=(const UDPClient&) = delete;

  /*!
   * \brief Destructor of the UDPClient object
   *
   */
  virtual ~UDPClient();

  /*!
   * \brief Asynchronous function to subscribe to the sensor data stream.
   *
   * 	param Callable
   * \param callback A callback of variable templated type Callable to support various formats like
   * std::function.
   */
  template <typename Callable>
  void run(Callable&& callback)
  {
    m_packet_handler = callback;
    beginReceive();
  }

  /*!
   * \brief Stops the currently running asynchronous sensor data stream subscribers.
   *
   */
  void stop();

  /*!
   * \brief Returns the actual port assigned to the local client
   *
   * \return sick::types::port_t.
   */
  sick::types::port_t getLocalPort() const;

  /*!
   * \brief Indicates whether the UDP ports are still opened.
   *
   * \return true
   * \return false
   */
  bool isConnected() const;

  /*!
   * \brief Indicates available data in the receiving buffer.
   *
   * \return true
   * \return false
   */
  bool isDataAvailable() const;

  /*!
   * \brief A synchronous function to receive data. This operation is blocking unless the timeout
   * has been exceeded.
   *
   * \param timeout Timeout on receiving operation.
   * \return sick::datastructure::PacketBuffer
   */
  sick::datastructure::PacketBuffer receive(sick::types::time_duration_t timeout);

private:
  boost::asio::io_service& m_io_service;
  boost::asio::ip::udp::endpoint m_remote_endpoint;
  boost::asio::ip::udp::socket m_socket;
  types::PacketHandler m_packet_handler;
  datastructure::PacketBuffer::ArrayBuffer m_recv_buffer;
  boost::asio::deadline_timer m_deadline;

  /*!
   * \brief A function to check internal deadline constraints on connect, receive and send
   * opterations of boost::asio.
   *
   */
  void checkDeadline();

  /*!
   * \brief Internal callback function to transform raw data into a packetBuffer and begin a new
   * receive-cycle.
   *
   * \param ec A boost::system error code.
   * \param bytes_recv The number of bytes received.
   */
  void handleReceive(boost::system::error_code ec, std::size_t bytes_recv);

  /*!
   * \brief Internal function to begin the next cycle to receive sensor data.
   *
   */
  void beginReceive();

  /*!
   * \brief Helper function to set error_codes if an internal deadline has been exceeded.
   *
   */
  static void handleReceiveDeadline(const boost::system::error_code& ec,
                                    std::size_t length,
                                    boost::system::error_code* out_ec,
                                    std::size_t* out_length)
  {
    *out_ec     = ec;
    *out_length = length;
  }
};

} // namespace communication
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COMMUNICATION_ASYNCUDPCLIENT_H
