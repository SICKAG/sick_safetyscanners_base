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
 * \file SickSafetyscannersBase.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <utility>
#include <chrono>
#include "sick_safetyscanners_base/SickSafetyscannersBase.h"

namespace sick
{

SickSafetyscannersBase::SickSafetyscannersBase(
    ip_address_t sensor_ip,
    unsigned short sensor_tcp_port,
    CommSettings comm_settings)
    : m_sensor_ip(sensor_ip),
      m_sensor_tcp_port(sensor_tcp_port),
      m_comm_settings(comm_settings),
      m_io_service_ptr(sick::make_unique<boost::asio::io_service>()),
      m_io_service(*m_io_service_ptr),
      m_udp_client(m_io_service, comm_settings.host_udp_port),
      m_session(
          std::move(sick::make_unique<sick::communication::TCPClient>(
              m_sensor_ip, sensor_tcp_port, m_io_service))),
      m_packet_merger()
{
  init();
}

SickSafetyscannersBase::SickSafetyscannersBase(
    ip_address_t sensor_ip,
    unsigned short sensor_tcp_port,
    CommSettings comm_settings,
    boost::asio::io_service &io_service)
    : m_sensor_ip(sensor_ip),
      m_sensor_tcp_port(sensor_tcp_port),
      m_comm_settings(comm_settings),
      m_io_service_ptr(nullptr),
      m_io_service(io_service),
      m_udp_client(m_io_service, comm_settings.host_udp_port),
      m_session(
          std::move(sick::make_unique<sick::communication::TCPClient>(
              m_sensor_ip, sensor_tcp_port, m_io_service))),
      m_packet_merger()
{
  init();
}

void SickSafetyscannersBase::init()
{
  LOG_INFO("Started SickSafetyscannersBase");
  changeSensorSettings(m_comm_settings);
}

void SickSafetyscannersBase::changeSensorSettings(const CommSettings &settings)
{
  // if (settings.host_udp_port != m_udp_client->getLocalPort())
  // {
  //   // TODO re-open port on demanded UDP Port
  //   m_udp_client->disconnect();
  //   m_udp_client.reset(new sick::communication::UDPClient(m_io_service, settings.host_udp_port));
  // }
  CommSettings _settings = settings;
  _settings.host_udp_port = m_udp_client.getLocalPort();
  createAndExecuteCommand<sick::cola2::ChangeCommSettingsCommand>(m_session, _settings);
}

void SickSafetyscannersBase::findSensor(uint16_t blink_time)
{
  createAndExecuteCommand<sick::cola2::FindMeCommand>(m_session, blink_time);
}

void SickSafetyscannersBase::requestTypeCode(sick::datastructure::TypeCode &type_code)
{
  createAndExecuteCommand<sick::cola2::TypeCodeVariableCommand>(m_session, type_code);
  LOG_INFO("Type Code: %s", type_code.getTypeCode().c_str());
}

void SickSafetyscannersBase::requestApplicationName(
    sick::datastructure::ApplicationName &application_name)
{
  createAndExecuteCommand<sick::cola2::ApplicationNameVariableCommand>(m_session, application_name);
  LOG_INFO("Application name: %s", application_name.getApplicationName().c_str());
}
void SickSafetyscannersBase::requestFieldData(
    std::vector<sick::datastructure::FieldData> &fields)
{
  sick::datastructure::ConfigData config_data;
  createAndExecuteCommand<sick::cola2::MeasurementCurrentConfigVariableCommand>(m_session, config_data);

  for (int i = 0; i < 128; i++)
  {
    sick::datastructure::FieldData field_data;
    createAndExecuteCommand<sick::cola2::FieldHeaderVariableCommand>(m_session, field_data, i);

    if (field_data.getIsValid())
    {
      createAndExecuteCommand<sick::cola2::FieldGeometryVariableCommand>(m_session, field_data, i);
      field_data.setStartAngleDegrees(config_data.getDerivedStartAngle());
      field_data.setAngularBeamResolutionDegrees(config_data.getDerivedAngularBeamResolution());
      fields.push_back(field_data);
    }
    else if (i > 0) // index 0 is reserved for contour data
    {
      break; // skip other requests after first invalid
    }
  }
}

void SickSafetyscannersBase::requestMonitoringCases(
    std::vector<sick::datastructure::MonitoringCaseData> &monitoring_cases)
{
  for (int i = 0; i < 254; i++)
  {
    sick::datastructure::MonitoringCaseData monitoring_case_data;
    createAndExecuteCommand<sick::cola2::MonitoringCaseVariableCommand>(m_session, monitoring_case_data, i);
    if (monitoring_case_data.getIsValid())
    {
      monitoring_cases.push_back(monitoring_case_data);
    }
    else
    {
      break; // skip other requests after first invalid
    }
  }
}

void SickSafetyscannersBase::requestDeviceName(

    datastructure::DeviceName &device_name)
{
  createAndExecuteCommand<sick::cola2::DeviceNameVariableCommand>(m_session, device_name);
  LOG_INFO("Device name: %s", device_name.getDeviceName().c_str());
}

void SickSafetyscannersBase::requestSerialNumber(
    datastructure::SerialNumber &serial_number)
{
  createAndExecuteCommand<sick::cola2::SerialNumberVariableCommand>(m_session, serial_number);
  LOG_INFO("Serial Number: %s", serial_number.getSerialNumber().c_str());
}

void SickSafetyscannersBase::requestOrderNumber(
    datastructure::OrderNumber &order_number)
{
  createAndExecuteCommand<sick::cola2::OrderNumberVariableCommand>(m_session, order_number);
  LOG_INFO("Order Number: %s", order_number.getOrderNumber().c_str());
}

void SickSafetyscannersBase::requestProjectName(
    datastructure::ProjectName &project_name)
{
  createAndExecuteCommand<sick::cola2::ProjectNameVariableCommand>(m_session, project_name);
  LOG_INFO("Project Name: %s", project_name.getProjectName().c_str());
}

void SickSafetyscannersBase::requestUserName(
    datastructure::UserName &user_name)
{
  createAndExecuteCommand<sick::cola2::UserNameVariableCommand>(m_session, user_name);
  LOG_INFO("User Name: %s", user_name.getUserName().c_str());
}
void SickSafetyscannersBase::requestFirmwareVersion(
    datastructure::FirmwareVersion &firmware_version)
{
  createAndExecuteCommand<sick::cola2::FirmwareVersionVariableCommand>(m_session, firmware_version);
  LOG_INFO("Firmware Version: %s", firmware_version.getFirmwareVersion().c_str());
}

void SickSafetyscannersBase::requestPersistentConfig(
    sick::datastructure::ConfigData &config_data)
{
  createAndExecuteCommand<cola2::MeasurementPersistentConfigVariableCommand>(m_session, config_data);
}

void SickSafetyscannersBase::requestConfigMetadata(
    sick::datastructure::ConfigMetadata &config_metadata)
{
  createAndExecuteCommand<sick::cola2::ConfigMetadataVariableCommand>(m_session, config_metadata);
}

void SickSafetyscannersBase::requestStatusOverview(
    sick::datastructure::StatusOverview &status_overview)
{
  createAndExecuteCommand<sick::cola2::StatusOverviewVariableCommand>(m_session, status_overview);
}

void SickSafetyscannersBase::requestDeviceStatus(
    sick::datastructure::DeviceStatus &device_status)
{
  createAndExecuteCommand<sick::cola2::DeviceStatusVariableCommand>(m_session, device_status);
}

void SickSafetyscannersBase::requestLatestTelegram(
    sick::datastructure::Data &data,
    int8_t index)
{
  if (index < 0 || index > 3)
  {
    LOG_WARN("Index is out of bounds, returning default channel 0");
    index = 0;
  }
  createAndExecuteCommand<sick::cola2::LatestTelegramVariableCommand>(m_session, data, index);
}

void SickSafetyscannersBase::requestRequiredUserAction(
    sick::datastructure::RequiredUserAction &required_user_action)
{
  createAndExecuteCommand<sick::cola2::RequiredUserActionVariableCommand>(m_session, required_user_action);
}

AsyncSickSafetyScanner::AsyncSickSafetyScanner(ip_address_t sensor_ip, unsigned short sensor_tcp_port, CommSettings comm_settings, sick::types::ScanDataCb callback)
    : SickSafetyscannersBase(sensor_ip, sensor_tcp_port, comm_settings),
      m_scan_data_cb(callback),
      m_work(sick::make_unique<boost::asio::io_service::work>(m_io_service))
{
  m_service_thread = boost::thread([this] {
    try
    {
      m_io_service.run();
    }
    catch (const std::exception &e)
    {
      LOG_ERROR("%s", e.what());
    }
  });
}

AsyncSickSafetyScanner::AsyncSickSafetyScanner(ip_address_t sensor_ip, unsigned short sensor_tcp_port, CommSettings comm_settings, sick::types::ScanDataCb callback, boost::asio::io_service &io_service)
    : SickSafetyscannersBase(sensor_ip, sensor_tcp_port, comm_settings, io_service),
      m_scan_data_cb(),
      m_work()
{
}

AsyncSickSafetyScanner::~AsyncSickSafetyScanner()
{
  m_io_service.stop();
  m_work.reset();
  if (m_service_thread.joinable())
  {
    m_service_thread.join();
  }
}

void AsyncSickSafetyScanner::processUDPPacket(const sick::datastructure::PacketBuffer &buffer)
{
  if (m_packet_merger.addUDPPacket(buffer))
  {
    sick::datastructure::PacketBuffer deployed_buffer =
        m_packet_merger.getDeployedPacketBuffer();
    sick::data_processing::ParseData data_parser;
    sick::datastructure::Data data = data_parser.parseUDPSequence(deployed_buffer);

    m_scan_data_cb(data);
  }
}

void AsyncSickSafetyScanner::run()
{
  sick::types::PacketHandler callback = [this](const sick::datastructure::PacketBuffer &buffer) {
    processUDPPacket(buffer);
  };
  m_udp_client.run(std::move(callback));
}

void AsyncSickSafetyScanner::stop()
{
  m_udp_client.stop();
}

bool SyncSickSafetyScanner::isDataAvailable() const
{
  return m_udp_client.isDataAvailable();
}

const Data SyncSickSafetyScanner::receive(int timeout_ms)
{
  sick::data_processing::ParseData data_parser;
  while (!m_packet_merger.isComplete())
  {
    sick::datastructure::PacketBuffer buffer;
    m_udp_client.receive(buffer, boost::posix_time::seconds(2));
    // if (bytes_recv == 0) {
    //   timeval timeout;
    //   timeout.tv_sec = 2;
    //   timeout.tv_usec = 0;
    //   throw sick::timeout_error("Timeout during receiving sensor data", timeout);
    // }
    m_packet_merger.addUDPPacket(buffer);
  }
  sick::datastructure::PacketBuffer deployed_buffer =
      m_packet_merger.getDeployedPacketBuffer();
  sick::datastructure::Data data = data_parser.parseUDPSequence(deployed_buffer);
  return data;
}

} // namespace sick
