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

#include <sick_safetyscanners_base/logging/logging_wrapper.h>

#include <sick_safetyscanners_base/communication/AsyncUDPClient.h>

namespace sick {
namespace communication {
AsyncUDPClient::AsyncUDPClient(const PacketHandler& packet_handler,
                               boost::asio::io_service& io_service,
                               const uint16_t& local_port)
  : m_packet_handler(packet_handler)
  , m_io_service(io_service)
{
  try
  {
    m_socket_ptr = std::make_shared<boost::asio::ip::udp::socket>(
      boost::ref(m_io_service),
      boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), local_port));
  }
  catch (std::exception& e)
  {
    ROS_ERROR("Exception while creating socket: %s", e.what());
  }

  ROS_INFO("UDP client is setup");
}

AsyncUDPClient::~AsyncUDPClient()
{
}

void AsyncUDPClient::start()
{
  startReceive();
}

void AsyncUDPClient::stop()
{
  boost::system::error_code ec;
  m_socket_ptr->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  m_socket_ptr->close(ec);
}

void AsyncUDPClient::startReceive()
{
  m_socket_ptr->async_receive_from(boost::asio::buffer(m_recv_buffer),
                                   m_remote_endpoint,
                                   std::bind( &AsyncUDPClient::handleReceive, this, std::placeholders::_1, std::placeholders::_2 ) );
}

void AsyncUDPClient::handleReceive(const boost::system::error_code& error,
                                   const std::size_t& bytes_transferred)
{
  if ( !error )
  {
    sick::datastructure::PacketBuffer packet_buffer(m_recv_buffer, bytes_transferred);
    m_packet_handler(packet_buffer);

    startReceive();
  }
  else
  {
    ROS_ERROR("Error in UDP handle receive: %i", error.value());
  }
}

unsigned short AsyncUDPClient::getLocalPort()
{
  return m_socket_ptr->local_endpoint().port();
}

} // namespace communication
} // namespace sick
