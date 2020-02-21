#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <memory>
#include <sick_safetyscanners_base/datastructure/Data.h>

namespace sick
{
namespace types
{


// Strongly Typed BitFlag operator overloads
template <typename Enum>
struct EnableBitMaskOperators
{
    static const bool enable = false;
};

template <typename Enum>
constexpr typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
operator|(Enum const &lhs, Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_t>(lhs) |
        static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
operator&(Enum const &lhs, Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_t>(lhs) &
        static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
operator^(Enum const &lhs, Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        static_cast<underlying_t>(lhs) ^
        static_cast<underlying_t>(rhs));
}

template <typename Enum>
constexpr typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
operator~(Enum const &rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    return static_cast<Enum>(
        ~static_cast<underlying_t>(rhs));
}

template <typename Enum>
typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
    &operator|=(Enum &lhs, Enum rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum>(
        static_cast<underlying_t>(lhs) |
        static_cast<underlying_t>(rhs));
    return lhs;
}

template <typename Enum>
typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
    &operator&=(Enum &lhs, Enum rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum>(
        static_cast<underlying_t>(lhs) &
        static_cast<underlying_t>(rhs));
    return lhs;
}

template <typename Enum>
typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
    &operator^=(Enum &lhs, Enum rhs)
{
    using underlying_t = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum>(
        static_cast<underlying_t>(lhs) ^
        static_cast<underlying_t>(rhs));
    return lhs;
}

#define ENABLE_BITMASK_OPERATORS(x)        \
    template <>                            \
    struct EnableBitMaskOperators<x> \
    {                                      \
        static const bool enable = true;   \
    };

// C++11 does not come with a make_unique function, so here it is.
// Borrowed from Scott Meyers' Effective C++ page 139.
template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

template <typename T>
using CallbackT = std::function<void(T)>;

// Specific type-instantiations 
enum class SensorFeatures 
{
  None = 0,
  SystemState = 1 << 0,
  DerivededSettings = 1 << 1,
  MeasurementData = 1 << 2,
  IntursionData = 1 << 3,
  ApplicationData = 1 << 4

};
ENABLE_BITMASK_OPERATORS(SensorFeatures);


// Callback types
using DataReceivedCb = CallbackT<const sick::datastructure::Data &>;

} // namespace types
} // namespace sick

#endif // ifndef SICK_SAFETYSCANNERS_TYPES_H