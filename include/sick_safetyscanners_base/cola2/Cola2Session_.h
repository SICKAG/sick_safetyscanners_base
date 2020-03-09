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
#include <boost/thread.hpp>
#include <memory>
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

using TCPClientPtr = std::unique_ptr<communication::TCPClient>;

class Cola2Session
{
public:
    //TODO pass general TCPClient?
    explicit Cola2Session(TCPClientPtr tcp_client);
    Cola2Session() = delete;
    Cola2Session(const Cola2Session &) = delete;
    Cola2Session &operator=(const Cola2Session &) = delete;

    // Synchronous interface
    void executeCommand(CommandMsg &cmd, ulong timeout_ms = 2000);
    void tick();

    // Asynchronous interface
    void executeCommandAsync(CommandMsg &cmd);
    // void sendCommandAsync(Command &cmd);

    boost::optional<uint32_t> getSessionID() const;

private:
    void openSession();
    void closeSession();
    uint16_t getNextRequestID();
    void setSessionID(uint32_t session_id);

    uint16_t m_request_id_{0};
    boost::mutex m_execution_mutex_{};
    boost::optional<uint32_t> m_session_id_{boost::none};
    // Currently set in CreateSession command
    // boost::optional<uint8_t> m_session_time_out_sec{boost::none};
    TCPClientPtr m_tcp_client_ptr_{};
    // SessionState m_state_{SessionState::CLOSED};
};
} // namespace cola2
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COLA2_COLA2SESSION_H