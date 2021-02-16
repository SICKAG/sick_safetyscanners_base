# Sick_Safetyscanners_Base CPP Driver

## Table of contents

- [Supported Hardware](#supported-hardware)
- [Getting started](#getting-started)
- [API](#api)
- [Creators](#creators)

A CPP standalone Driver which reads the raw data from the SICK Safety Scanners and takes custom functions to publish the data.

## Supported Hardware
Supported are all microScan3, nanoScan3 and outdoorScan3 variants with Ethernet connection.

![ ](docs/images/safetyscanners.png  "Sick microScan3")



## Getting started

The driver will be released on this github repository, and can then be installed from source.

### Prerequisites

* Linux
* Correctly setup SICK Safety Scanner
* Connected SICK Safety Scanner and a correctly setup ethernet network. Both the host and the sensor have to be in the same network.
* Installed libboost

### Installation

For installation this github repository has to be cloned and afterwards installed. If a custom install directory is wanted use the -DCMAKE_INSTALL_PREFIX option to specify a path.

```bash
git clone https://github.com/SICKAG/sick_safetyscanners_base.git
cd sick_safetyscanners_base
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<path to install folder> ..
make -j8
make install
```

### Usage

To use the library in a driver the path of the installation has to be added to the cmake prefix path of your application. You can achieve this by using, bevor invoking cmake on your application.

```
export CMAKE_PREFIX_PATH=<path to install folder>
```

Afterwards the driver and the settings for the driver can be included with:
```
#include <sick_safetyscanners_base/SickSafetyscanners.h>
#include <sick_safetyscanners_base/Exceptions.h>
#include <sick_safetyscanners_base/Types.h>
#include <sick_safetyscanners_base/datastructure/CommSettings.h>
```

To get the driver up and running you need first to choose between the synchronous and asynchronous APIs based on your needs.

In the latter case you can also pass an instance of boost::asio::io_service to the constructor of the AsyncSickSafetyScanner. 


## API

### Synchronous Client

In cases where you do not want the driver to spawn internal child threads to asynchronously process incomming sensor data you can use the ```SyncSickSafetyScanner``` class.

| Function                                                                                                                                                  | Information                                                                                                                                          |
| --------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| SyncSickSafetyScanner<br>(sick::types::ip_address_t sensor_ip, <br>sick::types::port_t sensor_port, <br>sick::datastructure::CommSettings comm_settings); | Constructor call.                                                                                                                                    |
| bool isDataAvailable();                                                                                                                                   | Non-blocking call that indicates wheether sensor data is available to fetch via the receive-function from the internal sensor data receiving buffer. |
| const Data receive<br>(sick::types::time_duration_t timeout = boost::posix_time::pos_infin);                                                              | Blocking call to receive one sensor data packet at a time.                                                                                           |

Example
```
// Sensor IP and Port
std::string sensor_ip_str = 192.168.1.11
sick::types::ip_address_t sensor_ip = boost::asio::ip::address_v4::from_string(sensor_ip_str);
sick::types::port_t tcp_port {2122};

// Prepare the CommSettings for Sensor streaming data
sick::datastructure::CommSettings comm_settings;
std::string host_ip_str = "192.168.1.9"
comm_settings.host_ip = boost::asio::ip::address_v4::from_string(host_ip_str);
comm_settings.host_udp_port = 0;

// Create a sensor instance
auto safety_scanner = std::make_uniqe<sick::SyncSickSafetyScanner>(sensor_ip, tcp_port, comm_settings);

// Receive one sensor data packet
auto timeout = boost::posix_time::seconds(5);
sick::datastructure::Data data = safety_scanner->receive(timeout);

// ...
```

### Asynchronous Client

If you dont need restrict your client to work with single-threaded blocking receive calls in a strict sequential manner you can use the ```AsyncSickSafetyScanner``` class. The sensor data callback required by the constructor has the following signature
```
void callback(const sick::datastructure::Data& data);
```

| Function                                                                                                                                                  | Information                                                                                                                                          |
| --------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| AsyncSickSafetyScanner<br>(sick::types::ip_address_t sensor_ip, <br>sick::types::port_t sensor_port, <br>sick::datastructure::CommSettings comm_settings, <br>sick::types::ScanDataCb callback); | Constructor call with custom data sensor callback.                                
| AsyncSickSafetyScanner<br>(sick::types::ip_address_t sensor_ip, <br>sick::types::port_t sensor_port, <br>sick::datastructure::CommSettings comm_settings, <br>sick::types::ScanDataCb callback, <br>boost::asio::io_service& io_service); | Constructor call. This variant allows the user to pass through an instance of boost::asio::io_service to get full control over the thread execution. In this case the driver is not spawning an internal child thread but relies on the user to perform regular io_service.run() operations and keep the service busy.
| void run(); | Starts to receive sensor data via UDP and passes the data to the callback as specified in the constructor.
| void stop(); | Stops all asynchronous receiving and processing operations.


Example
```
// Sensor IP and Port
std::string sensor_ip_str = 192.168.1.11
sick::types::ip_address_t sensor_ip = boost::asio::ip::address_v4::from_string(sensor_ip_str);
sick::types::port_t tcp_port {2122};

// Prepare the CommSettings for Sensor streaming data
sick::datastructure::CommSettings comm_settings;
std::string host_ip_str = "192.168.1.9"
comm_settings.host_ip = boost::asio::ip::address_v4::from_string(host_ip_str);
comm_settings.host_udp_port = 0;

// Define a sensor data callback
sick::types::ScanDataCb cb = [](const sick::datastructure::Data &data) {
    std::cout << "Number of beams: " << data.getMeasurementDataPtr()->getNumberOfBeams() << std::endl;
};

// Create a sensor instance
auto safety_scanner = std::make_unique<sick::AsyncSickSafetyScanner>(sensor_ip, tcp_port, comm_settings, cb);

// Start async receiving and processing of sensor data
safety_scanner->run();

// ... Do other stuff

// Stop async processing
safety_scanner->stop();
```

### Parameters of Communication Settings

The parameters can be set using the setters of the CommSettings class. To set for example to host_ip the following function can be called.

```
sick::datastructure::CommSettings m_communication_settings;
m_communication_settings.setHostIp("192.168.1.100");
```

| Parameter Name       | API                                                                                                                                                                                 | Default      | Information                                                                                                                                               |
| -------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| sensor_ip            | void setSensorIp(const std::string& sensor_ip);                                                                                                                                     | 192.168.1.11 | Sensor IP address. Can be passed as an argument to the launch file.                                                                                       |
| host_ip              | void setHostIp(const std::string& host_ip);                                                                                                                                         | 192.168.1.9  | Host IP address.  Can be passed as an argument to the launch file.                                                                                        |
| host_udp_port        | void setHostUdpPort(const uint16_t& host_udp_port);                                                                                                                                 | 0            | Host UDP Port.  Can be passed as an argument to the launch file.  Zero allows system chosen port.                                                         |
| publishing_frequency | void setPublishingFrequency(const uint16_t& publishing_frequency)                                                                                                                   | 1            | Publish every n_th scan, where n is the publishing frequency.  For a 25Hz laser, setting  to 1 makes it publish at 25Hz, to 2 makes it publish at 12.5Hz. |
| start_angle          | void setStartAngle(const uint32_t& start_angle)                                                                                                                                     | 0.0          | Start angle of scan in radians, if both start and end angle are equal, all angels are regarded.  0° is at the front of the scanner.                       |
| end_angle            | void setEndAngle(const uint32_t& start_angle)                                                                                                                                       | 0.0          | End angle of scan in radians, if both start and end angle are equal, all angels are regarded.  0° is at the front of the scanner.                         |
| channel_enabled      | void setEnabled(bool enabled);                                                                                                                                                      | true         | If the channel should be enabled                                                                                                                          |
| e_interface_type     | void setEInterfaceType(const uint8_t& e_interface_type)                                                                                                                             | 0            | Sets the interface type of the sensor <br>0: EFI-pro <br>1:  EtherNet/IP <br>3:  Profinet<br>4: Non-safe Ethernet                                         |
| features             | void CommSettings::setFeatures<br>(<br> bool general_system_state,<br> bool derived_settings, <br> bool measurement_data, <br> bool intrusion_data, <br> bool application_data<br>) | all true     | Enables the individual data outputs.                                                                                                                      |


### COLA2 Functions (available on both driver API variants)

The Library allows to access variables of the sensor and invoke methods to change settings using the COLA2 protocol. The following methods can be called:

| Function                                                                                                                                                         | Information                                                                                                                                                                                  |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| void changeSensorSettings<br>(const sick::datastructure::CommSettings& settings)                                                                                 | Updates the sensor settings to the passed configuration.                                                                                                                                      |
| void findSensor<br>(<br>uint16_t blink_time);                                                                 | Lets the sensor flash the display for the specified time.                                                                                                                                    |
| void requestApplicationName<br>(sick::datastructure::ApplicationName& application_name);                  | Returns the name of the current application.                                                                                                                                                  |
| void requestConfigMetadata<br>(sick::datastructure::ConfigMetadata& config_metadata);                     | Returns the metadata of the current configuration of the sensor.                                                                                                                              |
| void requestDeviceName<br>(sick::datastructure::DeviceName& device_name);                                 | Returns the device name.                                                                                                                                                                      |
| void requestDeviceStatus<br>(sick::datastructure::DeviceStatus& device_status);                           | Returns the device status.                                                                                                                                                                    |
| void requestFieldData<br>(std::vector< sick::datastructure::FieldData>& field_data);                     | Returns the field data of the warning and safety fields.                                                                                                                                      |
| void requestFirmwareVersion<br>(sick::datastructure::FirmwareVersion& firmware_version);                  | Returns the firmware version.                                                                                                                                                                 |
| void requestLatestTelegram<br>(sick::datastructure::Data& data, <br> int8_t index = 0);                   | Returns the latest telegram for the channel index. Up to 4 Channels can be supported by the cola protocol, the actual number depends on the used scanner. Channel 0 is set as default value. |
| void requestMonitoringCases<br>(std::vector< sick::datastructure::MonitoringCaseData>& monitoring_cases); | Returns the data of the Monitoring Cases.                                                                                                                                                    |
| void requestOrderNumber<br>(sick::datastructure::OrderNumber& order_number);                              | Returns the order Number.                                                                                                                                                                     |
| void requestPersistentConfig<br>(sick::datastructure::ConfigData& config_data);                           | Returns the Persistent configuration of the sensor, which was set in the Safety Designer.                                                                                                     |
| void requestProjectName<br>(sick::datastructure::ProjectName& project_name);                              | Returns the project name.                                                                                                                                                                     |
| void requestRequiredUserAction<br>(sick::datastructure::RequiredUserAction& required_user_action);       | Returns the required user actions as specified in the cola 2 manual.                                                                                                                         |
| void requestSerialNumber<br>(sick::datastructure::SerialNumber& serial_number);                           | Returns the serial number of the sensor.                                                                                                                                                      |
| void requestStatusOverview<br>(sick::datastructure::StatusOverview& status_overview);                     | Returns the status overview.                                                                                                                                                                  |
| void requestTypeCode<br>(sick::datastructure::TypeCode& type_code)                                 | Returns the type code of the sensor.                                                                                                                                                          |
| void requestUserName<br>(sick::datastructure::UserName& user_name);                                       | Returns the user name.                                                                                                                                                                        |

### Troubleshooting

* Check if the sensor has power and is connected to the host.
* Check if both sensor and host are in the same subnet e.g. 192.168.1
* Are the correct IPs configured for the application?
* Is the correct Interface Type configured?





## Creators

**Lennart Puck** <br>
**Martin Schulze**

FZI Forschungszentrum Informatik


- <http://www.fzi.de>

on behalf of SICK AG 

- <http://www.sick.com>




