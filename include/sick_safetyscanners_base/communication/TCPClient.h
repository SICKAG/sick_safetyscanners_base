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
 * \file TCPClient.h
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_COMMUNICATION_SYNCTCPCLIENT_H
#define SICK_SAFETYSCANNERS_BASE_COMMUNICATION_SYNCTCPCLIENT_H

#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "sick_safetyscanners_base/types.h"
#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"
#include "sick_safetyscanners_base/logging.h"
#include "sick_safetyscanners_base/exceptions.h"

namespace sick
{
namespace communication
{
class TCPClient
{
public:
    TCPClient(
        sick::types::ip_address_t server_ip,
        sick::types::port_t server_port);
        

    TCPClient() = delete;
    TCPClient(const TCPClient &) = delete;
    TCPClient &operator=(const TCPClient &) = delete;

    void connect(sick::types::time_duration_t timeout = boost::posix_time::seconds(5));
    void disconnect();
    void send(const std::vector<uint8_t> &buf);
    bool isConnected();
    sick::datastructure::PacketBuffer receive(sick::types::time_duration_t timeout = boost::posix_time::seconds(5));

private:
    boost::asio::io_service m_io_service;
    sick::datastructure::PacketBuffer::ArrayBuffer m_recv_buffer;
    boost::asio::ip::tcp::socket m_socket;
    sick::types::ip_address_t m_server_ip;
    sick::types::port_t m_server_port;
    boost::asio::deadline_timer m_deadline;

    void checkDeadline();

    static void handleReceiveDeadline(
        const boost::system::error_code &ec, std::size_t length,
        boost::system::error_code *out_ec, std::size_t *out_length)
    {
        *out_ec = ec;
        *out_length = length;
    }
};

using TCPClientPtr = std::unique_ptr<sick::communication::TCPClient>;
} // namespace communication
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COMMUNICATION_SYNCTCPCLIENT_H
