#include <functional>

namespace sick
{
namespace generics
{

template <typename Enum>
struct EnableBitMaskOperators
{
    static const bool enable = false;
};

#define ENABLE_BITMASK_OPERATORS(x)      \
    namespace sick                       \
    {                                    \
    namespace generics                   \
    {                                    \
    template <>                          \
    struct EnableBitMaskOperators<x>     \
    {                                    \
        static const bool enable = true; \
    };                                   \
    }                                    \
    }

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

} // namespace generics
} // namespace sick