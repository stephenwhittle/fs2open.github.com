#pragma once
#include "parse/SCPTable.h"
#include "asteroid/SCPAsteroidInfo.h
#include "FSMathTypes.h"
namespace SCP
{
	class AsteroidTable : public SCPTableBase<AsteroidTable>
	{
	public:
		SCPTableProperty<SCP_vector<AsteroidInfo>> Asteroids;
		SCPTableProperty<SCP_string> ImpactExplosionAnimation;
		SCPTableProperty<float> ImpactExplosionRadius;
		SCPTableProperty<SCP_string> BriefingCloseupModel; // defaults to large asteroid's first POF file
		SCPTableProperty<vec3d> BriefingCloseupPosition; // defaults to  0.0f, 0.0f, -334.0f
		SCPTableProperty<float> BriefingCloseupZoom; // default value 0.5f

		//GetAsteroidInfo(index AsteroidIndex) // small med large 0 1 2
		//GetDebrisInfo(SpeciesIndex) //returns n, n+1, n+2 where N is (species index +1) * 3
		//GetBriefingModel()
	};
	// clang-format off
	const DeserializationHandlers<AsteroidTable> SCPTableBase<AsteroidTable>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetSectionName("Asteroid Types"), DeserializeChildren},
		{SCPTableFormatDescriptor::GetVariableName("$Impact Explosion"), ReplaceValue(&AsteroidTable::ImpactExplosionAnimation)},
		{SCPTableFormatDescriptor::GetVariableName("$Impact Explosion Radius"), ReplaceValue(&AsteroidTable::ImpactExplosionRadius)},
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Icon Closeup Model"), ReplaceValue(&AsteroidTable::BriefingCloseupModel)}
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Icon Closeup Position"), ReplaceValue(&AsteroidTable::BriefingCloseupPosition)}
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Icon Closeup Zoom"), ReplaceValue(&AsteroidTable::BriefingCloseupZoom)}
	};

	template<>
	inline tl::optional<AsteroidInfo> construct(const SCPParsedTableData& InData)
	{
		if (InData.name == "AsteroidData")
		{
			AsteroidInfo AsteroidTableEntry;
			for (auto Node : InData.nodes)
			{
				AsteroidTableEntry.Deserialize(Node->name, *Node);
			}
			return AsteroidTableEntry;
		}
		return tl::optional<AsteroidInfo>();
	}


	// clang-format on
} // namespace SCP