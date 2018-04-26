#ifndef ZUSE_POINTERS_H
#define ZUSE_POINTERS_H

#include <type_traits>



namespace zuse
{



namespace util
{



///
/// \brief Pointer with ownership
///
/// We should use std::unique_ptr in most cases.
/// This template is only for compatibility.
///
template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
using Owner = T;

///
/// \brief Pointer without ownership
///
template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
using Observer = T;



} // namespace util



} // namespace zuse



#endif // ZUSE_POINTERS_H
