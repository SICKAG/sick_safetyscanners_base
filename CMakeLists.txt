cmake_minimum_required(VERSION 2.8.12) 
project(sick_safetyscanners_base CXX)

## Use C++11
add_definitions(-std=c++11)
## By adding -Wall and -Werror, the compiler does not ignore warnings anymore,
## enforcing cleaner code.
#add_definitions(-std=c++11 -Wall -Werror)


## Find system libraries
#find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)





###########
## Build ##
###########
option(WITHOUT_ROS_LOGGING "Disable ROS Logging and use printf instead" ON)

if(WITHOUT_ROS_LOGGING)
  add_definitions(-DNO_ROS_LOGGING=1)
endif()

## Declare a cpp library
add_library(sick_safetyscanners_base
  src/SickSafetyScannersBase.cpp
  src/cola2/ApplicationNameVariableCommand.cpp
  src/cola2/ChangeCommSettingsCommand.cpp
  src/cola2/CloseSession.cpp
  src/cola2/Cola2Session.cpp
  src/cola2/Command.cpp
  src/cola2/ConfigMetadataVariableCommand.cpp
  src/cola2/CreateSession.cpp
  src/cola2/DeviceNameVariableCommand.cpp
  src/cola2/DeviceStatusVariableCommand.cpp
  src/cola2/FieldGeometryVariableCommand.cpp
  src/cola2/FieldHeaderVariableCommand.cpp
  src/cola2/FieldSetsVariableCommand.cpp
  src/cola2/FindMeCommand.cpp
  src/cola2/FirmwareVersionVariableCommand.cpp
  src/cola2/MeasurementCurrentConfigVariableCommand.cpp
  src/cola2/MeasurementPersistentConfigVariableCommand.cpp
  src/cola2/MethodCommand.cpp
  src/cola2/MonitoringCaseTableHeaderVariableCommand.cpp
  src/cola2/MonitoringCaseVariableCommand.cpp
  src/cola2/OrderNumberVariableCommand.cpp
  src/cola2/ProjectNameVariableCommand.cpp
  src/cola2/RequiredUserActionVariableCommand.cpp
  src/cola2/SerialNumberVariableCommand.cpp
  src/cola2/StatusOverviewVariableCommand.cpp
  src/cola2/TypeCodeVariableCommand.cpp
  src/cola2/UserNameVariableCommand.cpp
  src/cola2/VariableCommand.cpp
  src/communication/AsyncTCPClient.cpp
  src/communication/AsyncUDPClient.cpp
  src/data_processing/ParseApplicationData.cpp
  src/data_processing/ParseApplicationNameData.cpp
  src/data_processing/ParseConfigMetadata.cpp
  src/data_processing/ParseData.cpp
  src/data_processing/ParseDataHeader.cpp
  src/data_processing/ParseDatagramHeader.cpp
  src/data_processing/ParseDerivedValues.cpp
  src/data_processing/ParseDeviceName.cpp
  src/data_processing/ParseDeviceStatus.cpp
  src/data_processing/ParseFieldGeometryData.cpp
  src/data_processing/ParseFieldHeaderData.cpp
  src/data_processing/ParseFieldSetsData.cpp
  src/data_processing/ParseFirmwareVersion.cpp
  src/data_processing/ParseGeneralSystemState.cpp
  src/data_processing/ParseIntrusionData.cpp
  src/data_processing/ParseMeasurementCurrentConfigData.cpp
  src/data_processing/ParseMeasurementData.cpp
  src/data_processing/ParseMeasurementPersistentConfigData.cpp
  src/data_processing/ParseMonitoringCaseData.cpp
  src/data_processing/ParseOrderNumber.cpp
  src/data_processing/ParseProjectName.cpp
  src/data_processing/ParseRequiredUserAction.cpp
  src/data_processing/ParseSerialNumber.cpp
  src/data_processing/ParseStatusOverview.cpp
  src/data_processing/ParseTCPPacket.cpp
  src/data_processing/ParseTypeCodeData.cpp
  src/data_processing/ParseUserNameData.cpp
  src/data_processing/TCPPacketMerger.cpp
  src/data_processing/UDPPacketMerger.cpp
  src/datastructure/ApplicationData.cpp
  src/datastructure/ApplicationInputs.cpp
  src/datastructure/ApplicationName.cpp
  src/datastructure/ApplicationOutputs.cpp
  src/datastructure/CommSettings.cpp
  src/datastructure/ConfigData.cpp
  src/datastructure/ConfigMetadata.cpp
  src/datastructure/Data.cpp
  src/datastructure/DatagramHeader.cpp
  src/datastructure/DataHeader.cpp
  src/datastructure/DerivedValues.cpp
  src/datastructure/DeviceName.cpp
  src/datastructure/DeviceStatus.cpp
  src/datastructure/FieldData.cpp
  src/datastructure/FieldSets.cpp
  src/datastructure/FirmwareVersion.cpp
  src/datastructure/GeneralSystemState.cpp
  src/datastructure/IntrusionData.cpp
  src/datastructure/IntrusionDatum.cpp
  src/datastructure/LatestTelegram.cpp
  src/datastructure/MeasurementData.cpp
  src/datastructure/MonitoringCaseData.cpp
  src/datastructure/OrderNumber.cpp
  src/datastructure/PacketBuffer.cpp
  src/datastructure/ParsedPacketBuffer.cpp
  src/datastructure/ProjectName.cpp
  src/datastructure/RequiredUserAction.cpp
  src/datastructure/ScanPoint.cpp
  src/datastructure/SerialNumber.cpp
  src/datastructure/StatusOverview.cpp
  src/datastructure/TypeCode.cpp
  src/datastructure/UserName.cpp
)

target_compile_options(sick_safetyscanners_base
  PUBLIC ${CXX11_FLAG}
  PRIVATE -Wall)

option(WITHOUT_ROS_LOGGING "Disable ROS Logging and use printf instead" ON)

if(WITHOUT_ROS_LOGGING)
  target_compile_definitions(sick_safetyscanners_base
    PRIVATE NO_ROS_LOGGING=1)
endif()

target_include_directories(sick_safetyscanners_base PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${Boost_INCLUDE_DIR}
  )

target_link_libraries(sick_safetyscanners_base PUBLIC ${Boost_LIBRARIES}
)

add_library(sick_safetyscanners_base::sick_safetyscanners_base ALIAS sick_safetyscanners_base)

#############
## Install ##
#############

# Mark executables and/or libraries for installation

install(
  TARGETS sick_safetyscanners_base EXPORT sick_safetyscanners_base_targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  EXPORT sick_safetyscanners_base_targets
  DESTINATION lib/cmake/sick_safetyscanners_base
  FILE sick_safetyscanners_baseTargets.cmake
  NAMESPACE sick_safetyscanners_base::
  )

# Mark cpp header files for installation
install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING 
  PATTERN "*.h"
  PATTERN "*.hpp"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(sick_safetyscanners_baseConfigVersion.cmake VERSION 1.0.0
  COMPATIBILITY SameMajorVersion)

install(FILES sick_safetyscanners_baseConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/sick_safetyscanners_baseConfigVersion.cmake
  DESTINATION lib/cmake/sick_safetyscanners_base)


##############################################
## Exporting from the build tree
##############################################

#export(EXPORT sick_safetyscanners_base_targets FILE
  #${CMAKE_CURRENT_BINARY_DIR}/sick_safetyscanners_baseTargets.cmake NAMESPACE sick_safetyscanners_base::)

##Register package in user's package registry
#export(PACKAGE sick_safetyscanners_base)


