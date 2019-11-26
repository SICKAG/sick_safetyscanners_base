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
 * \file CommSettings.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <cmath>

#include <sick_safetyscanners_base/datastructure/CommSettings.h>

namespace sick {
namespace datastructure {

CommSettings::CommSettings()
{
  // Defaults, could be moved to an external config xml
  m_sensor_ip            = boost::asio::ip::address_v4::from_string("192.168.1.11");
  m_host_ip              = boost::asio::ip::address_v4::from_string("192.168.1.9");
  m_host_udp_port        = 0;
  m_publishing_frequency = 1;
  m_start_angle          = 0.0;
  m_end_angle            = 0.0;
  m_channel              = 0;
  m_enabled              = true;
  m_e_interface_type     = 0;

  m_features = 0;
  m_features += 1 << 0; // enable general system state
  m_features += 1 << 1; // enable derived settings
  m_features += 1 << 2; // enable measurement data
  m_features += 1 << 3; // enable intursion data
  m_features += 1 << 4; // enable application data
}

boost::asio::ip::address_v4 CommSettings::getHostIp() const
{
  return m_host_ip;
}

void CommSettings::setHostIp(const boost::asio::ip::address_v4& host_ip)
{
  m_host_ip = host_ip;
}

void CommSettings::setHostIp(const std::string& host_ip)
{
  m_host_ip = boost::asio::ip::address_v4::from_string(host_ip);
}

uint16_t CommSettings::getHostUdpPort() const
{
  return m_host_udp_port;
}

void CommSettings::setHostUdpPort(const uint16_t& host_udp_port)
{
  m_host_udp_port = host_udp_port;
}

uint8_t CommSettings::getChannel() const
{
  return m_channel;
}

void CommSettings::setChannel(const uint8_t& channel)
{
  m_channel = channel;
}

bool CommSettings::getEnabled() const
{
  return m_enabled;
}

void CommSettings::setEnabled(bool enabled)
{
  m_enabled = enabled;
}

uint8_t CommSettings::getEInterfaceType() const
{
  return m_e_interface_type;
}

void CommSettings::setEInterfaceType(const uint8_t& e_interface_type)
{
  m_e_interface_type = e_interface_type;
}

uint16_t CommSettings::getPublishingFrequency() const
{
  return m_publishing_frequency;
}

void CommSettings::setPublishingFrequency(const uint16_t& publishing_frequency)
{
  m_publishing_frequency = publishing_frequency;
}

int32_t CommSettings::getStartAngle() const
{
  return m_start_angle;
}

void CommSettings::setStartAngle(const float& start_angle)
{
  m_start_angle = std::floor( start_angle * 4194304 ); // TODO refactor in constant
}

int32_t CommSettings::getEndAngle() const
{
  return m_end_angle;
}

void CommSettings::setEndAngle(const float& end_angle)
{
  m_end_angle = std::ceil( end_angle * 4194304 );
}

uint16_t CommSettings::getFeatures() const
{
  return m_features;
}

void CommSettings::setFeatures(const uint16_t& features)
{
  m_features = features;
}

void CommSettings::setFeatures(bool general_system_state,
                               bool derived_settings,
                               bool measurement_data,
                               bool intrusion_data,
                               bool application_data)
{
  m_features = 0;
  m_features += (static_cast<int>(general_system_state) << 0);
  m_features += (static_cast<int>(derived_settings) << 1);
  m_features += (static_cast<int>(measurement_data) << 2);
  m_features += (static_cast<int>(intrusion_data) << 3);
  m_features += (static_cast<int>(application_data) << 4);
}

boost::asio::ip::address_v4 CommSettings::getSensorIp() const
{
  return m_sensor_ip;
}

void CommSettings::setSensorIp(const boost::asio::ip::address_v4& sensor_ip)
{
  m_sensor_ip = sensor_ip;
}

void CommSettings::setSensorIp(const std::string& sensor_ip)
{
  m_sensor_ip = boost::asio::ip::address_v4::from_string(sensor_ip);
}

uint16_t CommSettings::getSensorTcpPort() const
{
  return m_sensor_tcp_port;
}

void CommSettings::setSensorTcpPort(const uint16_t& sensor_tcp_port)
{
  m_sensor_tcp_port = sensor_tcp_port;
}


} // namespace datastructure
} // namespace sick
