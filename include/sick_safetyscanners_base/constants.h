
#include <sick_safetyscanners_base/types.h>

namespace sick
{
using sick::types::SensorFeatures;
constexpr SensorFeatures all_sensor_features = SensorFeatures::SystemState | SensorFeatures::DerivededSettings | SensorFeatures::ApplicationData | SensorFeatures::IntursionData | SensorFeatures::MeasurementData;

constexpr float angle_factor = 4194304.0;

} // namespace sick