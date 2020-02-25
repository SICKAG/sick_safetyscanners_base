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
 * \file SickSafetyscannersBase.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERSBASE_H
#define SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERSBASE_H

//#include <ros/ros.h>
#include <sick_safetyscanners_base/log.h>
#include <sick_safetyscanners_base/types.h>

#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

#include <sick_safetyscanners_base/communication/AsyncTCPClient.h>
#include <sick_safetyscanners_base/communication/AsyncUDPClient.h>
#include <sick_safetyscanners_base/data_processing/ParseData.h>
#include <sick_safetyscanners_base/data_processing/UDPPacketMerger.h>
#include <sick_safetyscanners_base/datastructure/CommSettings.h>
#include <sick_safetyscanners_base/datastructure/ConfigData.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>

#include <sick_safetyscanners_base/cola2/ApplicationNameVariableCommand.h>
#include <sick_safetyscanners_base/cola2/ChangeCommSettingsCommand.h>
#include <sick_safetyscanners_base/cola2/Cola2Session.h>
#include <sick_safetyscanners_base/cola2/ConfigMetadataVariableCommand.h>
#include <sick_safetyscanners_base/cola2/DeviceNameVariableCommand.h>
#include <sick_safetyscanners_base/cola2/DeviceStatusVariableCommand.h>
#include <sick_safetyscanners_base/cola2/FieldGeometryVariableCommand.h>
#include <sick_safetyscanners_base/cola2/FieldHeaderVariableCommand.h>
#include <sick_safetyscanners_base/cola2/FindMeCommand.h>
#include <sick_safetyscanners_base/cola2/FirmwareVersionVariableCommand.h>
#include <sick_safetyscanners_base/cola2/LatestTelegramVariableCommand.h>
#include <sick_safetyscanners_base/cola2/MeasurementCurrentConfigVariableCommand.h>
#include <sick_safetyscanners_base/cola2/MeasurementPersistentConfigVariableCommand.h>
#include <sick_safetyscanners_base/cola2/MonitoringCaseTableHeaderVariableCommand.h>
#include <sick_safetyscanners_base/cola2/MonitoringCaseVariableCommand.h>
#include <sick_safetyscanners_base/cola2/OrderNumberVariableCommand.h>
#include <sick_safetyscanners_base/cola2/ProjectNameVariableCommand.h>
#include <sick_safetyscanners_base/cola2/RequiredUserActionVariableCommand.h>
#include <sick_safetyscanners_base/cola2/SerialNumberVariableCommand.h>
#include <sick_safetyscanners_base/cola2/StatusOverviewVariableCommand.h>
#include <sick_safetyscanners_base/cola2/TypeCodeVariableCommand.h>
#include <sick_safetyscanners_base/cola2/UserNameVariableCommand.h>

namespace sick
{

using ip_address_t = boost::asio::ip::address_v4;
using namespace sick::datastructure;
/*!
 * \brief Class managing the algorithmic part of the package.
 */
class SickSafetyscannersBase
{
public:
  /*!
   *  Typedef for function which has to be passed to this class. This enables the use of
   *  functions from the calling class.
   */

  /*!
   * \brief Constructor of the SickSafetyscannersBase class.
   * called when a new packet is received.
   * \param settings Current settings for the sensor.
   */
  SickSafetyscannersBase() = default;
  SickSafetyscannersBase(ip_address_t host_ip, uint16_t host_port, ip_address_t sensor_ip, uint16_t sensor_port);

  /*!
   * \brief Destructor
   */
  virtual ~SickSafetyscannersBase() = default;

  /*!
   * \brief Changes the internal settings of the sensor.
   * \param settings New set of settings to pass to the sensor.
   */
  void changeSensorSettings(const CommSettings &settings);

  /*!
   * \brief Requests the typecode of the sensor.
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param type_code Returned typecode.
   */
  void requestTypeCode(TypeCode &type_code);

