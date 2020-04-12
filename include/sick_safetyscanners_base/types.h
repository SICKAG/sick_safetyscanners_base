#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <memory>
#include "sick_safetyscanners_base/datastructure/Data.h"
#include "sick_safetyscanners_base/datastructure/PacketBuffer.h"
#include "sick_safetyscanners_base/generics.h"
// #include "flagset.h"

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

namespace SensorDataChannels
{
constexpr uint16_t ALL = 0b11111;
constexpr uint16_t NONE = 0;
constexpr uint16_t GENERAL_SYSTEM_STATE = 1 << 0;
constexpr uint16_t DERIVED_SETTINGS = 1 << 1;
constexpr uint16_t MEASUREMENT_DATA = 1 << 2;
constexpr uint16_t INTRUSION_DATA = 1 << 3;
constexpr uint16_t APPLICATION_DATA = 1 << 4;

constexpr bool isFlagSet(uint16_t channels, uint16_t flag)
{
    return (channels & flag) > 0;
};

constexpr uint16_t toFeatureFlags(
    bool general_system_state,
    bool derived_settings,
    bool measurement_data,
    bool intursion_data,
    bool application_data)
{
    // In C++11 constexpr functions are restricted to a function-body that contains a single return-statement only.
    // bool * int gets promoted to either 1 * int or 0 * int, so this is works to avoid if-statements.
    return (general_system_state * sick::SensorDataChannels::GENERAL_SYSTEM_STATE) +
    (derived_settings * sick::SensorDataChannels::DERIVED_SETTINGS) +
    (measurement_data * sick::SensorDataChannels::MEASUREMENT_DATA) +
    (intursion_data * sick::SensorDataChannels::INTRUSION_DATA) +
    (application_data * sick::SensorDataChannels::APPLICATION_DATA);
}

} // namespace SensorDataChannels

// enum class SensorDataChannels : uint16_t
// {
//     SYSTEM_STATE,
//     DERIVED_SETTINGS,
//     MEASUREMENT_DATA,
//     INTRUSION_DATA,
//     APPLICATION_DATA,
//     _ // Sentinel to restrict the range of possible domain values
// };

// using SensorFeatures = flag_set<SensorDataChannels>;
using SensorFeatures = uint16_t;

} // namespace sick

// ENABLE_BITMASK_OPERATORS(sick::SensorFeatures);

#endif // ifndef SICK_SAFETYSCANNERS_TYPES_H