#ifndef SICK_SAFETYSCANNERS_BASE_GENERICS_H
#define SICK_SAFETYSCANNERS_BASE_GENERICS_H

#include <memory>

namespace sick
{

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