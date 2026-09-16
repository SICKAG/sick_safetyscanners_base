// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
/*!
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!
 * \file ParseContaminationInfo.cpp
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/data_processing/ParseContaminationInfo.h"

#include "sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp"

namespace sick {
namespace data_processing {

namespace {

// Reply layout, see "CoLa2 Interface Description Fields and Monitoring Case
// Table Extension" (SICK document E213012), chapter "Contamination
// Measurement". Each sector block is 36 bytes: status (2), level (2) and 32
// reserved bytes.
constexpr size_t kSectorStride      = 36;
constexpr size_t kSectorStatusOfs   = 0;
constexpr size_t kSectorLevelOfs    = 2;
constexpr size_t kProcessingStateOfs = 4;

// Sector angles in degrees. The sectors overlap significantly in reality;
// these are the nominal boundaries given in the document.
constexpr float kMicroSectorAngles[16] = {-48.f, -4.f, 18.f,  34.f,  40.f,
                                          51.f,  62.f, 82.f,  98.f,  118.f,
                                          129.f, 140.f, 146.f, 162.f, 192.f,
                                          228.f};

constexpr float kNanoSectorAngles[18] = {
  -47.5f, -32.f, -25.f, -8.f,  7.f,   23.f,  38.f,  58.f,  76.f,
  104.f,  122.f, 142.f, 157.f, 173.f, 188.f, 205.f, 212.f, 227.5f};

struct Layout
{
  size_t sector_count;
  size_t first_sector_offset;
  const float* angles;
};

Layout layoutFor(datastructure::ContaminationVariant variant)
{
  if (variant == datastructure::ContaminationVariant::NANO_SCAN3)
  {
    return {17, 64, kNanoSectorAngles};
  }
  return {15, 56, kMicroSectorAngles};
}

} // namespace

bool ParseContaminationInfo::parseTCPSequence(
  const datastructure::PacketBuffer& buffer,
  datastructure::ContaminationVariant variant,
  datastructure::ContaminationInfo& contamination_info) const
{
  const Layout layout = layoutFor(variant);

  // Keep our own copy of the shared_ptr to keep the iterators valid
  const std::shared_ptr<std::vector<uint8_t> const> vec_ptr = buffer.getBuffer();

  const size_t required =
    layout.first_sector_offset + layout.sector_count * kSectorStride;
  if (vec_ptr->size() < required)
  {
    // The device does not support this variable, or answered with a layout we
    // do not know. Leave the output untouched rather than reading past the end.
    return false;
  }

  std::vector<uint8_t>::const_iterator data_ptr = vec_ptr->begin();

  contamination_info.setProcessingState(
    read_write_helper::readUint16LittleEndian(data_ptr + kProcessingStateOfs));

  std::vector<datastructure::ContaminationSector>& sectors =
    contamination_info.getSectors();
  sectors.clear();
  sectors.reserve(layout.sector_count);

  for (size_t i = 0; i < layout.sector_count; ++i)
  {
    const size_t base = layout.first_sector_offset + i * kSectorStride;

    const uint16_t status =
      read_write_helper::readUint16LittleEndian(data_ptr + base + kSectorStatusOfs);
    const int16_t raw_level =
      read_write_helper::readInt16LittleEndian(data_ptr + base + kSectorLevelOfs);

    datastructure::ContaminationSector sector;
    // r8_8Level is a fixed point number, divide by 256 to get percent.
    sector.level       = static_cast<float>(raw_level) / 256.f;
    sector.warning     = static_cast<bool>(status & (0x01 << 0));
    sector.error       = static_cast<bool>(status & (0x01 << 1));
    sector.angle_start = layout.angles[i];
    sector.angle_end   = layout.angles[i + 1];
    sectors.push_back(sector);
  }

  return true;
}

} // namespace data_processing
} // namespace sick
