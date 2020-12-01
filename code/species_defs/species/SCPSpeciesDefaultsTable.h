#pragma once
#include "parse/SCPTable.h"

namespace SCP
{

	class ThrusterAnimInfo : SCPTableBase<SCP::ThrusterAnimInfo>
	{
		public:
		SCPTableProperty<SCP_string> PrimaryNormal;
		SCPTableProperty<SCP_string> PrimaryAfterburn;
		SCPTableProperty<SCP_string> SecondaryNormal;
		SCPTableProperty<SCP_string> SecondaryAfterburn;
		SCPTableProperty<SCP_string> TertiaryNormal;
		SCPTableProperty<SCP_string> TertiaryAfterburn;
		SCPTableProperty<SCP_string> DistortionNormal;
		SCPTableProperty<SCP_string> DistortionAfterburn;
	};

	class GlowBitmapInfo : SCPTableBase<SCP::GlowBitmapInfo>
	{
	public:
		SCPTableProperty<SCP_string> Normal;
		SCPTableProperty<SCP_string> Afterburn;
	};
	class SpeciesDefaults : SCPTableBase<SCP::SpeciesDefaults>
	{
	public:
		SCPTableProperty<SCP_string> Name;
		SCPTableProperty<SCP_string> DefaultIFF;
		SCPTableProperty<color> FredColor;
		SCPTableProperty<SCP_string> DebrisTexture;
		SCPTableProperty<SCP_string> ShieldHitAni;
		SCPTableProperty<ThrusterAnimInfo> ThrusterAnims;
		SCPTableProperty<GlowBitmapInfo> GlowBitmaps;
		SCPTableProperty<float> AwacsMultiplier;
		SCPTableProperty<SCP_string> CountermeasureType;
	};

	// define a lambda that binds the extra variable for wether nocreate is set or not
	// also another function returning a lambda that checks to see if the parent node has a certain name, and dispatches
	// differently based on that

	class SpeciesDefaultsTable : SCPTableBase<SCP::SpeciesDefaultsTable>
	{
	public:
		SCPTableProperty<SCP_vector<SCP::SpeciesDefaults>> Entries;
	};
} // namespace SCP