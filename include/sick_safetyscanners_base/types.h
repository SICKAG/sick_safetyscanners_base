#ifndef SICK_SAFETYSCANNERS_BASE_TYPES_H
#define SICK_SAFETYSCANNERS_BASE_TYPES_H

#include <functional>
#include <sick_safetyscanners_base/datastructure/Data.h>

namespace sick
{

// C++11 does not come with a make_unique function, so here it is.
// Borrowed from Scott Meyers' Effective C++ page 139.
template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
template <typename T>
using CallbackT = std::function<void(T)>;

using DataReceivedCb = CallbackT<const sick::datastructure::Data&>;

} // namespace sick
#endif // ifndef SICK_SAFETYSCANNERS_TYPES_H