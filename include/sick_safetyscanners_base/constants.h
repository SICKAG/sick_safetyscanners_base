#ifndef SICK_SAFETYSCANNERS_BASE_CONSTANTS_H
#define SICK_SAFETYSCANNERS_BASE_CONSTANTS_H

#include "sick_safetyscanners_base/types.h"

namespace sick
{
using sick::SensorFeatures;
const SensorFeatures all_sensor_features = SensorFeatures::SYSTEM_STATE | SensorFeatures::DERIVED_SETTINGS | SensorFeatures::APPLICATION_DATA | SensorFeatures::INTRUSION_DATA | SensorFeatures::MEASUREMENT_DATA;
constexpr float angle_factor = 4194304.0;

} // namespace sick
#endif // SICK_SAFETYSCANNERS_BASE_CONSTANTS_H