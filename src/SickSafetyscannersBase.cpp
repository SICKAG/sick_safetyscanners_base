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

#include <sick_safetyscanners_base/logging/logging_wrapper.h>

#include "sick_safetyscanners_base/SickSafetyscannersBase.h"

namespace sick {


namespace detail
{
  typedef std::shared_ptr<sick::cola2::Cola2Session> Cola2SessionPtr;
  typedef std::function<void(const boost::system::error_code&, Cola2SessionPtr )> CompleteHandler;
  typedef std::function<void(const boost::system::error_code&)> AsyncCompleteHandler;

  /**
   * @brief openCola2Session
   *
   * @note The given handler will not be called from within this method
   *
   * @param ios
   * @param settings
   * @param handler The handler that should be called if the session is opened or an error occured
   */
  void openCola2Session(boost::asio::io_service& ios,
                        const sick::datastructure::CommSettings& settings,
                        CompleteHandler handler)
  {
    std::shared_ptr<sick::communication::AsyncTCPClient> async_tcp_client =
      std::make_shared<sick::communication::AsyncTCPClient>(
        ios,
        settings.getSensorIp(),
        settings.getSensorTcpPort());

    async_tcp_client->doConnect( [async_tcp_client, handler]( boost::system::error_code ec ) {
      if ( !ec )
      {
        Cola2SessionPtr session = std::make_shared<sick::cola2::Cola2Session>(async_tcp_client);
        session->open( std::bind(handler, std::placeholders::_1, session) );
      }
      else
      {
        handler( ec, Cola2SessionPtr() );
      }
    });
  }

  template< typename U, typename T >
  void processCola2Command(boost::asio::io_service& ios,
                           const datastructure::CommSettings& settings,
                           std::reference_wrapper<T> command,
                           AsyncCompleteHandler handler)
  {
    openCola2Session(ios, settings, [command, handler](const boost::system::error_code& ec, Cola2SessionPtr session) {
      // if there is no error -> create Cola2 Command and execute it
      if ( !ec )
      {
        sick::cola2::Cola2Session::CommandPtr command_ptr = std::make_shared<U>(*session, command);

        session->executeCommand(command_ptr, [session, handler](const boost::system::error_code& ec) {
          AsyncCompleteHandler nextHandler = handler;

          // if there is an error -> we call the handler and reset it because we always want to close the session
          // but we cannot call the given handler multiple times.
          if (ec)
          {
            nextHandler( ec );
            nextHandler = AsyncCompleteHandler();
          }

          session->close([session, nextHandler](const boost::system::error_code& ec) {
            session->doDisconnect();  // we always want to disconnect the session/tcp socket

            // it the handler was not already called -> we call it with the given error_code
            if (nextHandler)
            {
              nextHandler( ec );
            }
          });
        });
      }
      // if there is an error -> call handler with error code
      else
      {
        handler(ec);
      }
    });
  }


  void requestNextFieldData(int index,
                            Cola2SessionPtr session,
                            std::shared_ptr<sick::datastructure::ConfigData> config_data,
                            std::reference_wrapper< std::vector<sick::datastructure::FieldData> > fields,
                            AsyncCompleteHandler handler )
  {

    std::shared_ptr< sick::datastructure::FieldData > field_data = std::make_shared<sick::datastructure::FieldData>() ;
    sick::cola2::Cola2Session::CommandPtr command_ptr = std::make_shared<sick::cola2::FieldHeaderVariableCommand>(*session,
                                                                                                                  *field_data,
                                                                                                                  index );

    session->executeCommand(command_ptr, [index, session, config_data, fields, field_data, handler](const boost::system::error_code& ec) {
      if ( !ec )
      {
        sick::cola2::Cola2Session::CommandPtr command2_ptr = std::make_shared<sick::cola2::FieldGeometryVariableCommand>(*session,
                                                                                                                         *field_data,
                                                                                                                          index );

        session->executeCommand(command2_ptr, [index, session, config_data, fields, field_data, handler](const boost::system::error_code& ec) {
          if ( !ec )
          {
            if (field_data->getIsValid())
            {
              field_data->setStartAngleDegrees(config_data->getDerivedStartAngle());
              field_data->setAngularBeamResolutionDegrees(config_data->getDerivedAngularBeamResolution());
              fields.get().push_back( *field_data );
            }

            if ( index < 128 )
            {
              requestNextFieldData( index+1, session, config_data, fields, handler );
            }
          }
          else
          {
            handler( ec );
          }
        });
      }
      else
      {
        handler( ec );
      }
    });
  }


