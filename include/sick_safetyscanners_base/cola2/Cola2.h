// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2020, SICK AG, Waldkirch
*  Copyright (C) 2020, FZI Forschungszentrum Informatik, Karlsruhe, Germany
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
 * \file Cola2.h
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#include "sick_safetyscanners_base/cola2/Cola2Session.h"

#include "sick_safetyscanners_base/cola2/ApplicationNameVariableCommand.h"
#include "sick_safetyscanners_base/cola2/ChangeCommSettingsCommand.h"
#include "sick_safetyscanners_base/cola2/CloseSession.h"
#include "sick_safetyscanners_base/cola2/Command.h"
#include "sick_safetyscanners_base/cola2/ConfigMetadataVariableCommand.h"
#include "sick_safetyscanners_base/cola2/CreateSession.h"
#include "sick_safetyscanners_base/cola2/DeviceNameVariableCommand.h"
#include "sick_safetyscanners_base/cola2/DeviceStatusVariableCommand.h"
#include "sick_safetyscanners_base/cola2/FieldGeometryVariableCommand.h"
#include "sick_safetyscanners_base/cola2/FieldHeaderVariableCommand.h"
#include "sick_safetyscanners_base/cola2/FieldSetsVariableCommand.h"
#include "sick_safetyscanners_base/cola2/FindMeCommand.h"
#include "sick_safetyscanners_base/cola2/FirmwareVersionVariableCommand.h"
#include "sick_safetyscanners_base/cola2/LatestTelegramVariableCommand.h"
#include "sick_safetyscanners_base/cola2/MeasurementCurrentConfigVariableCommand.h"
#include "sick_safetyscanners_base/cola2/MeasurementPersistentConfigVariableCommand.h"
#include "sick_safetyscanners_base/cola2/MethodCommand.h"
#include "sick_safetyscanners_base/cola2/MonitoringCaseTableHeaderVariableCommand.h"
#include "sick_safetyscanners_base/cola2/MonitoringCaseVariableCommand.h"
#include "sick_safetyscanners_base/cola2/OrderNumberVariableCommand.h"
#include "sick_safetyscanners_base/cola2/ProjectNameVariableCommand.h"
#include "sick_safetyscanners_base/cola2/RequiredUserActionVariableCommand.h"
#include "sick_safetyscanners_base/cola2/SerialNumberVariableCommand.h"
#include "sick_safetyscanners_base/cola2/StatusOverviewVariableCommand.h"
#include "sick_safetyscanners_base/cola2/TypeCodeVariableCommand.h"
#include "sick_safetyscanners_base/cola2/UserNameVariableCommand.h"
#include "sick_safetyscanners_base/cola2/VariableCommand.h"
