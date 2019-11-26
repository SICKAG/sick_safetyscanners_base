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
 * \file ParseData.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_safetyscanners_base/data_processing/ParseData.h>

namespace sick {
namespace data_processing {

ParseData::ParseData()
{
}

sick::datastructure::Data
ParseData::parseUDPSequence(const datastructure::PacketBuffer& buffer)
{
  sick::datastructure::Data data;
  setDataBlocksInData(buffer, data);
  return data;
}

bool ParseData::parseTCPSequence(const datastructure::PacketBuffer& buffer,
                                 sick::datastructure::Data& data)
{
  setDataBlocksInData(buffer, data);
  return true;
}

void ParseData::setDataBlocksInData(const datastructure::PacketBuffer& buffer,
                                    datastructure::Data& data)
{
  setDataHeaderInData(buffer, data);
  setDerivedValuesInData(buffer, data);
  setMeasurementDataInData(buffer, data);
  setGeneralSystemStateInData(buffer, data);
  setIntrusionDataInData(buffer, data);
  setApplicationDataInData(buffer, data);
}

void ParseData::setDataHeaderInData(const datastructure::PacketBuffer& buffer,
                                    datastructure::Data& data)
{
  sick::datastructure::DataHeader data_header =
    sick::data_processing::ParseDataHeader::parseUDPSequence(buffer, data);
  data.setDataHeaderPtr(std::make_shared<sick::datastructure::DataHeader>(data_header));
}

void ParseData::setDerivedValuesInData(const datastructure::PacketBuffer& buffer,
                                       datastructure::Data& data)
{
  sick::datastructure::DerivedValues derived_values =
    sick::data_processing::ParseDerivedValues::parseUDPSequence(buffer, data);
  data.setDerivedValuesPtr(std::make_shared<sick::datastructure::DerivedValues>(derived_values));
}

void ParseData::setMeasurementDataInData(const datastructure::PacketBuffer& buffer,
                                         datastructure::Data& data)
{
  sick::datastructure::MeasurementData measurement_data =
    sick::data_processing::ParseMeasurementData::parseUDPSequence(buffer, data);
  data.setMeasurementDataPtr(
    std::make_shared<sick::datastructure::MeasurementData>(measurement_data));
}

void ParseData::setGeneralSystemStateInData(const datastructure::PacketBuffer& buffer,
                                            datastructure::Data& data)
{
  sick::datastructure::GeneralSystemState general_system_state =
    sick::data_processing::ParseGeneralSystemState::parseUDPSequence(buffer, data);
  data.setGeneralSystemStatePtr(
    std::make_shared<sick::datastructure::GeneralSystemState>(general_system_state));
}

void ParseData::setIntrusionDataInData(const datastructure::PacketBuffer& buffer,
                                       datastructure::Data& data)
{
  sick::datastructure::IntrusionData intrusion_data =
    sick::data_processing::ParseIntrusionData::parseUDPSequence(buffer, data);
  data.setIntrusionDataPtr(std::make_shared<sick::datastructure::IntrusionData>(intrusion_data));
}

void ParseData::setApplicationDataInData(const datastructure::PacketBuffer& buffer,
                                         datastructure::Data& data)
{
  sick::datastructure::ApplicationData application_data =
    sick::data_processing::ParseApplicationData::parseUDPSequence(buffer, data);

  data.setApplicationDataPtr(
    std::make_shared<sick::datastructure::ApplicationData>(application_data));
}

} // namespace data_processing
} // namespace sick
