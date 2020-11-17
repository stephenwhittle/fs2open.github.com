#pragma once
#include "parse/SCPTable.h"
#include "parse/SCPTableFormatDescriptor.h"
#include "FSStdTypes.h"

namespace SCP
{
	class AsteroidSplitRule : public SCPTableBase<AsteroidSplitRule>
	{
		SCPTableProperty<std::uint8_t> AsteroidIndex;
		SCPTableProperty<std::uint32_t> Min;
		SCPTableProperty<std::uint32_t> Max;
	};
	

	class AsteroidInfo : public SCPTableBase<AsteroidInfo>
	{
	public:
		SCPTableProperty<SCP_string> Name;
		SCPTableProperty<SCP_string> POFFile1;
		SCPTableProperty<SCP_string> POFFile2;
		SCPTableProperty<SCP_string> POFFile3;
		SCPTableProperty<SCP_vector<std::uint32_t>> DetailDistance;
		SCPTableProperty<float> MaxSpeed;
		SCPTableProperty<SCP_string> DamageType;
		SCPTableProperty<SCP_vector<std::uint32_t>> ExplosionAnimations;
		SCPTableProperty<float> ExplosionRadiusMultiplier;
		SCPTableProperty<float> ExplosionInnerRadius;
		SCPTableProperty<float> ExplosionOuterRadius;
		SCPTableProperty<float> ExplosionDamage;
		SCPTableProperty<float> ExplosionBlastIntensity;
		SCPTableProperty<float> Hitpoints;
		SCPTableProperty<SCP_vector<AsteroidSplitRule>> SplitRules;
	};

	class AsteroidGroup : public SCPTableBase<AsteroidGroup>
	{
	public:
		SCPTableProperty<AsteroidInfo> SmallAsteroid;
		SCPTableProperty<AsteroidInfo> MediumAsteroid;
		SCPTableProperty<AsteroidInfo> LargeAsteroid;
	};
} // namespace SCP

const DeserializationHandlers<SCP::AsteroidGroup> SCPTableBase<SCP::AsteroidGroup>::Deserializers = {};

// clang-format off
	const DeserializationHandlers<SCP::AsteroidInfo> SCPTableBase<SCP::AsteroidInfo>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Name"), ReplaceValue(&SCP::AsteroidInfo::Name)},
		{SCPTableFormatDescriptor::GetVariableName("$POF file1"), ReplaceValue(&SCP::AsteroidInfo::POFFile1)},
		{SCPTableFormatDescriptor::GetVariableName("$POF file2"), ReplaceValue(&SCP::AsteroidInfo::POFFile2)},
		{SCPTableFormatDescriptor::GetVariableName("$POF file3"), ReplaceValue(&SCP::AsteroidInfo::POFFile3)}
	};
// clang-format on