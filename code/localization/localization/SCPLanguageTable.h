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
tl::optional<SCPLanguageTable> construct(const SCPParsedTableData& InData);


template <>
tl::optional<SCPLanguageConfiguration> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCPLanguageString> construct(const SCPParsedTableData& InData);

