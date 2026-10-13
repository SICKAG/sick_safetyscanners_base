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
 * \file ParseContaminationInfo.h
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSECONTAMINATIONINFO_H
#define SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSECONTAMINATIONINFO_H

#include "sick_safetyscanners_base/datastructure/ContaminationInfo.h"
#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"

namespace sick {
namespace data_processing {

/*!
 * \brief Parser for the contamination diagnostics CoLa2 variable.
 */
class ParseContaminationInfo
{
public:
  ParseContaminationInfo() = default;

  /*!
   * \brief Parses a contamination diagnostics reply.
   *
   * \param buffer The reply payload.
   * \param variant Device variant, selects the reply layout.
   * \param contamination_info Output.
   * \return Whether the buffer was long enough to be parsed.
   */
  bool parseTCPSequence(const datastructure::PacketBuffer& buffer,
                        datastructure::ContaminationVariant variant,
                        datastructure::ContaminationInfo& contamination_info) const;
};

/*!
 * \brief Parser for the per beam contamination CoLa2 variable.
 */
class ParseContaminationScan
{
public:
  ParseContaminationScan() = default;

  /*!
   * \brief Parses a per beam contamination reply.
   *
   * \param buffer The reply payload.
   * \param contamination_scan Output.
   * \return Whether the buffer held a complete, valid reply.
   */
  bool parseTCPSequence(const datastructure::PacketBuffer& buffer,
                        datastructure::ContaminationScan& contamination_scan) const;
};

} // namespace data_processing
} // namespace sick

#endif // SICK_SAFETYSCANNERS_BASE_DATA_PROCESSING_PARSECONTAMINATIONINFO_H
