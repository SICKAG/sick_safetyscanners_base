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

#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <future>

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

namespace sick {

/*!
 * \brief Class managing the algorithmic part of the package.
 */
class SickSafetyscannersBase
{
private:
  typedef std::promise<boost::system::error_code> CompletePromise;
  typedef std::shared_ptr<CompletePromise> CompletePromisePtr;

public:
  /*!
   *  Typedef for function which has to be passed to this class. This enables the use of
   *  functions from the calling class.
   */
  typedef boost::function<void(const sick::datastructure::Data&)> packetReceivedCallbackFunction;

  typedef std::function<void(const boost::system::error_code&)> AsyncCompleteHandler;

  /*!
   * \brief Constructor of the SickSafetyscannersBase class.
   * \param newPacketReceivedCallbackFunction Function from the calling class, which will be
   * called when a new packet is received.
   * \param settings Current settings for the sensor.
   */
  SickSafetyscannersBase(const packetReceivedCallbackFunction& newPacketReceivedCallbackFunction,
                         sick::datastructure::CommSettings* settings);


  /**
   * \brief Constructor of the SickSafetyscannersBase class.
   * \param io_service
   * \param newPacketReceivedCallbackFunction Function from the calling class, which will be
   * called when a new packet is received.
   * \param settings Current settings for the sensor.
   */
  SickSafetyscannersBase(boost::asio::io_service& io_service,
                         const packetReceivedCallbackFunction& newPacketReceivedCallbackFunction,
                         sick::datastructure::CommSettings* settings);

  /*!
   * \brief Destructor
   */
  virtual ~SickSafetyscannersBase();

  /*!
   * \brief Start the connection to the sensor and enables output.
   * \return If the setup was correct.
   */
  bool run();

  /**
   * \brief stop
   */
  void stop();

  /*!
   * \brief Changes the internal settings of the sensor.
   * \param settings New set of settings to pass to the sensor.
   */
  void changeSensorSettings(const sick::datastructure::CommSettings& settings);
  void asyncChangeSensorSettings(const sick::datastructure::CommSettings& settings,
                                 AsyncCompleteHandler handler);

  void findSensor(const datastructure::CommSettings& settings, uint16_t blink_time);
  void asyncFindSensor(const datastructure::CommSettings& settings, uint16_t blink_time,
                       AsyncCompleteHandler handler);

  /*!
   * \brief Requests the typecode of the sensor.
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param type_code Returned typecode.
   */
  void requestTypeCode(const sick::datastructure::CommSettings& settings,
                       sick::datastructure::TypeCode& type_code);
  void asyncRequestTypeCode(const sick::datastructure::CommSettings& settings,
                            sick::datastructure::TypeCode& type_code,
                            AsyncCompleteHandler handler);


  void requestApplicationName(const sick::datastructure::CommSettings& settings,
                              sick::datastructure::ApplicationName& application_name);
  void asyncRequestApplicationName(const sick::datastructure::CommSettings& settings,
                                   sick::datastructure::ApplicationName& application_name,
                                   AsyncCompleteHandler handler);

  void requestSerialNumber(const sick::datastructure::CommSettings& settings,
                           sick::datastructure::SerialNumber& serial_number);
  void asyncRequestSerialNumber(const sick::datastructure::CommSettings& settings,
                                sick::datastructure::SerialNumber& serial_number,
                                AsyncCompleteHandler handler);

  void requestFirmwareVersion(const sick::datastructure::CommSettings& settings,
                              sick::datastructure::FirmwareVersion& firmware_version);
  void asyncRequestFirmwareVersion(const sick::datastructure::CommSettings& settings,
                                   sick::datastructure::FirmwareVersion& firmware_version,
                                   AsyncCompleteHandler handler);

  void requestOrderNumber(const datastructure::CommSettings& settings,
                          datastructure::OrderNumber& order_number);
  void asyncRequestOrderNumber(const datastructure::CommSettings& settings,
                               datastructure::OrderNumber& order_number,
                               AsyncCompleteHandler handler);

