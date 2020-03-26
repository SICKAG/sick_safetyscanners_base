#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <memory>
#include "sick_safetyscanners_base/datastructure/Data.h"
#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"
#include "sick_safetyscanners_base/generics.h"

namespace sick
{
namespace types
{

template <typename T>
using CallbackT = std::function<void(T)>;

using ScanDataCb = std::function<void(const sick::datastructure::Data &)>;
using PacketHandler = std::function<void(const sick::datastructure::PacketBuffer &)>;
// using ScanDataCb = std::function<void(const sick::datastructure::Data&)>;


} // namespace types

enum class SensorFeatures : uint16_t
{
    NONE = 0,
    SYSTEM_STATE = 1,
    DERIVED_SETTINGS = 1 << 1,
    MEASUREMENT_DATA = 1 << 2,
    INTRUSION_DATA = 1 << 3,
    APPLICATION_DATA = 1 << 4
};
} // namespace sick

ENABLE_BITMASK_OPERATORS(sick::SensorFeatures);

#endif // ifndef SICK_SAFETYSCANNERS_TYPES_H