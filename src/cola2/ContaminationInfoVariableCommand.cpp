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
 * \file ContaminationInfoVariableCommand.cpp
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/cola2/ContaminationInfoVariableCommand.h"

#include "sick_safetyscanners_base/cola2/Cola2Session.h"
#include "sick_safetyscanners_base/cola2/Command.h"

namespace sick {
namespace cola2 {

uint16_t ContaminationInfoVariableCommand::variableIndexFor(
  datastructure::ContaminationVariant variant)
{
  return variant == datastructure::ContaminationVariant::NANO_SCAN3 ? 373 : 371;
}

ContaminationInfoVariableCommand::ContaminationInfoVariableCommand(
  Cola2Session& session,
  datastructure::ContaminationVariant variant,
  datastructure::ContaminationInfo& contamination_info)
  : VariableCommand(session, variableIndexFor(variant))
  , m_variant(variant)
  , m_contamination_info(contamination_info)
{
  m_parser_ptr = std::make_shared<sick::data_processing::ParseContaminationInfo>();
}

bool ContaminationInfoVariableCommand::canBeExecutedWithoutSessionID() const
{
  return true;
}

bool ContaminationInfoVariableCommand::processReply()
{
  if (!base_class::processReply())
  {
    return false;
  }
  return m_parser_ptr->parseTCPSequence(getDataVector(), m_variant, m_contamination_info);
}

} // namespace cola2
} // namespace sick
