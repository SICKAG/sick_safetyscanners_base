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
 * \file AsyncTCPClient.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_safetyscanners_base/logging/logging_wrapper.h>

#include <sick_safetyscanners_base/communication/AsyncTCPClient.h>

namespace sick {
namespace communication {
AsyncTCPClient::AsyncTCPClient(boost::asio::io_service& io_service,
                               const boost::asio::ip::address_v4& server_ip,
                               const uint16_t& server_port)
  : m_io_service(io_service)
{
  try
  {
    m_socket_ptr = std::make_shared<boost::asio::ip::tcp::socket>(boost::ref(m_io_service));
  }
  catch (std::exception& e)
  {
    ROS_ERROR("Exception while creating socket: %s", e.what());
  }

  m_remote_endpoint = boost::asio::ip::tcp::endpoint(server_ip, server_port);
  ROS_INFO("TCP client is setup");
}

AsyncTCPClient::~AsyncTCPClient() {}

void AsyncTCPClient::doConnect( CompleteHandler handler )
{
  m_socket_ptr->async_connect(m_remote_endpoint, handler );
}

boost::system::error_code AsyncTCPClient::doDisconnect()
{
  boost::mutex::scoped_lock lock(m_socket_mutex);
  boost::system::error_code ec;
  m_socket_ptr->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  if (ec)
  {
    ROS_ERROR("Error shutting socket down: %i", ec.value());
  }
  else
  {
    ROS_INFO("TCP Connection successfully shutdown");
  }

  m_socket_ptr->close(ec);
  if (ec)
  {
    ROS_ERROR("Error closing Socket: %i", ec.value());
  }
  else
  {
    ROS_INFO("TCP Socket successfully closed.");
  }

  return ec;
}

void AsyncTCPClient::send(const std::vector<uint8_t>& sendBuffer, CompleteHandler handler)
{
  if (!m_socket_ptr)
  {
    handler( boost::asio::error::not_connected );
    return;
  }

  boost::asio::async_write(*m_socket_ptr,
                           boost::asio::buffer(sendBuffer),
                           std::bind( handler, std::placeholders::_1 ) );
}

void AsyncTCPClient::readSome( PacketHandler handler )
{
  if (!m_socket_ptr)
  {
    handler( sick::datastructure::PacketBuffer(), boost::asio::error::not_connected );
    return;
  }

  m_socket_ptr->async_read_some(boost::asio::buffer(m_recv_buffer),
                                std::bind( &AsyncTCPClient::handleReceive,
                                           this,
                                           handler,
                                           std::placeholders::_1, std::placeholders::_2 ) );
}

void AsyncTCPClient::handleReceive(PacketHandler handler,
                                   const boost::system::error_code& error,
                                   const std::size_t& bytes_transferred)
{
  if (!error)
  {
    sick::datastructure::PacketBuffer packet_buffer(m_recv_buffer, bytes_transferred);
    handler( packet_buffer, error );
  }
  else
  {
    ROS_ERROR("Error in tcp handle receive: %i", error.value());

    handler( sick::datastructure::PacketBuffer(), error );
  }
}


} // namespace communication
} // namespace sick
