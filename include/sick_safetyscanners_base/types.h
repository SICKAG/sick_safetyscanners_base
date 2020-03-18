#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <memory>
#include "sick_safetyscanners_base/datastructure/Data.h"
#include "sick_safetyscanners_base/generics.h"

namespace sick
{
namespace types
{

template <typename T>
using CallbackT = std::function<void(T)>;

using ScanDataCb = CallbackT<const sick::datastructure::Data &>;

enum class SensorFeatures : uint16_t
{
    NONE = 0,
    SYSTEM_STATE = 1 << 0,
    DERIVED_SETTINGS = 1 << 1,
    MEASUREMENT_DATA = 1 << 2,
    INTRUSION_DATA = 1 << 3,
    APPLICATION_DATA = 1 << 4
};

} // namespace types
} // namespace sick

ENABLE_BITMASK_OPERATORS(sick::types::SensorFeatures);

#endif // ifndef SICK_SAFETYSCANNERS_TYPES_H