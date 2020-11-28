#pragma once
#include "FSMathTypes.h"
#include "asteroid/SCPAsteroidInfo.h"
#include "parse/SCPTable.h"
#include "parse/SCPTableFormatDescriptor.h"
namespace SCP
{
	class AsteroidTable : public SCPTableBase<AsteroidTable>
	{
	public:
		SCPTableProperty<SCP_vector<AsteroidGroup>> AsteroidGroups;
		SCPTableProperty<SCP_string> ImpactExplosionAnimation;
		SCPTableProperty<float> ImpactExplosionRadius;
		SCPTableProperty<SCP_string> BriefingCloseupModel; // defaults to large asteroid's first POF file
		SCPTableProperty<vec3d> BriefingCloseupPosition; // defaults to  0.0f, 0.0f, -334.0f
		SCPTableProperty<float> BriefingCloseupZoom; // default value 0.5f

		// GetAsteroidInfo(index AsteroidIndex) // small med large 0 1 2
		// GetDebrisInfo(SpeciesIndex) //returns n, n+1, n+2 where N is (species index +1) * 3
		// GetBriefingModel()
	};

} // namespace SCP

template<>
inline tl::optional<SCP::AsteroidTable> construct(const SCPParsedTableData& InData)
{
	SCP::AsteroidTable Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
inline tl::optional<SCP::AsteroidInfo> construct(const SCPParsedTableData& InData)
{
	if (InData.name == "AsteroidData")
	{
		SCP::AsteroidInfo AsteroidTableEntry;
		for (auto Node : InData.nodes)
		{
			AsteroidTableEntry.Deserialize(Node->name, *Node);
		}
		return AsteroidTableEntry;
	}
	return tl::optional<SCP::AsteroidInfo>();
}

// TODO: Refactor the definition of TableBase into namespace SCP to make it easier to define these overloads in that
// namespace too
void DeserializeChildren(SCP::AsteroidTable* ClassInstance, const SCPParsedTableData& InData)
{
	for (auto Node : InData.nodes)
	{
		ClassInstance->Deserialize(Node->name, *Node);
	}
}

void DeserializeAsteroidGroup(SCP::AsteroidTable* ClassInstance, const SCPParsedTableData& InData)
{
	if (InData.nodes.size() == 3)
	{
		SCP::AsteroidGroup NewGroup;
		NewGroup.SmallAsteroid.ReplaceValue(construct<SCP::AsteroidInfo>(*(InData.nodes[0])));
		NewGroup.MediumAsteroid.ReplaceValue(construct<SCP::AsteroidInfo>(*(InData.nodes[1])));
		NewGroup.LargeAsteroid.ReplaceValue(construct<SCP::AsteroidInfo>(*(InData.nodes[2])));

		if (ClassInstance->AsteroidGroups.IsSet())
		{
			SCP_vector<SCP::AsteroidGroup>& GroupList = ClassInstance->AsteroidGroups;
			GroupList.push_back(NewGroup);
		}
		else
		{
			ClassInstance->AsteroidGroups = SCP_vector<SCP::AsteroidGroup>({NewGroup});
		}
	}
}
// clang-format off
	const DeserializationHandlers<SCP::AsteroidTable> SCPTableBase<SCP::AsteroidTable>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetSectionName("Asteroid Types"), DeserializeChildren},
		{"AsteroidDataGroup", DeserializeAsteroidGroup},
		{SCPTableFormatDescriptor::GetVariableName("$Impact Explosion"), ReplaceValue(&SCP::AsteroidTable::ImpactExplosionAnimation)},
		{SCPTableFormatDescriptor::GetVariableName("$Impact Explosion Radius"), ReplaceValue(&SCP::AsteroidTable::ImpactExplosionRadius)},
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Icon Closeup Model"), ReplaceValue(&SCP::AsteroidTable::BriefingCloseupModel)},
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Icon Closeup Position"), ReplaceValue(&SCP::AsteroidTable::BriefingCloseupPosition)},
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Icon Closeup Zoom"), ReplaceValue(&SCP::AsteroidTable::BriefingCloseupZoom)}
	};
// clang-format on