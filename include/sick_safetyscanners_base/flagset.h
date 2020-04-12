#pragma once

#include <bitset>
#include <iostream>
#include <cassert>
#include <string>

namespace sick
{

template <typename T>
class flag_set
{
public:
    flag_set() = default;

    explicit flag_set(const T &val) { flags.set(static_cast<typename std::underlying_type<T>::type>(val)); }

    // Binary operations.

    flag_set &operator&=(const T &val) noexcept
    {
        bool tmp = flags.test(static_cast<typename std::underlying_type<T>::type>(val));
        flags.reset();
        flags.set(static_cast<typename std::underlying_type<T>::type>(val), tmp);
        return *this;
    }

    flag_set &operator&=(const flag_set &o) noexcept
    {
        flags &= o.flags;
        return *this;
    }

    flag_set &operator|=(const T &val) noexcept
    {
        flags.set(static_cast<typename std::underlying_type<T>::type>(val));
        return *this;
    }

    flag_set &operator|=(const flag_set &o) noexcept
    {
        flags |= o.flags;
        return *this;
    }

    // The resulting bitset can contain at most 1 bit.
    flag_set operator&(const T &val) const
    {
        flag_set ret(*this);
        ret &= val;

        assert(ret.flags.count() <= 1);
        return ret;
    }

    flag_set operator&(const flag_set &val) const
    {
        flag_set ret(*this);
        ret.flags &= val.flags;

        return ret;
    }

    // The resulting bitset contains at least 1 bit.
    flag_set operator|(const T &val) const
    {
        flag_set ret(*this);
        ret |= val;

        assert(ret.flags.count() >= 1);
        return ret;
    }

    flag_set operator|(const flag_set &val) const
    {
        flag_set ret(*this);
        ret.flags |= val.flags;

        return ret;
    }

    flag_set operator~() const
    {
        flag_set cp(*this);
        cp.flags.flip();

        return cp;
    }

    // The bitset evaluates to true if any bit is set.
    explicit operator bool() const { return flags.any(); }

    // Methods from std::bitset.

    bool operator==(const flag_set &o) const { return flags == o.flags; }

    std::size_t size() const { return flags.size(); }

    std::size_t count() const { return flags.count(); }

    flag_set &set()
    {
        flags.set();
        return *this;
    }

    flag_set &reset()
    {
        flags.reset();
        return *this;
    }

    flag_set &flip()
    {
        flags.flip();
        return *this;
    }

    flag_set &set(const T &val, bool value = true)
    {
        flags.set(static_cast<typename std::underlying_type<T>::type>(val), value);
        return *this;
    }

    flag_set &reset(const T &val)
    {
        flags.reset(static_cast<typename std::underlying_type<T>::type>(val));
        return *this;
    }

    flag_set &flip(const T &val)
    {
        flags.flip(static_cast<typename std::underlying_type<T>::type>(val));
        return *this;
    }

    constexpr bool operator[](const T &val) const { return flags[static_cast<typename std::underlying_type<T>::type>(val)]; }

    explicit operator typename std::underlying_type<T>::type() const
    {
        return static_cast<typename std::underlying_type<T>::type>(flags.to_ullong());
    }

    std::string to_string() const { return flags.to_string(); }

    // Operator for outputting to std::ostream.
    friend std::ostream &operator<<(std::ostream &stream, const flag_set &self)
    {
        return stream << self.flags;
    }

private:
    // _ is last value sentinel and must be present in enum T.
    std::bitset<static_cast<typename std::underlying_type<T>::type>(T::_)> flags;
};

template <typename T, typename = void>
struct is_enum_that_contains_sentinel : std::false_type
{
};

template <typename T>
struct is_enum_that_contains_sentinel<T, decltype(static_cast<void>(T::_))> : std::is_enum<T>
{
};

// Operator that combines two enumeration values into a flag_set only if the
// enumeration contains the sentinel `_`.
template <typename T>
typename std::enable_if<is_enum_that_contains_sentinel<T>::value, flag_set<T>>::type operator|(const T &lhs,
                                                                                               const T &rhs)
{
    flag_set<T> fs;
    fs |= lhs;
    fs |= rhs;

    return fs;
}
} // namespace sick