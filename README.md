# Sick_Safetyscanners_Base CPP Driver

## Table of contents

- [Supported Hardware](#supported-hardware)
- [Getting started](#getting-started)
- [API](#api)
- [Creators](#creators)

A CPP standalone Driver which reads the raw data from the SICK Safety Scanners and takes custom functions to publish the data.

## Supported Hardware
Supported are all microScan3, nanoScan3 and outdoorScan3 variants with Ethernet connection.

![ ](docs/images/safetyscanners.jpeg  "Sick microScan3")



## Getting started

The driver will be released on this github repository, and can then be installed from source.

### Prerequisites

* Linux
* Correctly setup SICK Safety Scanner
* Connected SICK Safety Scanner and a correctly setup ethernet network. Both the host and the sensor have to be in the same network.

### Installation

For installation this github repository has to be cloned and afterwards installed. If a custom install directory is wanted use the -DCMAKE_INSTALL_PREFIX option to specify a path.

```bash
git clone https://github.com/SICKAG/sick_safetyscanners_base.git
cd sick_safetyscanners_base
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<path to install folder> ..
make -j9
make install
```

### Usage

To use the library in a driver the path of the installation has to be added to the cmake prefix path of your application. You can achieve this by using, bevor invoking cmake on your application.

```
export CMAKE_PREFIX_PATH=<path to install folder>
```

Afterwards the driver and the settings for the driver can be included with:
```
#include <sick_safetyscanners_base/SickSafetyscannersBase.h> 
#include <sick_safetyscanners_base/datastructure/CommSettings.h>
```

To setup the driver one has to invoke the constructor and pass a function and the settings for communication. The function then will be executed everytime a new scan from the sensor arrives and gives access to the data.

```
sick::datastructure::CommSettings m_communication_settings;

#Set the correct parameters for the communication settings, otherwise the default parameters will be used. 
#For example:
m_communication_settings.setHostIp("192.168.1.100");

std::shared_ptr<sick::SickSafetyscannersBase> m_device;
m_device = std::make_shared<sick::SickSafetyscannersBase>(boost::bind(&your_class::your_function, this, _1), &m_communication_settings);
m_device->run();  
```

With the passed function having the following declaration:

```
void your_function(const sick::datastructure::Data& data) ;
```

An Example can be found in the sick_safetyscanners ROS Driver: https://github.com/SICKAG/sick_safetyscanners/blob/master/src/SickSafetyscannersRos.cpp

## API

### Parameters of Communication Settings

The parameters can be set using the setters of the CommSettings class. To set for example to host_ip the following function can be called.

```
sick::datastructure::CommSettings m_communication_settings;
m_communication_settings.setHostIp("192.168.1.100");
```

| Parameter Name        | API | Default  | Information |
| -------------                         | ------|------- | ------------- |
| sensor_ip                    | void setSensorIp(const std::string& sensor_ip); | 192.168.1.11  |Sensor IP address. Can be passed as an argument to the launch file. |
| host_ip                        |   void setHostIp(const std::string& host_ip);  | 192.168.1.9  | Host IP address.  Can be passed as an argument to the launch file.  |
| host_udp_port             | void setHostUdpPort(const uint16_t& host_udp_port);  | 0 | Host UDP Port.  Can be passed as an argument to the launch file.  Zero allows system chosen port. |
| publishing_frequency    | void setPublishingFrequency(const uint16_t& publishing_frequency) | 1 | Publish every n_th scan, where n is the publishing frequency.  For a 25Hz laser, setting  to 1 makes it publish at 25Hz, to 2 makes it publish at 12.5Hz. |
| start_angle              | void setStartAngle(const uint32_t& start_angle) |  0.0 | Start angle of scan in radians, if both start and end angle are equal, all angels are regarded.  0° is at the front of the scanner. |
| end_angle                | void setEndAngle(const uint32_t& start_angle) | 0.0 | End angle of scan in radians, if both start and end angle are equal, all angels are regarded.  0° is at the front of the scanner. |
| channel_enabled     | void setEnabled(bool enabled); | true | If the channel should be enabled  |
|e_interface_type | void setEInterfaceType(const uint8_t& e_interface_type) | 0 | Sets the interface type of the sensor <br>0: EFI-pro <br>1:  EtherNet/IP <br>3:  Profinet<br>4: Non-safe Ethernet
| features | void CommSettings::setFeatures<br>(<br> bool general_system_state,<br> bool derived_settings, <br> bool measurement_data, <br> bool intrusion_data, <br> bool application_data<br>) | all true | Enables the individual data outputs. |


### Functions

Thd Library allows to access variables of the sensor and invoke methods to change settings using the cola2 protocol. The following methods can be called:

|  Function | Information |
|-------------|-----------------|
| void changeSensorSettings<br>(const sick::datastructure::CommSettings& settings) | Updates the sensor settigns to the passed configuration |
| void findSensor<br>(const sick::datastructure::CommSettings& settings, <br>uint16_t blink_time); | Lets the sensor flash the display for the specified time. |
| void requestApplicationName<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::ApplicationName& application_name); | Returns the name of the current application  |
| void requestConfigMetadata<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::ConfigMetadata& config_metadata); | Returns the metadata of the current configuration of the sensor |
| void requestDeviceName<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::DeviceName& device_name);  | Returns the device name |
| void requestDeviceStatus<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::DeviceStatus& device_status); | Returns the device status |
| void requestFieldData<br>(const sick::datastructure::CommSettings& settings,  <br>std::vector< sick::datastructure::FieldData>& field_data); | Returns the field data of the warning and safety fields |
| void requestFirmwareVersion<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::FirmwareVersion& firmware_version); | Returns the firmware version |
| void requestLatestTelegram<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::Data& data, <br> int8_t index = 0); | Returns the latest telegram for the channel index. Up to 4 Channels can be supported by the cola protocol, the actual number depends on the used scanner. Channel 0 is set as default value. |
| void requestMonitoringCases<br>(const sick::datastructure::CommSettings& settings, <br>std::vector< sick::datastructure::MonitoringCaseData>& monitoring_cases); | Returns the data of the  Monitoring Cases|
 | void requestOrderNumber<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::OrderNumber& order_number); | Returns the order Number|
 | void requestPersistentConfig<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::ConfigData& config_data);  | Returns the Persistent configuration of the sensor, which was set in the Safety Designer|
 | void requestProjectName<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::ProjectName& project_name); |  Returns the project name|
 | void requestRequiredUserAction<br>(const sick::datastructure::CommSettings& settings,  <br>sick::datastructure::RequiredUserAction& required_user_action); | Returns the required user actions as specified in the cola 2 manual.|
| void requestSerialNumber<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::SerialNumber& serial_number); | Returns the serial number of the sensor |
| void requestStatusOverview<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::StatusOverview& status_overview);  | Returns the status overview  |
| void requestTypeCode<br>(const sick::datastructure::CommSettings& settings,        <br>sick::datastructure::TypeCode& type_code) | Returns the type code of the sensor |
| void requestUserName<br>(const sick::datastructure::CommSettings& settings, <br>sick::datastructure::UserName& user_name);  | Returns the user name |

### Troubleshooting

* Check if the sensor has power and is connected to the host.
* Check if both sensor and host are in the same subnet e.g. 192.168.1
* Are the correct IPs configured for the application?
* Is the correct Interface Type configured?





## Creators

**Lennart Puck** 

FZI Forschungszentrum Informatik


- <http://www.fzi.de>

on behalf of SICK AG 

- <http://www.sick.com>




