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
 * \file enumflags.h
 *
 * \author  Martin Schulze <schulze@fzi.de>
 * \date    2020-04-15
 */
//----------------------------------------------------------------------

#ifndef SICK_SAFETYSCANNERS_BASE_ENUMFLAGS_H
#define SICK_SAFETYSCANNERS_BASE_ENUMFLAGS_H

#include <type_traits>
#include <functional>
#include <memory>

#define ENABLE_BITMASK_OPERATORS(x)      \
    namespace sick                       \
    {                                    \
    template <>                          \
    struct is_bitmask_enum<x>            \
    {                                    \
        static const bool enable = true; \
    };                                   \
    } // namespace sick

namespace sick
{

// Strongly typed BitFlag operator overloads
template <typename Enum>
struct is_bitmask_enum
{
    static const bool enable = false;
};

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
operator|(Enum const &lhs, Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_t>(lhs) |
        static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
operator&(Enum const &lhs, Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_t>(lhs) &
        static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
operator^(Enum const &lhs, Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_t>(lhs) ^
        static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
operator~(Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        ~static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
    &
    operator|=(Enum &lhs, Enum rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum>(
        static_cast<underlying_t>(lhs) |
        static_cast<underlying_t>(rhs));
    return lhs;
}

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
    &
    operator&=(Enum &lhs, Enum rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum>(
        static_cast<underlying_t>(lhs) &
        static_cast<underlying_t>(rhs));
    return lhs;
}

template <typename Enum>
constexpr typename std::enable_if<is_bitmask_enum<Enum>::enable, Enum>::type
    &
    operator^=(Enum &lhs, Enum rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum>(
        static_cast<underlying_t>(lhs) ^
        static_cast<underlying_t>(rhs));
    return lhs;
}

} // namespace sick
#endif // SICK_SAFETYSCANNERS_BASE_ENUMFLAGS_H