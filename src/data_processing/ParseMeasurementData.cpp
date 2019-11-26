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
 * \file ParseMeasurementData.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_safetyscanners_base/data_processing/ParseMeasurementData.h>

namespace sick {
namespace data_processing {

ParseMeasurementData::ParseMeasurementData() {}

datastructure::MeasurementData
ParseMeasurementData::parseUDPSequence(const datastructure::PacketBuffer& buffer,
                                       datastructure::Data& data)
{
  datastructure::MeasurementData measurement_data;
  if (!checkIfPreconditionsAreMet(data))
  {
    measurement_data.setIsEmpty(true);
    return measurement_data;
  }
  // Keep our own copy of the shared_ptr to keep the iterators valid
  const std::shared_ptr<std::vector<uint8_t> const> vec_ptr = buffer.getBuffer();
  std::vector<uint8_t>::const_iterator data_ptr =
    vec_ptr->begin() + data.getDataHeaderPtr()->getMeasurementDataBlockOffset();

  const float angle       = data.getDerivedValuesPtr()->getStartAngle();
  const float angle_delta = data.getDerivedValuesPtr()->getAngularBeamResolution();

  setDataInMeasurementData(angle, angle_delta, data_ptr, measurement_data);
  return measurement_data;
}

bool ParseMeasurementData::checkIfPreconditionsAreMet(const datastructure::Data& data)
{
  if (!checkIfMeasurementDataIsPublished(data))
  {
    return false;
  }
  if (!checkIfDataContainsNeededParsedBlocks(data))
  {
    return false;
  }
  return true;
}

bool ParseMeasurementData::checkIfMeasurementDataIsPublished(const datastructure::Data& data)
{
  return !(data.getDataHeaderPtr()->getMeasurementDataBlockOffset() == 0 &&
           data.getDataHeaderPtr()->getMeasurementDataBlockSize() == 0);
}

bool ParseMeasurementData::checkIfDataContainsNeededParsedBlocks(
  const datastructure::Data& data)
{
  if (data.getDataHeaderPtr()->isEmpty())
  {
    return false;
  }
  if (data.getDerivedValuesPtr()->isEmpty())
  {
    return false;
  }
  return true;
}

void ParseMeasurementData::setDataInMeasurementData(
  const float& angle,
  const float& angle_delta,
  std::vector<uint8_t>::const_iterator data_ptr, datastructure::MeasurementData& measurement_data)
{
  setNumberOfBeamsInMeasurementData(data_ptr, measurement_data);
  setScanPointsInMeasurementData(angle, angle_delta, data_ptr, measurement_data);
}

void ParseMeasurementData::setNumberOfBeamsInMeasurementData(
  std::vector<uint8_t>::const_iterator data_ptr,
  datastructure::MeasurementData& measurement_data)
{
  measurement_data.setNumberOfBeams(read_write_helper::readUint32LittleEndian(data_ptr + 0));
}

void ParseMeasurementData::setScanPointsInMeasurementData(
  const float& angle,
  const float& angle_delta,
  std::vector<uint8_t>::const_iterator data_ptr, datastructure::MeasurementData& measurement_data)
{
  float cur_angle = angle;
  for (size_t i = 0; i < measurement_data.getNumberOfBeams(); i++)
  {
    addScanPointToMeasurementData(i, cur_angle, data_ptr, measurement_data);
    cur_angle += angle_delta;
  }
}

void ParseMeasurementData::addScanPointToMeasurementData(
  const uint16_t offset,
  const float& angle,
  std::vector<uint8_t>::const_iterator data_ptr,
  datastructure::MeasurementData& measurement_data)
{
  uint16_t distance    = read_write_helper::readUint16LittleEndian(data_ptr + (4 + offset * 4));
  uint8_t reflectivity = read_write_helper::readUint8LittleEndian(data_ptr + (6 + offset * 4));
  uint8_t status       = read_write_helper::readUint8LittleEndian(data_ptr + (7 + offset * 4));
  bool valid           = static_cast<bool>(status & (0x01 << 0));
  bool infinite        = static_cast<bool>(status & (0x01 << 1));
  bool glare           = static_cast<bool>(status & (0x01 << 2));
  bool reflector       = static_cast<bool>(status & (0x01 << 3));
  bool contamination   = static_cast<bool>(status & (0x01 << 4));
  bool contamination_warning = static_cast<bool>(status & (0x01 << 5));

  measurement_data.addScanPoint(sick::datastructure::ScanPoint(angle,
                                                               distance,
                                                               reflectivity,
                                                               valid,
                                                               infinite,
                                                               glare,
                                                               reflector,
                                                               contamination,
                                                               contamination_warning));
}

} // namespace data_processing
} // namespace sick
