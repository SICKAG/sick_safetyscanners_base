#ifndef SICK_SAFETYSCANNERS_BASE_GENERICS_H
#define SICK_SAFETYSCANNERS_BASE_GENERICS_H

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

// Strongly Typed BitFlag operator overloads
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

template <typename T, typename... Args>
T createT(Args &&... ARGS)
{
    return T(std::forward<Args>(ARGS)...);
}

// C++11 does not come with a make_unique function, so here it is.
// Borrowed from Scott Meyers' Effective C++ page 139.
template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
} // namespace sick
#endif // SICK_SAFETYSCANNERS_BASE_GENERICS_H