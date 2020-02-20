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
  SickSafetyscannersBase(std::shared_ptr<sick::datastructure::CommSettings> settings);

  /*!
   * \brief Destructor
   */
  virtual ~SickSafetyscannersBase();

  /*!
   * \brief Changes the internal settings of the sensor.
   * \param settings New set of settings to pass to the sensor.
   */
  void changeSensorSettings(const sick::datastructure::CommSettings &settings);

  /*!
   * \brief Requests the typecode of the sensor.
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param type_code Returned typecode.
   */
  void requestTypeCode(const sick::datastructure::CommSettings &settings,
                       sick::datastructure::TypeCode &type_code);

  void requestApplicationName(const sick::datastructure::CommSettings &settings,
                              sick::datastructure::ApplicationName &application_name);
  void requestSerialNumber(const sick::datastructure::CommSettings &settings,
                           sick::datastructure::SerialNumber &serial_number);
  void requestFirmwareVersion(const sick::datastructure::CommSettings &settings,
                              sick::datastructure::FirmwareVersion &firmware_version);
  void requestOrderNumber(const datastructure::CommSettings &settings,
                          datastructure::OrderNumber &order_number);
  void requestProjectName(const datastructure::CommSettings &settings,
                          datastructure::ProjectName &project_name);
  void requestUserName(const datastructure::CommSettings &settings,
                       datastructure::UserName &user_name);
  void requestConfigMetadata(const datastructure::CommSettings &settings,
                             datastructure::ConfigMetadata &config_metadata);
  void requestStatusOverview(const datastructure::CommSettings &settings,
                             datastructure::StatusOverview &status_overview);
  void requestDeviceStatus(const datastructure::CommSettings &settings,
                           datastructure::DeviceStatus &device_status);
  void requestRequiredUserAction(const datastructure::CommSettings &settings,
                                 datastructure::RequiredUserAction &required_user_action);
  void requestLatestTelegram(const datastructure::CommSettings &settings,
                             datastructure::Data &data,
                             int8_t index = 0);
  void findSensor(const datastructure::CommSettings &settings, uint16_t blink_time);
  /*!
   * \brief Requests data of the protective and warning fields from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param field_data Returned field data.
   */
  void requestFieldData(const sick::datastructure::CommSettings &settings,
                        std::vector<sick::datastructure::FieldData> &field_data);

  /*!
   * \brief Requests the name of the device from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param device_name Returned device name.
   */
  void requestDeviceName(const sick::datastructure::CommSettings &settings,
                         datastructure::DeviceName &device_name);

  /*!
   * \brief Requests the persistent configuration from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param config_data Returned persistent configuration data.
   */
  void requestPersistentConfig(const datastructure::CommSettings &settings,
                               sick::datastructure::ConfigData &config_data);
  /*!
   * \brief Requests the monitoring cases from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param monitoring_cases Returned monitoring cases.
   */
  void
  requestMonitoringCases(const sick::datastructure::CommSettings &settings,
                         std::vector<sick::datastructure::MonitoringCaseData> &monitoring_cases);

private:
  // dataReceivedCb m_newPacketReceivedCallbackFunction;

  // std::shared_ptr<boost::asio::io_service> m_io_service_ptr;
  // std::shared_ptr<boost::asio::io_service::work> m_io_work_ptr;
  // std::shared_ptr<sick::communication::AsyncUDPClient> m_async_udp_client_ptr;
  // std::shared_ptr<sick::communication::AsyncTCPClient> m_async_tcp_client_ptr;
  // boost::scoped_ptr<boost::thread> m_udp_client_thread_ptr;

  // std::shared_ptr<sick::cola2::Cola2Session> m_session_ptr;

  // std::shared_ptr<sick::data_processing::UDPPacketMerger> m_packet_merger_ptr;

  void processUDPPacket(const sick::datastructure::PacketBuffer &buffer);
  bool udpClientThread();
  void processTCPPacket(const sick::datastructure::PacketBuffer &buffer);
  void startTCPConnection(const sick::datastructure::CommSettings &settings);
  void changeCommSettingsInColaSession(const datastructure::CommSettings &settings);
  void stopTCPConnection();
  void requestTypeCodeInColaSession(sick::datastructure::TypeCode &type_code);
  void requestFieldDataInColaSession(std::vector<sick::datastructure::FieldData> &fields);
  void requestDeviceNameInColaSession(datastructure::DeviceName &device_name);
  void requestApplicationNameInColaSession(sick::datastructure::ApplicationName &application_name);
  void requestSerialNumberInColaSession(sick::datastructure::SerialNumber &serial_number);
  void requestOrderNumberInColaSession(sick::datastructure::OrderNumber &order_number);
  void requestProjectNameInColaSession(sick::datastructure::ProjectName &project_name);
  void requestUserNameInColaSession(sick::datastructure::UserName &user_name);
  void requestFirmwareVersionInColaSession(sick::datastructure::FirmwareVersion &firmware_version);
  void requestPersistentConfigInColaSession(sick::datastructure::ConfigData &config_data);
  void requestConfigMetadataInColaSession(sick::datastructure::ConfigMetadata &config_metadata);
  void requestStatusOverviewInColaSession(sick::datastructure::StatusOverview &status_overview);
  void requestDeviceStatusInColaSession(sick::datastructure::DeviceStatus &device_status);
  void requestRequiredUserActionInColaSession(
      sick::datastructure::RequiredUserAction &required_user_action);
  void requestMonitoringCaseDataInColaSession(
      std::vector<sick::datastructure::MonitoringCaseData> &monitoring_cases);
  void requestLatestTelegramInColaSession(sick::datastructure::Data &data, int8_t index = 0);
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
  void run(DataReceivedCb callback);
  void stop();
};

class SyncSickSafetyScanner : public SickSafetyscannersBase
{
public:
  void waitForData(long timeout_ms = 1000) const;
  const sick::datastructure::Data getData();
  void stop();
};

} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERSBASE_H
