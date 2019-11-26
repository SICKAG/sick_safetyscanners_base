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
 * \file FieldGeometryVariableCommand.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_safetyscanners_base/cola2/FieldGeometryVariableCommand.h>

#include <sick_safetyscanners_base/cola2/Cola2Session.h>
#include <sick_safetyscanners_base/cola2/Command.h>

namespace sick {
namespace cola2 {

FieldGeometryVariableCommand::FieldGeometryVariableCommand(Cola2Session& session,
                                                           datastructure::FieldData& field_data,
                                                           const uint16_t& index)
  : VariableCommand(session, 0x2810 + index)
  , m_field_data(field_data)
{
}

bool FieldGeometryVariableCommand::canBeExecutedWithoutSessionID() const
{
  return true;
}

bool FieldGeometryVariableCommand::processReply()
{
  if (!base_class::processReply())
  {
    return false;
  }

  return sick::data_processing::ParseFieldGeometryData::parseTCPSequence(getDataVector(), m_field_data);
}


} // namespace cola2
} // namespace sick
