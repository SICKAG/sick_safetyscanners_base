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
 * \file ContaminationInfoVariableCommand.h
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_COLA2_CONTAMINATIONINFOVARIABLECOMMAND_H
#define SICK_SAFETYSCANNERS_BASE_COLA2_CONTAMINATIONINFOVARIABLECOMMAND_H

#include "sick_safetyscanners_base/cola2/VariableCommand.h"
#include "sick_safetyscanners_base/data_processing/ParseContaminationInfo.h"
#include "sick_safetyscanners_base/datastructure/ContaminationInfo.h"

namespace sick {
namespace cola2 {

/*!
 * \brief Reads the contamination diagnostics variable of the device.
 *
 * The variable index depends on the device variant: 373
 * (ContaminationDetection_tNanoDiagnostics) for nanoScan3 and 371
 * (ContaminationDetection_tMicroDiagnostics) for microScan3 and outdoorScan3.
 */
class ContaminationInfoVariableCommand : public VariableCommand
{
public:
  typedef sick::cola2::VariableCommand base_class;

  /*!
   * \brief Constructor.
   *
   * \param session The current cola2 session.
   * \param variant Device variant, selects variable index and reply layout.
   * \param contamination_info Reference to the object the reply is parsed into.
   */
  ContaminationInfoVariableCommand(
    Cola2Session& session,
    datastructure::ContaminationVariant variant,
    datastructure::ContaminationInfo& contamination_info);

  bool canBeExecutedWithoutSessionID() const;

  bool processReply();

  /*!
   * \brief Returns the CoLa2 variable index used for a device variant.
   */
  static uint16_t variableIndexFor(datastructure::ContaminationVariant variant);

private:
  std::shared_ptr<sick::data_processing::ParseContaminationInfo> m_parser_ptr;
  datastructure::ContaminationVariant m_variant;
  datastructure::ContaminationInfo& m_contamination_info;
};

/*!
 * \brief Reads the per beam contamination variable (index 282) of the device.
 *
 * Only available from firmware 1.7x (microScan3, outdoorScan3) and 1.66
 * (nanoScan3).
 */
class ContaminationScanVariableCommand : public VariableCommand
{
public:
  typedef sick::cola2::VariableCommand base_class;

  /*!
   * \brief Constructor.
   *
   * \param session The current cola2 session.
   * \param contamination_scan Reference to the object the reply is parsed into.
   */
  ContaminationScanVariableCommand(Cola2Session& session,
                                   datastructure::ContaminationScan& contamination_scan);

  bool canBeExecutedWithoutSessionID() const;

  bool processReply();

private:
  std::shared_ptr<sick::data_processing::ParseContaminationScan> m_parser_ptr;
  datastructure::ContaminationScan& m_contamination_scan;
};

} // namespace cola2
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_COLA2_CONTAMINATIONINFOVARIABLECOMMAND_H
