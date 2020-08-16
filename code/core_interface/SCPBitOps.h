#pragma once
#include <limits>

template<typename T>
constexpr T MaskLowBits(T InValue, unsigned long long NumberOfBitsToMask)
{
	if (NumberOfBitsToMask == 0)
	{
		return InValue;
	}
	else
	{
		return MaskLowBits<T>(((InValue << 1) | 0b01), NumberOfBitsToMask - 1);
	}
}

constexpr inline std::uint32_t operator"" _LowBitsMask32(unsigned long long NumBits)
{
	return MaskLowBits<std::uint32_t>(0, NumBits);
}

template <typename T>
constexpr T MaskHighBits(T InValue, unsigned long long NumberOfBitsToMask)
{
	if (NumberOfBitsToMask == 0) {
		return InValue;
	}
	else {
		return MaskHighBits<T>((InValue >> 1) | (1 << (std::numeric_limits<T>::digits - 1)), NumberOfBitsToMask - 1);
	}
}

constexpr inline std::uint32_t operator"" _HighBitsMask32(unsigned long long NumBits)
{
	return MaskHighBits<std::uint32_t>(0, NumBits);
}