  void requestFieldData(Cola2SessionPtr session,
                        std::reference_wrapper< std::vector<sick::datastructure::FieldData> > fields,
                        AsyncCompleteHandler handler)
  {
    std::shared_ptr<sick::datastructure::ConfigData> config_data = std::make_shared<sick::datastructure::ConfigData>();
    sick::cola2::Cola2Session::CommandPtr command_ptr = std::make_shared<sick::cola2::MeasurementCurrentConfigVariableCommand>(*session,
                                                                                                                               *config_data);

    session->executeCommand(command_ptr, [session, fields, config_data, handler](const boost::system::error_code& ec) {
      if ( !ec )
      {
        detail::requestNextFieldData( 0, session, config_data, fields, handler);
      }
      else
      {
        handler(ec);
      }
    });
  }


  void requestNextMonitoringCase( int index,
                                  Cola2SessionPtr session,
                                  std::reference_wrapper< std::vector<sick::datastructure::MonitoringCaseData> > monitoring_cases,
                                  AsyncCompleteHandler handler )
  {
    std::shared_ptr< sick::datastructure::MonitoringCaseData > monitoring_case_data = std::make_shared<sick::datastructure::MonitoringCaseData>() ;
    sick::cola2::Cola2Session::CommandPtr command_ptr = std::make_shared<sick::cola2::MonitoringCaseVariableCommand>(*session,
                                                                                                                     *monitoring_case_data,
                                                                                                                     index );

    session->executeCommand(command_ptr, [session, index, monitoring_cases, monitoring_case_data, handler](const boost::system::error_code& ec) {
      if ( !ec )
      {
        if (monitoring_case_data->getIsValid())
        {
          monitoring_cases.get().push_back( *monitoring_case_data );
        }

        if ( index < 254 )
        {
          requestNextMonitoringCase( index+1, session, monitoring_cases, handler );
        }
      }
      else
      {
        handler( ec );
      }
    });
  }
}




SickSafetyscannersBase::SickSafetyscannersBase(
  const packetReceivedCallbackFunction& newPacketReceivedCallbackFunction)
  : m_newPacketReceivedCallbackFunction(newPacketReceivedCallbackFunction)
  , m_io_service_ptr ( std::make_shared<boost::asio::io_service>() )
  , m_io_service ( *m_io_service_ptr )
{
  ROS_INFO("Starting SickSafetyscannersBase");

  m_async_udp_client_ptr = std::make_shared<sick::communication::AsyncUDPClient>(
    boost::bind(&SickSafetyscannersBase::processUDPPacket, this, _1),
    m_io_service);

  m_packet_merger_ptr = std::make_shared<sick::data_processing::UDPPacketMerger>();

  ROS_INFO("Started SickSafetyscannersBase");
}


SickSafetyscannersBase::SickSafetyscannersBase(
    boost::asio::io_service& io_service,
    const packetReceivedCallbackFunction& newPacketReceivedCallbackFunction)
  : m_newPacketReceivedCallbackFunction(newPacketReceivedCallbackFunction)
  , m_io_service( io_service )
{
  ROS_INFO("Starting SickSafetyscannersBase");

  m_async_udp_client_ptr = std::make_shared<sick::communication::AsyncUDPClient>(
    boost::bind(&SickSafetyscannersBase::processUDPPacket, this, _1),
    m_io_service);

  m_packet_merger_ptr = std::make_shared<sick::data_processing::UDPPacketMerger>();

  ROS_INFO("Started SickSafetyscannersBase");
}

SickSafetyscannersBase::~SickSafetyscannersBase()
{
  stop();
}

boost::system::error_code SickSafetyscannersBase::start(const sick::datastructure::CommSettings& settings)
{
  // if we have an internal io_service -> create a thread and run the io_service
  if ( m_io_service_ptr )
  {
    m_udp_client_thread_ptr.reset(
      new boost::thread(boost::bind(&SickSafetyscannersBase::udpClientThread, this)));
  }

  const boost::system::error_code ec = m_async_udp_client_ptr->open( settings.getHostUdpPort() );

  if ( !ec )
  {
    m_async_udp_client_ptr->start();
  }

  return ec;
}

void SickSafetyscannersBase::stop()
{
  m_async_udp_client_ptr->stop();

  if ( m_io_service_ptr )
  {
    m_io_service_ptr->stop();
    m_udp_client_thread_ptr->join();
    m_udp_client_thread_ptr.reset();
  }
}

SickSafetyscannersBase::endpoint_type SickSafetyscannersBase::getLocalUdpEndpoint() const
{
  return m_async_udp_client_ptr->getLocalEndpoint();
}

bool SickSafetyscannersBase::udpClientThread()
{
  ROS_INFO("Enter io thread");

  // reset the io_service so that it can be used again
  m_io_service.get().reset();

  std::shared_ptr<boost::asio::io_service::work> io_work_ptr = std::make_shared<boost::asio::io_service::work>( m_io_service );
  m_io_service.get().run();

  ROS_INFO("Exit io thread");

  return true;
}

void SickSafetyscannersBase::changeSensorSettings(const datastructure::CommSettings& settings)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncChangeSensorSettings(settings, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncChangeSensorSettings(const sick::datastructure::CommSettings& settings,
                                                       AsyncCompleteHandler handler )
{
  // we must save the settings in a shared object, because we must gurantee that the value stays valid until the given handler is called
  std::shared_ptr< sick::datastructure::CommSettings > shared_settings = std::make_shared< sick::datastructure::CommSettings >( settings );

  detail::processCola2Command<sick::cola2::ChangeCommSettingsCommand>(m_io_service, settings,
                                                                      std::ref(*shared_settings),
                                                                      [shared_settings, handler](const boost::system::error_code& ec) {
                                                                         handler( ec );
                                                                      });
}

void SickSafetyscannersBase::findSensor(const datastructure::CommSettings& settings,
                                        uint16_t blink_time)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncFindSensor(settings, blink_time, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncFindSensor(const datastructure::CommSettings& settings,
                                             uint16_t blink_time,
                                             AsyncCompleteHandler handler)
{
  // we must save the blink_time in a shared object, because we must gurantee that the value stays valid until the given handler is called
  std::shared_ptr< uint16_t > shared_blink_time = std::make_shared< uint16_t >( blink_time );

  detail::processCola2Command<sick::cola2::FindMeCommand>(m_io_service, settings,
                                                          std::ref(*shared_blink_time),
                                                          [shared_blink_time, handler](const boost::system::error_code& ec) {
                                                             handler( ec );
                                                          });
}

void SickSafetyscannersBase::requestTypeCode(const datastructure::CommSettings& settings,
                                             sick::datastructure::TypeCode& type_code)
{ 
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestTypeCode(settings, type_code, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Type Code: %s", type_code.getTypeCode().c_str());
}

void SickSafetyscannersBase::asyncRequestTypeCode(const sick::datastructure::CommSettings& settings,
                                                  sick::datastructure::TypeCode& type_code,
                                                  AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::TypeCodeVariableCommand>( m_io_service, settings, std::ref(type_code), handler );
}

void SickSafetyscannersBase::requestApplicationName(const datastructure::CommSettings& settings,
                                                    sick::datastructure::ApplicationName& application_name)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestApplicationName(settings, application_name, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Application name: %s", application_name.getApplicationName().c_str());
}

void SickSafetyscannersBase::asyncRequestApplicationName(const sick::datastructure::CommSettings& settings,
                                                         sick::datastructure::ApplicationName& application_name,
                                                         AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::ApplicationNameVariableCommand>( m_io_service, settings, std::ref(application_name), handler );
}

void SickSafetyscannersBase::requestSerialNumber(const datastructure::CommSettings& settings,
                                                 datastructure::SerialNumber& serial_number)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestSerialNumber(settings, serial_number, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Serial Number: %s", serial_number.getSerialNumber().c_str());
}

void SickSafetyscannersBase::asyncRequestSerialNumber(const sick::datastructure::CommSettings& settings,
                                                      sick::datastructure::SerialNumber& serial_number,
                                                      AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::SerialNumberVariableCommand>( m_io_service, settings, std::ref(serial_number), handler );
}

void SickSafetyscannersBase::requestFirmwareVersion(const datastructure::CommSettings& settings,
                                                    datastructure::FirmwareVersion& firmware_version)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestFirmwareVersion(settings, firmware_version, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Firmware Version: %s", firmware_version.getFirmwareVersion().c_str());
}

void SickSafetyscannersBase::asyncRequestFirmwareVersion(const sick::datastructure::CommSettings& settings,
                                                         sick::datastructure::FirmwareVersion& firmware_version,
                                                         AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::FirmwareVersionVariableCommand>( m_io_service, settings, std::ref(firmware_version), handler );
}

void SickSafetyscannersBase::requestOrderNumber(const datastructure::CommSettings& settings,
                                                datastructure::OrderNumber& order_number)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestOrderNumber(settings, order_number, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Order Number: %s", order_number.getOrderNumber().c_str());
}

void SickSafetyscannersBase::asyncRequestOrderNumber(const datastructure::CommSettings& settings,
                                                     datastructure::OrderNumber& order_number,
                                                     AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::OrderNumberVariableCommand>( m_io_service, settings, std::ref(order_number), handler );
}

void SickSafetyscannersBase::requestProjectName(const datastructure::CommSettings& settings,
                                                datastructure::ProjectName& project_name)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestProjectName(settings, project_name, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Project Name: %s", project_name.getProjectName().c_str());
}

void SickSafetyscannersBase::asyncRequestProjectName(const datastructure::CommSettings& settings,
                                                     datastructure::ProjectName& project_name,
                                                     AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::ProjectNameVariableCommand>( m_io_service, settings, std::ref(project_name), handler );
}

void SickSafetyscannersBase::requestUserName(const datastructure::CommSettings& settings,
                                             datastructure::UserName& user_name)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestUserName(settings, user_name, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("User Name: %s", user_name.getUserName().c_str());
}

void SickSafetyscannersBase::asyncRequestUserName(const datastructure::CommSettings& settings,
                                                  datastructure::UserName& user_name,
                                                  AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::UserNameVariableCommand>( m_io_service, settings, std::ref(user_name), handler );
}

void SickSafetyscannersBase::requestConfigMetadata(const datastructure::CommSettings& settings,
                                                   sick::datastructure::ConfigMetadata& config_metadata)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestConfigMetadata(settings, config_metadata, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncRequestConfigMetadata(const datastructure::CommSettings& settings,
                                                        datastructure::ConfigMetadata& config_metadata,
                                                        AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::ConfigMetadataVariableCommand>( m_io_service, settings, std::ref(config_metadata), handler );
}

void SickSafetyscannersBase::requestStatusOverview(const datastructure::CommSettings& settings,
                                                   sick::datastructure::StatusOverview& status_overview)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestStatusOverview(settings, status_overview, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncRequestStatusOverview(const datastructure::CommSettings& settings,
                                                        datastructure::StatusOverview& status_overview,
                                                        AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::StatusOverviewVariableCommand>( m_io_service, settings, std::ref(status_overview), handler );
}

void SickSafetyscannersBase::requestDeviceStatus(const datastructure::CommSettings& settings,
                                                 sick::datastructure::DeviceStatus& device_status)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestDeviceStatus(settings, device_status, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncRequestDeviceStatus(const datastructure::CommSettings& settings,
                                                      datastructure::DeviceStatus& device_status,
                                                      AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::DeviceStatusVariableCommand>( m_io_service, settings, std::ref(device_status), handler );
}

void SickSafetyscannersBase::requestRequiredUserAction(const datastructure::CommSettings& settings,
                                                       sick::datastructure::RequiredUserAction& required_user_action)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestRequiredUserAction(settings, required_user_action, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncRequestRequiredUserAction(const datastructure::CommSettings& settings,
                                                            datastructure::RequiredUserAction& required_user_action,
                                                            AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::RequiredUserActionVariableCommand>( m_io_service, settings, std::ref(required_user_action), handler );
}

void SickSafetyscannersBase::requestDeviceName(const datastructure::CommSettings& settings,
                                               datastructure::DeviceName& device_name)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestDeviceName(settings, device_name, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();

  ROS_INFO("Device name: %s", device_name.getDeviceName().c_str());
}

void SickSafetyscannersBase::asyncRequestDeviceName(const sick::datastructure::CommSettings& settings,
                                                    datastructure::DeviceName& device_name,
                                                    AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::DeviceNameVariableCommand>( m_io_service, settings, std::ref(device_name), handler );
}

void SickSafetyscannersBase::requestPersistentConfig(const datastructure::CommSettings& settings,
                                                     sick::datastructure::ConfigData& config_data)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestPersistentConfig(settings, config_data, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncRequestPersistentConfig(const datastructure::CommSettings& settings,
                                                          sick::datastructure::ConfigData& config_data,
                                                          AsyncCompleteHandler handler)
{
  detail::processCola2Command<sick::cola2::MeasurementPersistentConfigVariableCommand>( m_io_service, settings, std::ref(config_data), handler );
}





void SickSafetyscannersBase::requestFieldData(const datastructure::CommSettings& settings,
                                              std::vector<sick::datastructure::FieldData>& field_data)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestFieldData(settings, field_data, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}

void SickSafetyscannersBase::asyncRequestFieldData(const sick::datastructure::CommSettings& settings,
                                                   std::vector<sick::datastructure::FieldData>& field_data,
                                                   AsyncCompleteHandler handler)
{
  auto fd = std::ref(field_data);

  detail::openCola2Session(m_io_service, settings, [fd, handler](const boost::system::error_code& ec, detail::Cola2SessionPtr session) {
    if ( ec )
    {
      handler( ec );
      return;
    }

    detail::requestFieldData( session, fd, [session, handler](const boost::system::error_code& ec) {
      AsyncCompleteHandler nextHandler = handler;
      if ( ec )
      {
        nextHandler( ec );
        nextHandler = AsyncCompleteHandler();
      }

      session->close( [session, nextHandler](const boost::system::error_code& ec) {
        session->doDisconnect();

        if ( nextHandler )
        {
          nextHandler( ec );
        }
      });
    });
  });
}


void SickSafetyscannersBase::requestMonitoringCases(const datastructure::CommSettings& settings,
                                                    std::vector<sick::datastructure::MonitoringCaseData>& monitoring_cases)
{
  CompletePromisePtr promise = std::make_shared<CompletePromise>();

  asyncRequestMonitoringCases(settings, monitoring_cases, [promise](const boost::system::error_code& ec) {
    promise->set_value(ec);
  });

  promise->get_future().wait();
}


void SickSafetyscannersBase::asyncRequestMonitoringCases(const sick::datastructure::CommSettings& settings,
                                                         std::vector<sick::datastructure::MonitoringCaseData>& monitoring_cases,
                                                         AsyncCompleteHandler handler)
{
  auto mc = std::ref(monitoring_cases);

  detail::openCola2Session(m_io_service, settings, [mc, handler](const boost::system::error_code& ec, detail::Cola2SessionPtr session) {
    if ( ec )
    {
      handler( ec );
      return;
    }

    detail::requestNextMonitoringCase( 0, session, mc, [session, handler](const boost::system::error_code& ec) {
      AsyncCompleteHandler nextHandler = handler;
      if ( ec )
      {
        nextHandler( ec );
        nextHandler = AsyncCompleteHandler();
      }

      session->close( [session, nextHandler](const boost::system::error_code& ec) {
        session->doDisconnect();

        if ( nextHandler )
        {
          nextHandler( ec );
        }
      });
    });
  });
}

void SickSafetyscannersBase::processUDPPacket(const sick::datastructure::PacketBuffer& buffer)
{
  if (m_packet_merger_ptr->addUDPPacket(buffer))
  {
    sick::datastructure::PacketBuffer deployed_buffer =
      m_packet_merger_ptr->getDeployedPacketBuffer();

    sick::datastructure::Data data = sick::data_processing::ParseData::parseUDPSequence(deployed_buffer);

    m_newPacketReceivedCallbackFunction(data);
  }
}

} // namespace sick