  void requestApplicationName(ApplicationName &application_name);
  void requestSerialNumber(SerialNumber &serial_number);
  void requestFirmwareVersion(FirmwareVersion &firmware_version);
  void requestOrderNumber(datastructure::OrderNumber &order_number);
  void requestProjectName(datastructure::ProjectName &project_name);
  void requestUserName(datastructure::UserName &user_name);
  void requestConfigMetadata(datastructure::ConfigMetadata &config_metadata);
  void requestStatusOverview(datastructure::StatusOverview &status_overview);
  void requestDeviceStatus(datastructure::DeviceStatus &device_status);
  void requestRequiredUserAction(datastructure::RequiredUserAction &required_user_action);
  void requestLatestTelegram(datastructure::Data &data, int8_t index = 0);
  void findSensor(uint16_t blink_time);
  /*!
   * \brief Requests data of the protective and warning fields from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param field_data Returned field data.
   */
  void requestFieldData(std::vector<FieldData> &field_data);

  /*!
   * \brief Requests the name of the device from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param device_name Returned device name.
   */
  void requestDeviceName(datastructure::DeviceName &device_name);

  /*!
   * \brief Requests the persistent configuration from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param config_data Returned persistent configuration data.
   */
  void requestPersistentConfig(ConfigData &config_data);
  /*!
   * \brief Requests the monitoring cases from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param monitoring_cases Returned monitoring cases.
   */
  void
  requestMonitoringCases(std::vector<MonitoringCaseData> &monitoring_cases);

private:
  ip_address_t m_sensor_ip{ip_address_t::from_string("192.168.1.11")};
  ip_address_t m_host_ip{ip_address_t::from_string("192.168.1.9")};
  uint16_t m_sensor_port{0};
  uint16_t m_host_port{0};
  CommSettings m_comm_settings{};
  sick::cola2::Cola2Session m_session{};

  // dataReceivedCb m_newPacketReceivedCallbackFunction;

  // std::shared_ptr<boost::asio::io_service> m_io_service_ptr;
  // std::shared_ptr<boost::asio::io_service::work> m_io_work_ptr;
  // std::shared_ptr<sick::communication::AsyncUDPClient> m_async_udp_client_ptr;
  // std::shared_ptr<sick::communication::AsyncTCPClient> m_async_tcp_client_ptr;
  // boost::scoped_ptr<boost::thread> m_udp_client_thread_ptr;

  // std::shared_ptr<sick::cola2::Cola2Session> m_session_ptr;

  // std::shared_ptr<sick::data_processing::UDPPacketMerger> m_packet_merger_ptr;

  void processUDPPacket(const PacketBuffer &buffer);
  bool udpClientThread();
  void processTCPPacket(const PacketBuffer &buffer);
  void startTCPConnection(const CommSettings &settings);
  void changeCommSettingsInColaSession(const datastructure::CommSettings &settings);
  void stopTCPConnection();
  void requestTypeCodeInColaSession(TypeCode &type_code);
  void requestFieldDataInColaSession(std::vector<FieldData> &fields);
  void requestDeviceNameInColaSession(datastructure::DeviceName &device_name);
  void requestApplicationNameInColaSession(ApplicationName &application_name);
  void requestSerialNumberInColaSession(SerialNumber &serial_number);
  void requestOrderNumberInColaSession(OrderNumber &order_number);
  void requestProjectNameInColaSession(ProjectName &project_name);
  void requestUserNameInColaSession(UserName &user_name);
  void requestFirmwareVersionInColaSession(FirmwareVersion &firmware_version);
  void requestPersistentConfigInColaSession(ConfigData &config_data);
  void requestConfigMetadataInColaSession(ConfigMetadata &config_metadata);
  void requestStatusOverviewInColaSession(StatusOverview &status_overview);
  void requestDeviceStatusInColaSession(DeviceStatus &device_status);
  void requestRequiredUserActionInColaSession(
      RequiredUserAction &required_user_action);
  void requestMonitoringCaseDataInColaSession(
      std::vector<MonitoringCaseData> &monitoring_cases);
  void requestLatestTelegramInColaSession(Data &data, int8_t index = 0);
  void findSensorInColaSession(uint16_t blink_time);
};

class AsyncSickSafetyScanner : public SickSafetyscannersBase
{
public:
  // SickSafetyScannerAsync() : SickSafetyscannersBase(){};

  /*!
   * \brief Start the connection to the sensor and enables output.
   * \return If the setup was correct.
   */
  void run(sick::types::DataReceivedCb callback);
  void stop();
};

class SyncSickSafetyScanner : public SickSafetyscannersBase
{
public:
  void waitForData(long timeout_ms = 1000) const;
  const Data getData();
  void stop();
};

} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERSBASE_H
