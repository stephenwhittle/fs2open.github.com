#pragma once
#include "parse/SCPTableFormatDescriptor.h"

SCPTableFormatDescriptor ColorsFile;

auto DefaultColorSection = ColorsFile.Define("DefaultColorSection",
	ColorsFile.Section("Start Colors",
		ColorsFile.OptionalVariable("$Blue")
		& ColorsFile.OptionalVariable("$Bright Blue")
		& ColorsFile.OptionalVariable("$Green")
		& ColorsFile.OptionalVariable("$Bright Green")
		& ColorsFile.OptionalVariable("$Black")
		& ColorsFile.OptionalVariable("$Grey")
		& ColorsFile.OptionalVariable("$Silver")
		& ColorsFile.OptionalVariable("$White")
		& ColorsFile.OptionalVariable("$Bright White")
		& ColorsFile.OptionalVariable("$Violet Gray")
		& ColorsFile.OptionalVariable("$Violet")
		& ColorsFile.OptionalVariable("$Dim Red")
		& ColorsFile.OptionalVariable("$Red")
		& ColorsFile.OptionalVariable("$Bright Red") 
		& ColorsFile.OptionalVariable("$Pink")
		& ColorsFile.OptionalVariable("$Bright Pink")
		& ColorsFile.OptionalVariable("$Yellow")
		& ColorsFile.OptionalVariable("$Bright Yellow")
		& ColorsFile.OptionalVariable("$Orange")
		& ColorsFile.OptionalVariable("$UI Light Green") 
		& ColorsFile.OptionalVariable("$UI Green") 
		& ColorsFile.OptionalVariable("$UI Light Pink") 
		& ColorsFile.OptionalVariable("$UI Pink")
		)
	);

auto TeamColorInfo = ColorsFile.Define("TeamColor",
	ColorsFile.RequiredVariable("$Team Name") 
	& ColorsFile.RequiredVariable("$Team Stripe Color") 
	& ColorsFile.RequiredVariable("$Team Base Color")
);

auto TeamColorSection = ColorsFile.Define("TeamColorSection",
	ColorsFile.Section("Team Colors",
		ColorsFile.ListOf(TeamColorInfo, false)
	)
);

auto ColorTable = ColorsFile.Define("Root",
	ColorsFile.Optional(DefaultColorSection)
	& ColorsFile.Optional(TeamColorSection)
);