#pragma once

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
} // namespace SCP