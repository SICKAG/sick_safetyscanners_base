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
 * \file ContaminationInfo.h
 *
 * \brief Contamination measurement of the optics cover, as measured by the
 *        device's primary contamination measurement system.
 *
 * The contamination measurement uses laser diodes that measure orthogonally
 * to the main measurement beam through the optics cover. The individual diode
 * measurements are grouped into sectors; one level is reported per sector.
 *
 * 0% is the value measured during calibration in production (or after an
 * optics cover replacement). Per EN ISO 61496-3 the device raises a
 * contamination warning from 57.1% and a contamination error from 100%.
 *
 * Note that this information is read over CoLa2 and is therefore explicitly
 * non-safe: it may be used for diagnostics only, never as an input to a
 * safety function.
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATASTRUCTURE_CONTAMINATIONINFO_H
#define SICK_SAFETYSCANNERS_BASE_DATASTRUCTURE_CONTAMINATIONINFO_H

#include <cstdint>
#include <vector>

namespace sick {
namespace datastructure {

/*!
 * \brief Device variant, which selects both the CoLa2 variable index and the
 * layout of the reply (number of sectors and their offset).
 */
enum class ContaminationVariant
{
  MICRO_SCAN3,   /*!< variable index 371, 15 sectors */
  OUTDOOR_SCAN3, /*!< variable index 371, 15 sectors */
  NANO_SCAN3     /*!< variable index 373, 17 sectors */
};

/*!
 * \brief Contamination measurement of a single sector of the optics cover.
 */
struct ContaminationSector
{
  float level    = 0.f;   /*!< Contamination in percent, 0.0 - 100.0 */
  bool warning   = false; /*!< Pollution detected, warning level */
  bool error     = false; /*!< Pollution detected, error level */
  float angle_start = 0.f;  /*!< Sector start angle in degrees, sensor frame */
  float angle_end   = 0.f;  /*!< Sector end angle in degrees, sensor frame */
};

/*!
 * \brief Contamination measurement of the complete optics cover.
 */
class ContaminationInfo
{
public:
  ContaminationInfo() = default;

  /*!
   * \brief Processing state of the contamination measurement system.
   * \return 14 if the measurement is running, any other value otherwise.
   */
  uint16_t getProcessingState() const { return m_processing_state; }
  void setProcessingState(uint16_t processing_state)
  {
    m_processing_state = processing_state;
  }

  /*!
   * \brief Whether the reported sector data may be evaluated. The device only
   * produces valid sector data while the contamination measurement is running.
   */
  bool isValid() const { return m_processing_state == 14; }

  const std::vector<ContaminationSector>& getSectors() const { return m_sectors; }
  std::vector<ContaminationSector>& getSectors() { return m_sectors; }

private:
  uint16_t m_processing_state = 0;
  std::vector<ContaminationSector> m_sectors;
};

} // namespace datastructure
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATASTRUCTURE_CONTAMINATIONINFO_H
