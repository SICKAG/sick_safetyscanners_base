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
 * \file AsyncUDPClient.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/communication/UDPClient.h"

namespace sick
{
namespace communication
{

UDPClient::UDPClient(
    boost::asio::io_service &io_service,
    unsigned short server_port)
    : m_io_service(io_service),
      m_socket(boost::ref(io_service), boost::asio::ip::udp::endpoint{boost::asio::ip::udp::v4(), server_port}),
      m_packet_handler(),
      m_recv_buffer()
{
  LOG_INFO("UDP client is setup");
}

UDPClient::~UDPClient()
{
}

void UDPClient::handleReceive(boost::system::error_code ec, std::size_t bytes_recv)
{
  if (!ec)
  {
    sick::datastructure::PacketBuffer packet_buffer(m_recv_buffer, bytes_recv);
    std::cout << "packet_buffer ^^" << std::endl;
    m_packet_handler(packet_buffer);
  }
  else
  {
    LOG_ERROR("Error in UDP handle receive: %i", ec.value());
  }
  beginReceive();
}

void UDPClient::beginReceive()
{
  m_socket.async_receive_from(boost::asio::buffer(m_recv_buffer), m_remote_endpoint, [this](boost::system::error_code ec, std::size_t bytes_recvd) {
    this->handleReceive(ec, bytes_recvd);
  });
}

void UDPClient::stop()
{
  m_socket.cancel();
}

void UDPClient::connect()
{
  // m_socket.close();

  boost::system::error_code ec = boost::asio::error::host_not_found;
  // m_socket.connect(m_remote_endpoint, ec);
  if (ec)
  {
    LOG_ERROR("Could not connect to Sensor (UDP). Error code %i", ec.value());
    throw boost::system::system_error(ec);
  }
  else
  {
    LOG_INFO("UDP connection successfully established.");
  }
}

void UDPClient::disconnect()
{
}

unsigned short UDPClient::getLocalPort()
{
  if (m_socket.is_open())
  {
    return m_socket.local_endpoint().port();
  }
  return 0;
}

bool UDPClient::isConnected()
{
  return m_socket.is_open();
}

} // namespace communication
} // namespace sick
