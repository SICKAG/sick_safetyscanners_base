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