// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2020, SICK AG, Waldkirch
*  Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany
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
 * \file SickSafetyscanners.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------


#ifndef SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERS_H
#define SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERS_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

#include "sick_safetyscanners_base/Logging.h"
#include "sick_safetyscanners_base/Types.h"

#include "sick_safetyscanners_base/communication/TCPClient.h"
#include "sick_safetyscanners_base/communication/UDPClient.h"
#include "sick_safetyscanners_base/data_processing/ParseData.h"
#include "sick_safetyscanners_base/data_processing/UDPPacketMerger.h"

#include "sick_safetyscanners_base/cola2/Command.h"
#include "sick_safetyscanners_base/cola2/Cola2Session.h"
#include "sick_safetyscanners_base/datastructure/Datastructure.h"


namespace sick
{

using io_service_ptr = std::shared_ptr<boost::asio::io_service>;

using namespace sick::datastructure;

/*!
 * \brief Base class for the SICK safety scanners. This class provides a COLA2 API for the user and handles internally a COLA2 session and a UDP client for derived classes.
 * 
 */
class SickSafetyscannersBase
{
public:

  /*!
  * \brief Deleted default, copy and copy-assignment constructors.
  * 
  */
  SickSafetyscannersBase() = delete;
  SickSafetyscannersBase(const SickSafetyscannersBase &) = delete;
  SickSafetyscannersBase &operator=(const SickSafetyscannersBase &) = delete;

  /*!
   * \brief Constructor of the SickSafetyscannersBase class.
   * 
   * \param sensor_ip The IP4 address of the sensor.
   * \param sensor_tcp_port The TCP port of the sensor (COLA2).
   * \param comm_settings A CommSettings object containing parameters to be sent to the sensor. The host (client) UDP port, if not available for allocation, might be overwritten by an automatically choosen one.
   * \param io_service A boost::asio io_service instance used internally to manage sockets and threads. This constructor variant prevents creating an internal io_service and relies on the caller to perform run-calls and keep the io_service alive.
   */
  SickSafetyscannersBase(sick::types::ip_address_t sensor_ip, sick::types::port_t sensor_tcp_port, CommSettings comm_settings, boost::asio::io_service &io_service);
  /*!
   * \brief Constructor of the SickSafetyscannersBase class.
   * 
   * \param sensor_ip The IP4 address of the sensor.
   * \param sensor_tcp_port The TCP port of the sensor (COLA2).
   * \param comm_settings A CommSettings object containing parameters to be sent to the sensor. The host (client) UDP port, if not available for allocation, might be overwritten by an automatically choosen one.
   */
  SickSafetyscannersBase(sick::types::ip_address_t sensor_ip, sick::types::port_t sensor_tcp_port, CommSettings comm_settings);

  /*!
   * \brief Destructor
   */
  // TODO check if necessary
  // virtual ~SickSafetyscannersBase();

  /*!
   * \brief Changes the internal settings of the sensor.
   * \param settings New set of settings to pass to the sensor.
   */
  void changeSensorSettings(const CommSettings &settings);

  /**
   * \brief Requests the typecode of the sensor.
   * \param type_code Returned typecode.
   */
  void requestTypeCode(datastructure::TypeCode &type_code);

  /*!
   * \brief Requests the application name from the sensor.
   * 
   * \param application_name Returned application name.
   */
  void requestApplicationName(datastructure::ApplicationName &application_name);

  /*!
   * \brief Requests the serial number of the sensor.
   * 
   * \param serial_number Returned serial number.
   */
  void requestSerialNumber(datastructure::SerialNumber &serial_number);

  /*!
   * \brief Requests the firmware version of the sensor.
   * 
   * \param firmware_version Returned firmware version.
   */
  void requestFirmwareVersion(datastructure::FirmwareVersion &firmware_version);

  /*!
   * \brief Requests the order number of the sensor.
   * 
   * \param order_number Returned order number.
   */
  void requestOrderNumber(datastructure::OrderNumber &order_number);

  /*!
   * \brief Requests the project name from the sensor.
   * 
   * \param project_name Returned project name.
   */
  void requestProjectName(datastructure::ProjectName &project_name);

  /*!
   * \brief Requests the user name from the sensor.
   * 
   * \param user_name Returned user name.
   */
  void requestUserName(datastructure::UserName &user_name);

  /*!
   * \brief Requests the config metadata from the sensor.
   * 
   * \param config_metadata Returned config metadata.
   */
  void requestConfigMetadata(datastructure::ConfigMetadata &config_metadata);

  /*!
   * \brief Request a status overview from the sensor.
   * 
   * \param status_overview Returned status overview.
   */
  void requestStatusOverview(datastructure::StatusOverview &status_overview);

  /*!
   * \brief Requests the sensor's device status (e.g. useful for monitoring errors).
   * 
   * \param device_status Returned device status.
   */
  void requestDeviceStatus(datastructure::DeviceStatus &device_status);

  /*!
   * \brief Requests the 'required user action' which provides along with device status information on troubleshooting.
   * 
   * \param required_user_action Returned required user action information.
   */
  void requestRequiredUserAction(datastructure::RequiredUserAction &required_user_action);

  /*!
   * \brief Requests the latest sensor data telegram.
   * 
   * \param data Returned data.
   * \param channel_index The channel index in the range of (0-3).
   */
  void requestLatestTelegram(datastructure::Data &data, int8_t channel_index = 0);

  /*!
   * \brief Requests the sensor to let its display blink in various colors for the specified time.
   * 
   * \param blink_time The time of the display to blink [seconds].
   */
  void findSensor(uint16_t blink_time);

