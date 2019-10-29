// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2019, SICK AG, Waldkirch
*  Copyright (C) 2019, FZI Forschungszentrum Informatik, Karlsruhe, Germany
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
 * \file ParseSerialNumber.h
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2019_07:23
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSESERIALNUMBER_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSESERIALNUMBER_H

#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/datastructure/PacketBuffer.h>
#include <sick_safetyscanners_base/datastructure/SerialNumber.h>

#include <sick_safetyscanners_base/data_processing/ReadWriteHelper.hpp>

#include <string>

namespace sick {

namespace data_processing {


/*!
 * \brief Parser to read the serial number from a tcp sequence.
 */
class ParseSerialNumber
{
private:
  /*!
   * \brief Constructor of the parser.
   */
  ParseSerialNumber();

public:
  /*!
   * \brief Parses a tcp sequence to read the serial number of the sensor.
   *
   * \param buffer The incoming data sequence.
   * \param serial_number Reference to the parsed serial number.
   *
   * \returns If parsing the serial number was successful.
   */
  static bool parseTCPSequence(const datastructure::PacketBuffer& buffer,
                               datastructure::SerialNumber& serial_number);

  static std::string readSerialNumber(std::vector<uint8_t>::const_iterator data_ptr);
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSESERIALNUMBER_H
