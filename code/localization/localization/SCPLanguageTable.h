#pragma once
#include "parse/SCPTable.h"

class SCPLanguageConfiguration : public SCPTableBase<SCPLanguageConfiguration>
{
public:
	SCPTableProperty<std::string> Language;
	SCPTableProperty<std::string> Extension;
	SCPTableProperty<int> SpecialCharacterIndex;
};

class SCPLanguageString : public SCPTableBase<SCPLanguageString>
{
public:
	SCPTableProperty<int> StringIndex;
	SCPTableProperty<std::string> StringValue;
	SCPTableProperty<int> LowOffset;
	SCPTableProperty<int> HighOffset;
};

class SCPLanguageTable : public SCPTableBase<SCPLanguageTable>
{
public:
	SCPTableProperty<std::vector<SCPLanguageConfiguration>> SupportedLanguages;
	SCPTableProperty <std::map<std::string, std::vector<SCPLanguageString>>> LanguageStrings;
};

template<>
tl::optional<SCPLanguageTable> construct(const SCPParsedTableData& InData)
{
	SCPLanguageTable LanguageTable;

	for (auto Node : InData.nodes)
	{
		LanguageTable.Deserialize(Node->name, *Node);
	}
	return LanguageTable;
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

template<>
tl::optional<SCPLanguageString> construct(const SCPParsedTableData& InData)
{
	SCPLanguageString StringEntry;
	std::size_t NodeCount = InData.nodes.size();

	if (NodeCount >= 2)
	{
		StringEntry.StringIndex = construct<int>(*InData.nodes[0]);
		StringEntry.StringValue = construct<std::string>(*InData.nodes[1]);
		if (NodeCount >= 3) { StringEntry.LowOffset = construct<int>(*InData.nodes[2]); }
		if (NodeCount >= 4) { StringEntry.LowOffset = construct<int>(*InData.nodes[3]); }
		return StringEntry;
	}

	return tl::optional<SCPLanguageString>();
}