  void requestProjectName(const datastructure::CommSettings& settings,
                          datastructure::ProjectName& project_name);
  void asyncRequestProjectName(const datastructure::CommSettings& settings,
                               datastructure::ProjectName& project_name,
                               AsyncCompleteHandler handler);

  void requestUserName(const datastructure::CommSettings& settings,
                       datastructure::UserName& user_name);
  void asyncRequestUserName(const datastructure::CommSettings& settings,
                            datastructure::UserName& user_name,
                            AsyncCompleteHandler handler);

  void requestConfigMetadata(const datastructure::CommSettings& settings,
                             datastructure::ConfigMetadata& config_metadata);
  void asyncRequestConfigMetadata(const datastructure::CommSettings& settings,
                                  datastructure::ConfigMetadata& config_metadata,
                                  AsyncCompleteHandler handler);

  void requestStatusOverview(const datastructure::CommSettings& settings,
                             datastructure::StatusOverview& status_overview);
  void asyncRequestStatusOverview(const datastructure::CommSettings& settings,
                                  datastructure::StatusOverview& status_overview,
                                  AsyncCompleteHandler handler);

  void requestDeviceStatus(const datastructure::CommSettings& settings,
                           datastructure::DeviceStatus& device_status);
  void asyncRequestDeviceStatus(const datastructure::CommSettings& settings,
                                datastructure::DeviceStatus& device_status,
                                AsyncCompleteHandler handler);

  void requestRequiredUserAction(const datastructure::CommSettings& settings,
                                 datastructure::RequiredUserAction& required_user_action);
  void asyncRequestRequiredUserAction(const datastructure::CommSettings& settings,
                                      datastructure::RequiredUserAction& required_user_action,
                                      AsyncCompleteHandler handler);

  /*!
   * \brief Requests the name of the device from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param device_name Returned device name.
   */
  void requestDeviceName(const sick::datastructure::CommSettings& settings,
                         datastructure::DeviceName& device_name);
  void asyncRequestDeviceName(const sick::datastructure::CommSettings& settings,
                              datastructure::DeviceName& device_name,
                              AsyncCompleteHandler handler);


  /*!
   * \brief Requests data of the protective and warning fields from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param field_data Returned field data.
   */
  void requestFieldData(const sick::datastructure::CommSettings& settings,
                        std::vector<sick::datastructure::FieldData>& field_data);
  void asyncRequestFieldData(const sick::datastructure::CommSettings& settings,
                             std::vector<sick::datastructure::FieldData>& field_data,
                             AsyncCompleteHandler handler);

  /*!
   * \brief Requests the persistent configuration from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param config_data Returned persistent configuration data.
   */
  void requestPersistentConfig(const datastructure::CommSettings& settings,
                               sick::datastructure::ConfigData& config_data);
  void asyncRequestPersistentConfig(const datastructure::CommSettings& settings,
                                    sick::datastructure::ConfigData& config_data,
                                    AsyncCompleteHandler handler);

  /*!
   * \brief Requests the monitoring cases from the sensor.
   *
   * \param settings Settings containing information to establish a connection to the sensor.
   * \param monitoring_cases Returned monitoring cases.
   */
  void requestMonitoringCases(const sick::datastructure::CommSettings& settings,
                              std::vector<sick::datastructure::MonitoringCaseData>& monitoring_cases);
  void asyncRequestMonitoringCases(const sick::datastructure::CommSettings& settings,
                                   std::vector<sick::datastructure::MonitoringCaseData>& monitoring_cases,
                                   AsyncCompleteHandler handler);

private:
  packetReceivedCallbackFunction m_newPacketReceivedCallbackFunction;

  std::shared_ptr<boost::asio::io_service> m_io_service_ptr;
  boost::reference_wrapper< boost::asio::io_service > m_io_service;

  std::shared_ptr<sick::communication::AsyncUDPClient> m_async_udp_client_ptr;
  boost::scoped_ptr<boost::thread> m_udp_client_thread_ptr;

  std::shared_ptr<sick::data_processing::UDPPacketMerger> m_packet_merger_ptr;

  void processUDPPacket(const sick::datastructure::PacketBuffer& buffer);
  bool udpClientThread();
};

} // namespace sick


#endif // SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERSBASE_H
