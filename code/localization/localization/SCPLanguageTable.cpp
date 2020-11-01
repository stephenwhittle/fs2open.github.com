#include "localization/SCPLanguageTable.h"
#include "parse/SCPTableFormatDescriptor.h"

// clang-format off
const DeserializationHandlers<SCPLanguageConfiguration> SCPTableBase<SCPLanguageConfiguration>::Deserializers = 
{
	{SCPTableFormatDescriptor::GetVariableName("$Language"), DeserializeToField(&SCPLanguageConfiguration::Language)},
	{SCPTableFormatDescriptor::GetVariableName("+Extension"), DeserializeToField(&SCPLanguageConfiguration::Extension)},
	{SCPTableFormatDescriptor::GetVariableName("+Special Character Index"), DeserializeToField(&SCPLanguageConfiguration::SpecialCharacterIndex)}
};
// clang-format on

void CreateLanguageConfiguration(SCPLanguageTable* ClassInstance, const SCPParsedTableData& InData)
{
}

void AddLanguageStrings(SCPLanguageTable* ClassInstance, const SCPParsedTableData& InData)
{

}

const DeserializationHandlers<SCPLanguageTable> SCPTableBase<SCPLanguageTable>::Deserializers =
{
	{"LanguageConfiguration", CreateLanguageConfiguration},
	{"LanguageStrings", AddLanguageStrings}
};

template <>
tl::optional<SCPLanguageTable> construct(const SCPParsedTableData& InData)
{
	SCPLanguageTable LanguageTable;

	for (auto Node : InData.nodes) {
		LanguageTable.Deserialize(Node->name, *Node);
	}
	return LanguageTable;
}

template <>
tl::optional<SCPLanguageString> construct(const SCPParsedTableData& InData)
{
	SCPLanguageString StringEntry;
	std::size_t NodeCount = InData.nodes.size();

	if (NodeCount >= 2) {
		StringEntry.StringIndex = construct<int>(*InData.nodes[0]);
		StringEntry.StringValue = construct<std::string>(*InData.nodes[1]);
		if (NodeCount >= 3) {
			StringEntry.LowOffset = construct<int>(*InData.nodes[2]);
		}
		if (NodeCount >= 4) {
			StringEntry.HighOffset = construct<int>(*InData.nodes[3]);
		}
		return StringEntry;
	}

	return tl::optional<SCPLanguageString>();
}


template <>
tl::optional<SCPLanguageConfiguration> construct(const SCPParsedTableData& InData)
{
	SCPLanguageConfiguration LanguageConfig;

	for (auto Node : InData.nodes) {
		LanguageConfig.Deserialize(Node->name, *Node);
	}
	return LanguageConfig;
}
