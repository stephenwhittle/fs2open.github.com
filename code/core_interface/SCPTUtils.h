#pragma once
#include<type_traits>
/// SCP Template utilities

template <typename Base, typename Child>
using SCPRestrictToChildren = typename std::enable_if<std::is_base_of<Base, Child>::value, int>::type*;