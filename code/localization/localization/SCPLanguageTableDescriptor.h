#pragma once
#include "parse/SCPTableFormatDescriptor.h"

SCPTableFormatDescriptor LanguagesFile;

auto LanguageConfiguration = LanguagesFile.Define("LanguageConfiguration",
	LanguagesFile.RequiredVariable("$Language")
	& LanguagesFile.RequiredVariable("+Extension")
	& LanguagesFile.RequiredVariable("+Special Character Index")
	);

auto StringEntry = LanguagesFile.Define("StringEntry",
	LanguagesFile.GetDecl("Number")
	&LanguagesFile.Character(',')
	& LanguagesFile.GetDecl("QuotedString") 
	& LanguagesFile.Optional(LanguagesFile.GetDecl("Number")) 
	& LanguagesFile.Optional(LanguagesFile.GetDecl("Number"))
);

auto LanguageHeader = LanguagesFile.Define("LanguageHeader",
	LanguagesFile.Character('#')
	& LanguagesFile.GetDecl("String")
);
auto LanguageStrings = LanguagesFile.Define("LanguageStrings",
		LanguageHeader
		& LanguagesFile.ListOf(StringEntry, true)
		& LanguagesFile.Literal("#end")
	);

auto LanguageTable = LanguagesFile.Define("Root",
	LanguagesFile.Optional(
		LanguagesFile.Section("Supported Languages",
			LanguagesFile.ListOf(LanguageConfiguration, false)
		)
	)
	& LanguagesFile.ListOf(LanguageStrings, true)
);