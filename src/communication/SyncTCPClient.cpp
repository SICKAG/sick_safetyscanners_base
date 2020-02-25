#include <sick_safetyscanners_base/communication/SyncTCPClient.h>

namespace sick
{
namespace communication
{
SyncTCPClient::SyncTCPClient(const boost::asio::ip::address_v4 &server_ip, uint16_t server_port, boost::asio::io_service &io_service) : m_socket_(io_service), m_server_ip_(server_ip), m_server_port_(server_port), m_recv_buffer_()
{
}

void SyncTCPClient::connect()
{
    m_socket_.close();
    auto remote_endpoint = boost::asio::ip::tcp::endpoint(m_server_ip_, m_server_port_);
    boost::system::error_code ec = boost::asio::error::host_not_found;
    m_socket_.connect(remote_endpoint, ec);
    if (ec)
    {
        LOG_ERROR("Could not connect to TCP server. Error code %i", ec.value());
        throw boost::system::system_error(ec);
    }
    else
    {
        LOG_INFO("TCP connection successfully established.");
    }
}

void SyncTCPClient::disconnect()
{
    boost::system::error_code ec;
    m_socket_.close(ec);
    if (ec == boost::asio::error::eof)
    {
        // Peer closed connection as expected
        LOG_INFO("TCP connection has been closed.");
        return;
    }
    else if (ec)
    {
        // Some other error occured
        LOG_ERROR("An error occured during disconnecting from the server: %s", ec.message());
        throw boost::system::system_error(ec);
    }
}

void SyncTCPClient::send(const std::vector<uint8_t> &buf)
{
    std::lock_guard<std::mutex> guard(m_mutex_);
    boost::system::error_code ec;
    boost::asio::write(m_socket_, boost::asio::buffer(buf), boost::asio::transfer_all(), ec);

    if (ec) {
        LOG_ERROR("Error while sending a TCP message: %s", ec.message());
        throw boost::system::system_error(ec);
    }
}

sick::datastructure::PacketBuffer SyncTCPClient::receive()
{
    std::lock_guard<std::mutex> guard(m_mutex_);
    boost::system::error_code ec;
    // std::size_t bytes_received = m_socket_.read_some(boost::asio::buffer(m_recv_buffer_), ec);

    std::size_t bytes_received = m_socket_.read_some(m_recv_buffer_, ec);
    if (!ec)
    {
        sick::datastructure::PacketBuffer buffer(m_recv_buffer_, bytes_received);
        return buffer;
    }
    else
    {
        LOG_ERROR("Error while receiving TCP message: %s", ec.message());
        throw boost::system::system_error(ec);
    }
}

sick::datastructure::PacketBuffer SyncTCPClient::receive(std::size_t n_bytes)
{
    std::lock_guard<std::mutex> guard(m_mutex_);
    boost::system::error_code ec;
    std::size_t bytes_received = boost::asio::read(m_socket_, boost::asio::buffer(m_recv_buffer_), boost::asio::transfer_exactly(n_bytes), ec);

    if (!ec)
    {
        sick::datastructure::PacketBuffer buffer(m_recv_buffer_, bytes_received);
        return buffer;
    }
    else
    {
        LOG_ERROR("Error while receiving TCP message: %s", ec.message());
        throw boost::system::system_error(ec);
    }
}

} // namespace communication
} // namespace sick
