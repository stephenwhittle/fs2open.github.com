#include "localization/SCPLanguageTable.h"
#include "parse/SCPTableFormatDescriptor.h"


const DeserializationHandlers<SCPLanguageConfiguration> SCPTableBase<SCPLanguageConfiguration>::Deserializers = 
{
	{SCPTableFormatDescriptor::GetVariableName("$Language"),
	 DeserializeToField(&SCPLanguageConfiguration::Language)},
	{SCPTableFormatDescriptor::GetVariableName("+Extension"), DeserializeToField(&SCPLanguageConfiguration::Extension)},
	{SCPTableFormatDescriptor::GetVariableName("+Special Character Index"), 
		DeserializeToField(&SCPLanguageConfiguration::SpecialCharacterIndex)}
};


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