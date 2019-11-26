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
 * \file ParseIntrusionData.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_safetyscanners_base/data_processing/ParseIntrusionData.h>

namespace sick {
namespace data_processing {

ParseIntrusionData::ParseIntrusionData() {}

datastructure::IntrusionData
ParseIntrusionData::parseUDPSequence(const datastructure::PacketBuffer& buffer,
                                     datastructure::Data& data)
{
  datastructure::IntrusionData intrusion_data;
  if (!checkIfPreconditionsAreMet(data))
  {
    intrusion_data.setIsEmpty(true);
    return intrusion_data;
  }

  // Keep our own copy of the shared_ptr to keep the iterators valid
  const std::shared_ptr<std::vector<uint8_t> const> vec_ptr = buffer.getBuffer();
  std::vector<uint8_t>::const_iterator data_ptr =
    vec_ptr->begin() + data.getDataHeaderPtr()->getIntrusionDataBlockOffset();

  const uint16_t num_scan_points = data.getDerivedValuesPtr()->getNumberOfBeams();

  setDataInIntrusionData(num_scan_points, data_ptr, intrusion_data);
  return intrusion_data;
}

bool ParseIntrusionData::checkIfPreconditionsAreMet(const datastructure::Data& data)
{
  if (!checkIfIntrusionDataIsPublished(data))
  {
    return false;
  }
  if (!checkIfDataContainsNeededParsedBlocks(data))
  {
    return false;
  }
  return true;
}

bool ParseIntrusionData::checkIfIntrusionDataIsPublished(const datastructure::Data& data)
{
  return !(data.getDataHeaderPtr()->getIntrusionDataBlockOffset() == 0 &&
           data.getDataHeaderPtr()->getIntrusionDataBlockSize() == 0);
}

bool ParseIntrusionData::checkIfDataContainsNeededParsedBlocks(
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

void ParseIntrusionData::setDataInIntrusionData(const uint16_t num_scan_points,
                                                std::vector<uint8_t>::const_iterator data_ptr,
                                                datastructure::IntrusionData& intrusion_data)
{
  std::vector<sick::datastructure::IntrusionDatum> intrusion_datums;
  setDataInIntrusionDatums(num_scan_points, data_ptr, intrusion_datums);
  intrusion_data.setIntrusionDataVector(intrusion_datums);
}

void ParseIntrusionData::setDataInIntrusionDatums(
  uint16_t num_scan_points,
  std::vector<uint8_t>::const_iterator data_ptr,
  std::vector<sick::datastructure::IntrusionDatum>& intrusion_datums)
{
  uint16_t offset = 0;
  // Repeats for 24 CutOffPaths
  for (uint8_t i_set = 0; i_set < 24; ++i_set)
  {
    sick::datastructure::IntrusionDatum datum;
    setSizeInIntrusionDatum(offset, data_ptr, datum);
    offset += 4;
    setFlagsInIntrusionDatum(offset, num_scan_points, data_ptr, datum);
    offset += datum.getSize();
    intrusion_datums.push_back(datum);
  }
}


uint16_t
ParseIntrusionData::setSizeInIntrusionDatum(const uint16_t& offset,
                                            std::vector<uint8_t>::const_iterator data_ptr,
                                            sick::datastructure::IntrusionDatum& datum)
{
  uint32_t num_bytes_to_read = read_write_helper::readUint32LittleEndian(data_ptr + offset);
  datum.setSize(num_bytes_to_read);
  return offset;
}

uint16_t
ParseIntrusionData::setFlagsInIntrusionDatum(const uint16_t offset,
                                             const uint16_t num_scan_points,
                                             std::vector<uint8_t>::const_iterator data_ptr,
                                             sick::datastructure::IntrusionDatum& datum)
{
  uint32_t num_read_flags = 0;
  std::vector<bool> flags;
  for (uint16_t num_read_bytes = 0;
       (num_read_bytes < datum.getSize()) && (num_read_flags < num_scan_points);
       num_read_bytes++)
  {
    uint8_t bitset = read_write_helper::readUint8LittleEndian(data_ptr + offset + num_read_bytes);
    for (uint32_t i_bit = 0; (i_bit < 8) && (num_read_flags < num_scan_points);
         i_bit++, num_read_flags++)
    {
      flags.push_back(static_cast<bool>(bitset & (0x01 << i_bit)));
    }
  }
  datum.setFlagsVector(flags);
  return offset;
}

} // namespace data_processing
} // namespace sick
