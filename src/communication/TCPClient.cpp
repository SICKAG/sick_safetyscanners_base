#include "sick_safetyscanners_base/communication/TCPClient.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::lambda::bind;
using boost::lambda::var;
using boost::lambda::_1;


namespace sick
{
namespace communication
{
TCPClient::TCPClient(const boost::asio::ip::address_v4 &server_ip, uint16_t server_port, boost::asio::io_service &io_service)
    : m_recv_buffer_(),
      m_socket_(io_service),
      m_server_ip_(server_ip),
      m_mutex_(),
      m_server_port_(server_port),
      m_deadline(io_service)
{
    // Since synchronous timeout on socket.connect() are not available with boost::asio, this seems to be the most elegant way eventhough asynchronous.
    m_deadline.expires_at(boost::posix_time::pos_infin);
    checkDeadline();
}

void TCPClient::checkDeadline()
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
        m_socket_.close(ignored_ec);

        // There is no longer an active deadline. The expiry is set to positive
        // infinity so that the actor takes no action until a new deadline is set.
        m_deadline.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    m_deadline.async_wait(bind(&TCPClient::checkDeadline, this));
}

void TCPClient::connect()
{
    auto remote_endpoint = boost::asio::ip::tcp::endpoint(m_server_ip_, m_server_port_);
    boost::system::error_code ec = boost::asio::error::would_block;

    const boost::posix_time::time_duration timeout = boost::posix_time::milliseconds(2000);
    m_deadline.expires_from_now(timeout);

    m_socket_.async_connect(remote_endpoint, var(ec) = _1);

    do
        m_socket_.get_io_service().run_one();
    while (ec == boost::asio::error::would_block);

    if (ec || !m_socket_.is_open())
    {
        throw boost::system::system_error(ec ? ec : boost::asio::error::operation_aborted);
    }

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
        LOG_ERROR("An error occured during disconnecting from the server: %s. This error is internally ignored and socket has been closed", ec.message().c_str());
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
        LOG_ERROR("Error while sending a TCP message: %s", ec.message().c_str());
        throw boost::system::system_error(ec);
    }
}

sick::datastructure::PacketBuffer TCPClient::receive()
{
    std::lock_guard<std::mutex> guard(m_mutex_);
    boost::system::error_code ec;

    std::size_t bytes_received = m_socket_.read_some(boost::asio::buffer(m_recv_buffer_), ec);
    if (!ec)
    {
        sick::datastructure::PacketBuffer buffer(m_recv_buffer_, bytes_received);
        return buffer;
    }
    else
    {
        LOG_ERROR("Error while receiving TCP message: %s", ec.message().c_str());
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
        LOG_ERROR("Error while receiving TCP message: %s", ec.message().c_str());
        throw boost::system::system_error(ec);
    }
}

} // namespace communication
} // namespace sick
