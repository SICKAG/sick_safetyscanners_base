#include "sick_safetyscanners_base/communication/TCPClient.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

namespace sick
{
namespace communication
{

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::lambda::_1;
using boost::lambda::_2;
using boost::lambda::bind;
using boost::lambda::var;

TCPClient::TCPClient(sick::types::ip_address_t server_ip, sick::types::port_t server_port)
    : m_io_service(),
      m_recv_buffer(),
      m_socket(m_io_service),
      m_server_ip(server_ip),
      m_server_port(server_port),
      m_deadline(m_io_service)
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
        m_socket.close(ignored_ec);

        // There is no longer an active deadline. The expiry is set to positive
        // infinity so that the actor takes no action until a new deadline is set.
        m_deadline.expires_at(boost::posix_time::pos_infin);
    }

    // Put the actor back to sleep.
    m_deadline.async_wait(bind(&TCPClient::checkDeadline, this));
}

void TCPClient::connect(sick::types::time_duration_t timeout)
{
    auto remote_endpoint = boost::asio::ip::tcp::endpoint(m_server_ip, m_server_port);
    boost::system::error_code ec = boost::asio::error::would_block;

    m_deadline.expires_from_now(timeout);

    m_socket.async_connect(remote_endpoint, var(ec) = _1);

    do
        m_socket.get_io_service().run_one();
    while (ec == boost::asio::error::would_block);

    if (ec || !m_socket.is_open())
    {
        LOG_ERROR("Could not connect to TCP server. Error code %i", ec.value());
        throw boost::system::system_error(ec ? ec : boost::asio::error::operation_aborted);
    }
    else
    {
        LOG_DEBUG("TCP connection successfully established.");
    }
}

void TCPClient::disconnect()
{
    boost::system::error_code ec;
    m_socket.close(ec);
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
    return m_socket.is_open();
}

void TCPClient::send(const std::vector<uint8_t> &buf)
{
    // std::lock_guard<std::mutex> guard(m_mutex_);
    boost::system::error_code ec;
    boost::asio::write(m_socket, boost::asio::buffer(buf), boost::asio::transfer_all(), ec);

    if (ec)
    {
        LOG_ERROR("Error while sending a TCP message: %s", ec.message().c_str());
        throw boost::system::system_error(ec);
    }
}

sick::datastructure::PacketBuffer TCPClient::receive(sick::types::time_duration_t timeout)
{
    boost::system::error_code ec = boost::asio::error::would_block;

    m_deadline.expires_from_now(timeout);

    std::size_t bytes_recv = 0;

    m_socket.async_receive(boost::asio::buffer(m_recv_buffer),
                           bind(&TCPClient::handleReceiveDeadline, _1, _2, &ec, &bytes_recv));

    // Block until async_receive_from finishes or the deadline_timer exceeds its timeout.
    do
        m_socket.get_io_service().run_one();
    while (ec == boost::asio::error::would_block);

    if (ec || !m_socket.is_open())
    {
        LOG_ERROR("Error while receiving TCP message: %s", ec.message().c_str());
        throw boost::system::system_error(ec ? ec : boost::asio::error::operation_aborted);
    }
    sick::datastructure::PacketBuffer buffer(m_recv_buffer, bytes_recv);
    return buffer;
}

} // namespace communication
} // namespace sick
