#ifndef SICK_SAFETYSCANNERS_BASE_COMMUNICATION_SYNCTCPCLIENT_H
#define SICK_SAFETYSCANNERS_BASE_COMMUNICATION_SYNCTCPCLIENT_H

#include <vector>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>
#include <sick_safetyscanners_base/log.h>
#include <sick_safetyscanners_base/exceptions.h>

namespace sick
{
namespace communication
{
class SyncTCPClient
{
public:
    SyncTCPClient(
        const boost::asio::ip::address_v4 &server_ip,
        uint16_t server_port,
        boost::asio::io_service &io_service);

    SyncTCPClient() = delete;
    SyncTCPClient(const SyncTCPClient &) = delete;
    SyncTCPClient &operator=(const SyncTCPClient &) = delete;

    void connect();
    void disconnect();
    void send(const std::vector<uint8_t> &buf);
    sick::datastructure::PacketBuffer receive();
    sick::datastructure::PacketBuffer receive(std::size_t n_bytes);

private:
    // boost::asio::io_service m_io_service;
    sick::datastructure::PacketBuffer::ArrayBuffer m_recv_buffer_;
    boost::asio::ip::tcp::socket m_socket_;
    boost::asio::ip::address_v4 m_server_ip_;
    std::mutex m_mutex_;
    uint16_t m_server_port_;
};
} // namespace communication
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COMMUNICATION_SYNCTCPCLIENT_H
