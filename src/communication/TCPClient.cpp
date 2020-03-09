#include <sick_safetyscanners_base/communication/TCPClient.h>

namespace sick
{
namespace communication
{
TCPClient::TCPClient(const boost::asio::ip::address_v4 &server_ip, uint16_t server_port, boost::asio::io_service &io_service) : m_socket_(io_service), m_server_ip_(server_ip), m_server_port_(server_port), m_recv_buffer_()
{
}

TCPClient::TCPClient(const boost::asio::ip::address_v4 &server_ip, uint16_t server_port, boost::asio::ip::tcp::socket &&socket)
    : m_server_ip_(server_ip),
      m_server_port_(server_port),
      m_socket_(std::move(socket))

{
}

void TCPClient::connect()
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

void TCPClient::disconnect()
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
        LOG_ERROR("An error occured during disconnecting from the server: %s. This error is internally ignored and socket has been closed", ec.message());
    }
}

bool TCPClient::isConnected()
{
    return m_socket_.is_open();
}

void TCPClient::send(const std::vector<uint8_t> &buf)
{
    std::lock_guard<std::mutex> guard(m_mutex_);
    boost::system::error_code ec;
    boost::asio::write(m_socket_, boost::asio::buffer(buf), boost::asio::transfer_all(), ec);

    if (ec)
    {
        LOG_ERROR("Error while sending a TCP message: %s", ec.message());
        throw boost::system::system_error(ec);
    }
}

sick::datastructure::PacketBuffer TCPClient::receive()
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

sick::datastructure::PacketBuffer TCPClient::receive(std::size_t n_bytes)
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
