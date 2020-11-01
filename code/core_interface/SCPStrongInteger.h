#pragma once

namespace SCP
{
	template<typename UnderlyingIntegerType>
	struct StrongInteger
	{
		UnderlyingIntegerType Value = 0;
		// Force explicit use of the constructor when we need a StrongInteger (ie this means you must construct a
		// StrongInteger to pass into a function)
		explicit StrongInteger(UnderlyingIntegerType Value) : Value(Value) {};

		// Allow implicit conversion to the underlying type
		constexpr operator UnderlyingIntegerType() const
		{
			return Value;
		}

		constexpr StrongInteger& operator+=(const StrongInteger Other)
		{
			Value += Other.Value;
			return *this;
		}

		constexpr StrongInteger& operator-=(const StrongInteger Other)
		{
			Value -= Other.Value;
			return *this;
		}

		constexpr StrongInteger operator+(const StrongInteger Other) const
		{
			return StrongInteger(Value + Other.Value);
		}

		constexpr StrongInteger operator-(const StrongInteger Other) const
		{
			return StrongInteger(Value - Other.Value);
		}

		constexpr bool operator>(const StrongInteger Other) const
		{
			return Value > Other.Value;
		}

		constexpr bool operator<(const StrongInteger Other) const
		{
			return Value < Other.Value;
		}
		constexpr bool operator>=(const StrongInteger Other) const
		{
			return Value >= Other.Value;
		}

		constexpr bool operator<=(const StrongInteger Other) const
		{
			return Value <= Other.Value;
		}
	};
}