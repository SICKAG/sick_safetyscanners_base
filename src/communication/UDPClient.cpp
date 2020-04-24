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
 * \file AsyncUDPClient.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/communication/UDPClient.h"
#include "sick_safetyscanners_base/Exceptions.h"
#include "sick_safetyscanners_base/Logging.h"

#include <boost/array.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace sick {
namespace communication {

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::lambda::_1;
using boost::lambda::bind;
using boost::lambda::var;

UDPClient::UDPClient(boost::asio::io_service& io_service, sick::types::port_t server_port)
  : m_io_service(io_service)
  , m_socket(boost::ref(io_service),
             boost::asio::ip::udp::endpoint{boost::asio::ip::udp::v4(), server_port})
  , m_packet_handler()
  , m_recv_buffer()
  , m_deadline(io_service)
{
  m_deadline.expires_at(boost::posix_time::pos_infin);
  checkDeadline();
}

UDPClient::~UDPClient() {}

void UDPClient::checkDeadline()
{
  // Check whether the deadline has passed. We compare the deadline against
  // the current time since a new asynchronous operation may have moved the
  // deadline before this actor had a chance to run.
  if (m_deadline.expires_at() <= deadline_timer::traits_type::now())
  {
    // The deadline has passed. The socket is closed so that any outstanding
    // asynchronous operations are cancelled. This allows the blocked
    // connect(), read_line() or write_line() functions to return.
    boost::system::error_code ignored_ec;
    m_socket.close(ignored_ec);

    // There is no longer an active deadline. The expiry is set to positive
    // infinity so that the actor takes no action until a new deadline is set.
    m_deadline.cancel();
    m_deadline.expires_at(boost::posix_time::pos_infin);
  }

  // Put the actor back to sleep.
  m_deadline.async_wait(bind(&UDPClient::checkDeadline, this));
}

void UDPClient::handleReceive(boost::system::error_code ec, std::size_t bytes_recv)
{
  if (!ec)
  {
    sick::datastructure::PacketBuffer packet_buffer(m_recv_buffer, bytes_recv);
    m_packet_handler(packet_buffer);
  }
  else
  {
    throw runtime_error(ec.message());
  }
  beginReceive();
}

void UDPClient::beginReceive()
{
  m_socket.async_receive_from(boost::asio::buffer(m_recv_buffer),
                              m_remote_endpoint,
                              [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                                this->handleReceive(ec, bytes_recvd);
                              });
}

void UDPClient::stop()
{
  m_socket.cancel();
}

sick::datastructure::PacketBuffer UDPClient::receive(sick::types::time_duration_t timeout)
{
  boost::system::error_code ec = boost::asio::error::would_block;
  m_deadline.expires_from_now(timeout);

  std::size_t bytes_recv = 0;
  m_socket.async_receive_from(
    boost::asio::buffer(m_recv_buffer),
    m_remote_endpoint,
    boost::bind(&UDPClient::handleReceiveDeadline, _1, _2, &ec, &bytes_recv));

  // Block until async_receive_from finishes or the deadline_timer exceeds its timeout.
  do
    m_socket.get_io_service().run_one();
  while (ec == boost::asio::error::would_block);

  if (ec == boost::asio::error::timed_out || ec == boost::asio::error::operation_aborted)
  {
    throw timeout_error("Timeout exceeded while waiting for sensor data", timeout);
  }
  if (ec)
  {
    throw runtime_error(ec.message());
  }

  auto buffer = sick::datastructure::PacketBuffer(m_recv_buffer, bytes_recv);
  return buffer;
}

bool UDPClient::isDataAvailable() const
{
  return m_socket.is_open() && m_socket.available() > 0;
}

sick::types::port_t UDPClient::getLocalPort() const
{
  if (m_socket.is_open())
  {
    return m_socket.local_endpoint().port();
  }
  return 0;
}

bool UDPClient::isConnected() const
{
  return m_socket.is_open();
}

} // namespace communication
} // namespace sick
