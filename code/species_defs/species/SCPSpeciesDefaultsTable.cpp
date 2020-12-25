
#include "SCPSpeciesDefaultsTable.h"
#include "parse/SCPTableFormatDescriptor.h"

const DeserializationHandlers<SCP::ThrusterAnimInfo> SCPTableBase<SCP::ThrusterAnimInfo>::Deserializers = {
	{SCPTableFormatDescriptor::GetVariableName("+Pri_Normal"), ReplaceValue(&SCP::ThrusterAnimInfo::PrimaryNormal)},
	{SCPTableFormatDescriptor::GetVariableName("+Normal"), ReplaceValue(&SCP::ThrusterAnimInfo::PrimaryNormal)},
	{SCPTableFormatDescriptor::GetVariableName("+Pri_Afterburn"),
	 ReplaceValue(&SCP::ThrusterAnimInfo::PrimaryAfterburn)},
	{SCPTableFormatDescriptor::GetVariableName("+Afterburn"), ReplaceValue(&SCP::ThrusterAnimInfo::PrimaryAfterburn)},
	{SCPTableFormatDescriptor::GetVariableName("+Sec_Normal"), ReplaceValue(&SCP::ThrusterAnimInfo::SecondaryNormal)},
	{SCPTableFormatDescriptor::GetVariableName("+Sec_Afterburn"),
	 ReplaceValue(&SCP::ThrusterAnimInfo::SecondaryAfterburn)},
	{SCPTableFormatDescriptor::GetVariableName("+Ter_Normal"), ReplaceValue(&SCP::ThrusterAnimInfo::TertiaryNormal)},
	{SCPTableFormatDescriptor::GetVariableName("+Ter_Afterburn"),
	 ReplaceValue(&SCP::ThrusterAnimInfo::TertiaryAfterburn)},
	{SCPTableFormatDescriptor::GetVariableName("+Dist_Normal"), ReplaceValue(&SCP::ThrusterAnimInfo::DistortionNormal)},
	{SCPTableFormatDescriptor::GetVariableName("+Dist_Afterburn"),
	 ReplaceValue(&SCP::ThrusterAnimInfo::DistortionAfterburn)}

};

const DeserializationHandlers<SCP::GlowBitmapInfo> SCPTableBase<SCP::GlowBitmapInfo>::Deserializers = {
	{SCPTableFormatDescriptor::GetVariableName("+Normal"), ReplaceValue(&SCP::GlowBitmapInfo::Normal)},
	{SCPTableFormatDescriptor::GetVariableName("+Afterburn"), ReplaceValue(&SCP::GlowBitmapInfo::Afterburn)}};

const DeserializationHandlers<SCP::SpeciesDefaults> SCPTableBase<SCP::SpeciesDefaults>::Deserializers = {
	{SCPTableFormatDescriptor::GetVariableName("$Species_Name"), ReplaceValue(&SCP::SpeciesDefaults::Name)},
	{SCPTableFormatDescriptor::GetVariableName("$Default IFF"), DeserializeToField(&SCP::SpeciesDefaults::DefaultIFF)},
	{SCPTableFormatDescriptor::GetVariableName("$Fred Color"), DeserializeToField(&SCP::SpeciesDefaults::FredColor)},
	{SCPTableFormatDescriptor::GetVariableName("$Fred Colour"), DeserializeToField(&SCP::SpeciesDefaults::FredColor)},
	{SCPTableFormatDescriptor::GetVariableName("+Debris_Texture"),
	 DeserializeToField(&SCP::SpeciesDefaults::DebrisTexture)},
	{SCPTableFormatDescriptor::GetVariableName("+Shield_Hit_ani"),
	 DeserializeToField(&SCP::SpeciesDefaults::ShieldHitAni)},
	{"AnimInfo", Merge(&SCP::SpeciesDefaults::ThrusterAnims)},
	{"GlowInfo", Merge(&SCP::SpeciesDefaults::GlowBitmaps)},
	{SCPTableFormatDescriptor::GetVariableName("$AwacsMultiplier"),
	 DeserializeToField(&SCP::SpeciesDefaults::AwacsMultiplier)},
	{SCPTableFormatDescriptor::GetVariableName("$Countermeasure type"),
	 DeserializeToField(&SCP::SpeciesDefaults::CountermeasureType)}

};

void LoadSpeciesDefs(SCP::SpeciesDefaultsTable* ClassInstance, const SCPParsedTableData& InData)
{
	for (auto Node : InData.nodes)
	{
		ClassInstance->Deserialize(Node->name, *Node);
	}
}

const DeserializationHandlers<SCP::SpeciesDefaultsTable> SCPTableBase<SCP::SpeciesDefaultsTable>::Deserializers = {
	{"SpeciesEntry", PushBack(&SCP::SpeciesDefaultsTable::Entries)},
	{SCPTableFormatDescriptor::GetSectionName("Species Defs"), LoadSpeciesDefs}};