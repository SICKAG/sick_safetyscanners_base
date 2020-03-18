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
 * \file Cola2Session.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/cola2/Cola2Session.h"
#include "sick_safetyscanners_base/types.h"

namespace sick
{
namespace cola2
{

Cola2Session::Cola2Session(communication::TCPClientPtr tcp_client) : m_request_id_(0),
                                                                     m_session_id_(boost::none),
                                                                     m_tcp_client_ptr_(std::move(tcp_client))
{
}

boost::optional<uint32_t> Cola2Session::getSessionID() const
{
    return m_session_id_;
}

uint16_t Cola2Session::getNextRequestID()
{
    return ++m_request_id_;
}

void Cola2Session::setSessionID(uint32_t session_id)
{
    if (m_session_id_.is_initialized())
    {
        m_session_id_.reset(session_id);
    }
    else
    {
        m_session_id_.emplace(session_id);
    }
}

void Cola2Session::open()
{
    m_tcp_client_ptr_->connect();
    assert(m_tcp_client_ptr_->isConnected());
    CreateSession cmd(*this);
    executeCommand(cmd);
    auto sessID = cmd.getSessionID();
    setSessionID(sessID);
    LOG_INFO("Successfully opened Cola2 session with sessionID: %u", sessID);
}

void Cola2Session::close()
{
    if (!m_tcp_client_ptr_->isConnected())
    {
        return;
    }

    CloseSession cmd(*this);
    executeCommand(cmd);
    m_tcp_client_ptr_->disconnect();
    auto sessID = cmd.getSessionID();
    LOG_INFO("Closed Cola2 session with sessionID: %u", sessID);
}

void Cola2Session::sendRequest(CommandMsg &cmd)
{
    cmd.setSessionID(getSessionID().get_value_or(0));
    std::vector<uint8_t> telegram;
    telegram = cmd.constructTelegram(telegram);
    LOG_INFO("send telegram with sessID %u and reqID %u", cmd.getSessionID(), cmd.getRequestID());
    m_tcp_client_ptr_->send(telegram);
}

sick::datastructure::PacketBuffer Cola2Session::waitAndProcessResponse(CommandMsg &cmd, int64_t timeout_ms)
{
    sick::data_processing::TCPPacketMerger packet_merger(0);
    sick::data_processing::ParseTCPPacket tcp_packet_parser;
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    while (!packet_merger.isComplete())
    {
        int64_t elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
        if (elapsed_time > timeout_ms)
        {
            throw std::runtime_error("Timeout during collecting command response packages.");
        }
        sick::datastructure::PacketBuffer packet_buffer = m_tcp_client_ptr_->receive();
        if (packet_merger.isEmpty())
        {
            auto expectedPacketLength = tcp_packet_parser.getExpectedPacketLength(packet_buffer);
            packet_merger.setTargetSize(expectedPacketLength);
        }
        packet_merger.addTCPPacket(packet_buffer);
    }
    sick::datastructure::PacketBuffer response = packet_merger.getDeployedPacketBuffer();
    return response;
}

void Cola2Session::executeCommand(CommandMsg &cmd, long int timeout_ms)
{
    if (!m_tcp_client_ptr_->isConnected())
    {
        open();
    }

    sendRequest(cmd);
    auto response = waitAndProcessResponse(cmd, timeout_ms);
    cmd.processReplyBase(*response.getBuffer());

    if (!cmd.wasSuccessful())
    {
        throw std::runtime_error("Response to command telegram indicated failure.");
    }

    LOG_INFO("end of executeCommand");
}

} // namespace cola2
} // namespace sick
