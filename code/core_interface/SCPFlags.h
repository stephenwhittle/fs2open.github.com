#pragma once

#include "FSIntegerTypes.h"
#include "tl/optional.hpp"

#include <initializer_list>

//def_list
struct flag_def_list {
	const char *name;
	int def;
	ubyte var;
};

template<class T>
struct flag_def_list_new {
    const char* name;			// The parseable representation of this flag
    T def;				// The flag definition for this flag
    bool in_use;		// Whether or not this flag is currently in use or obsolete
    bool is_special;	// Whether this flag requires special processing. See parse_string_flag_list<T, T> for details
};

template<typename T>
class SCPFlags
{
	using EnumType = T;
	using StorageType = std::underlying_type_t<T>;
	tl::optional<StorageType> Value;
	
	constexpr SCPFlags<T>() {};

	
	
	constexpr static StorageType Convert(const EnumType& EnumValue) { return static_cast<StorageType>(EnumValue); }

public:
	constexpr StorageType RawValue() { return Value.disjunction(0).value(); }
	constexpr SCPFlags<T> operator|(const EnumType& EnumValue) const { return SCPFlags<T>(Value.disjunction(0).value() | Convert(EnumValue)); }
	constexpr SCPFlags<T> operator|=(const EnumType& EnumValue) { Value = Value.disjunction(0).value() | Convert(EnumValue); return *this; }
	constexpr SCPFlags<T>& SetFlag(const EnumType& EnumValue) { Value = Value.disjunction(0).value() | Convert(EnumValue); return *this; }
	
	constexpr bool operator&(const EnumType& EnumValue) const { return (Value.disjunction(0).value() & Convert(EnumValue)) == Convert(EnumValue); }
	constexpr bool HasFlag(const EnumType& EnumValue) const { return (Value.disjunction(0).value() & Convert(EnumValue)) == Convert(EnumValue); }
	
	constexpr bool operator^(const EnumType& EnumValue) const { return SCPFlags<T>(Value.disjunction(0).value() ^ Convert(EnumValue)); }
	constexpr bool operator^=(const EnumType& EnumValue) { Value = Value.disjunction(0).value() ^ Convert(EnumValue); return *this; }
	constexpr bool ToggleFlag(const EnumType& EnumValue) { Value = Value.disjunction(0).value() ^ Convert(EnumValue); return *this; }

	constexpr SCPFlags<T> operator~() const { return SCPFlags<T>(~Value.disjunction(0).value()); }
	constexpr bool ClearFlag(EnumType EnumValue) { Value = Value.disjunction(0).value() & ~Convert(EnumValue); return *this; }

	//constexpr StorageType GetRawValue() { return Value; }
	
	constexpr bool operator==(const EnumType& Other) const { return Value.has_value() && *Value == Convert(Other); }
	constexpr bool operator==(const SCPFlags<T>& Other) const
	{ 
		return (Value.has_value() && Other.Value.has_value() && *Value == *Other.Value) ||
			(!Value.has_value() && !Other.Value.has_value()); 
	}

	constexpr bool operator!=(const EnumType& Other) const { return !(*this == Other); }
	constexpr bool operator!=(const SCPFlags<T>& Other) const { return !(*this == Other); }

	constexpr bool Is(std::initializer_list<EnumType> Values)
	{
		return *this == SCPFlags<T>(Values);
	}

	constexpr SCPFlags<T>(std::initializer_list<EnumType> Values)
	{
		for (EnumType Flag : Values)
		{
			SetFlag(Flag);
		}
	}

	constexpr SCPFlags<T>(EnumType InitialValue) 
		: Value(Convert(InitialValue)) 
	{
	}
	
	constexpr SCPFlags<T>(StorageType InitialValue) : Value(InitialValue){};

	constexpr static const SCPFlags<T> Empty() { return SCPFlags<T>(); }
};