  /*!
   * \brief Requests data of the protective and warning fields from the sensor.
   *
   * \param field_data Returned field data.
   */
  void requestFieldData(std::vector<FieldData> &field_data);

  /*!
   * \brief Requests the name of the device from the sensor.
   *
   * \param device_name Returned device name.
   */
  void requestDeviceName(datastructure::DeviceName &device_name);

  /*!
   * \brief Requests the persistent configuration from the sensor.
   *
   * \param config_data Returned persistent configuration data.
   */
  void requestPersistentConfig(ConfigData &config_data);

  /*!
   * \brief Requests the monitoring cases from the sensor.
   *
   * \param monitoring_cases Returned monitoring cases.
   */
  void
  requestMonitoringCases(std::vector<MonitoringCaseData> &monitoring_cases);

private:
  sick::types::ip_address_t m_sensor_ip;
  sick::types::port_t m_sensor_tcp_port;
  CommSettings m_comm_settings;
  bool m_is_initialized;
  std::unique_ptr<boost::asio::io_service> m_io_service_ptr;

  /*!
   * \brief Helper function to create command objects generically.
   * 
   * 	param CommandT The command-object type.
   * 	param Args Argument list type.
   * \param args The actual passed arguments to initialize the command object.
   */
  template <class CommandT, typename... Args>
  void inline createAndExecuteCommand(Args &&... args)
  {
    auto cmd = new CommandT(std::forward<Args>(args)...);
    m_session.sendCommand(*cmd);
  }

protected:
  boost::asio::io_service &m_io_service;
  sick::communication::UDPClient m_udp_client;
  sick::cola2::Cola2Session m_session;
  sick::data_processing::UDPPacketMerger m_packet_merger;
};

/*!
 * \brief Asynchronous API for SICK safety scanners. The COLA2 API is inherited from its base class.
 * 
 */
class AsyncSickSafetyScanner final : public SickSafetyscannersBase
{
public:
  /*!
   * \brief Deleted default, copy and copy-assignment constructors.
   * 
   */
  AsyncSickSafetyScanner() = delete;
  AsyncSickSafetyScanner(const AsyncSickSafetyScanner &) = delete;
  AsyncSickSafetyScanner &operator=(const AsyncSickSafetyScanner &) = delete;

  /*!
   * \brief Constructor of the AsyncSickSafetyScanner class.
   * 
   * \param sensor_ip The IP4 address of the sensor.
   * \param sensor_tcp_port The TCP port of the sensor (COLA2).
   * \param comm_settings A CommSettings object containing parameters to be sent to the sensor. The host (client) UDP port, if not available for allocation, might be overwritten by an automatically choosen one.
   * \param callback A callback to process incomming sensor data.
   */
  AsyncSickSafetyScanner(sick::types::ip_address_t sensor_ip, sick::types::port_t sensor_tcp_port, CommSettings comm_settings, sick::types::ScanDataCb callback);

  /*!
   * \brief Constructor of the AsyncSickSafetyScanner class.
   * 
   * \param sensor_ip The IP4 address of the sensor.
   * \param sensor_tcp_port The TCP port of the sensor (COLA2).
   * \param comm_settings A CommSettings object containing parameters to be sent to the sensor. The host (client) UDP port, if not available for allocation, might be overwritten by an automatically choosen one.
   * \param callback A callback to process incomming sensor data.
   * \param io_service A boost::asio io_service instance used internally to manage sockets and threads. This constructor variant prevents creating an internal io_service and relies on the caller to perform run-calls and keep the io_service alive.
   */
  AsyncSickSafetyScanner(sick::types::ip_address_t sensor_ip, sick::types::port_t sensor_tcp_port, CommSettings comm_settings, sick::types::ScanDataCb callback, boost::asio::io_service &io_service);

  /*!
   * \brief Destructor of the AsyncSickSafetyScanner object
   * 
   */
  ~AsyncSickSafetyScanner();

  /*!
   * \brief Starts to receive sensor data via UDP and passes the data to the callback as specified in the constructor. This operation yields no timeouts.
   * 
   */
  void run();

  /*!
   * \brief Stops receiving and processing sensor data.
   * 
   */
  void stop();

private:
  /*!
   * \brief Merges incomming UDP packets and calls the callback function as passed via the constructor.
   * 
   * \param buffer Returned packet buffer.
   */
  void processUDPPacket(const sick::datastructure::PacketBuffer &buffer);

  sick::types::ScanDataCb m_scan_data_cb;
  std::unique_ptr<boost::asio::io_service> m_io_service_ptr;
  boost::thread m_service_thread;
  std::unique_ptr<boost::asio::io_service::work> m_work;
};

/*!
 * \brief Synchronous API for SICK safety scanners. The COLA2 API is inherited from its base class.
 * 
 */
class SyncSickSafetyScanner final : public SickSafetyscannersBase
{
public:
  // This adds the base class constructors to the local overload resolution set and threats local initialization like default construction would do.
  using SickSafetyscannersBase::SickSafetyscannersBase;

  /*!
   * \brief Indicates whether sensor data is available in the receiving buffers.
   * 
   * \return true Data is available.
   * \return false No data is available.
   */
  bool isDataAvailable() const;

  /*!
   * \brief Blocking call to receive one sensor data message. Throws an exception if the timeout is exceeded. 
   * 
   * \param timeout Timeout in [seconds].
   * \return const Data Returned sensor data.
   */
  const Data receive(sick::types::time_duration_t timeout = boost::posix_time::seconds(5));
};
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_SICKSAFETYSCANNERS_H
