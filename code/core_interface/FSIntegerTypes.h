#pragma once

#include <cstdint>

typedef std::int32_t _fs_time_t;  // time_t here is 64-bit and we need 32-bit
typedef std::int32_t fix;

// PTR compatible sizes
typedef ptrdiff_t ptr_s;
typedef size_t ptr_u;

typedef std::int64_t longlong;
typedef std::uint64_t ulonglong;
typedef std::uint8_t ubyte;
typedef std::uint16_t ushort;
typedef std::uint32_t uint;
typedef unsigned long ulong;


