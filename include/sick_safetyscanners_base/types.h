#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <memory>
#include <sick_safetyscanners_base/datastructure/Data.h>
#include <sick_safetyscanners_base/generics.h>

namespace sick
{
namespace types
{

// Strongly Typed BitFlag operator overloads

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

// Callback types
using DataReceivedCb = CallbackT<const sick::datastructure::Data &>;


enum class SensorFeatures
{
    None = 0,
    SystemState = 1 << 0,
    DerivededSettings = 1 << 1,
    MeasurementData = 1 << 2,
    IntursionData = 1 << 3,
    ApplicationData = 1 << 4

};


} // namespace types
} // namespace sick

ENABLE_BITMASK_OPERATORS(SensorFeatures);

#endif // ifndef SICK_SAFETYSCANNERS_TYPES_H