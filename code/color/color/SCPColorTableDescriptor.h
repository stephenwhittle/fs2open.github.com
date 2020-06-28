#pragma once
#include "parse/SCPTableFormatDescriptor.h"

SCPTableFormatDescriptor ColorsFile;

auto TeamColorInfo = ColorsFile.Define("TeamColor",
	ColorsFile.RequiredVariable("$Team Name")
	& ColorsFile.RequiredVariable("$Team Stripe Color")
	& ColorsFile.RequiredVariable("$Team Base Color")
);

auto ColorTag = ColorsFile.Define("ColorTag",
	ColorsFile.RequiredVariable("$Tag")
	& ColorsFile.OneOf(
		ColorsFile.RequiredVariable("+Color"),
		ColorsFile.RequiredVariable("+Friendly"),
		ColorsFile.RequiredVariable("+Hostile"),
		ColorsFile.RequiredVariable("+Neutral")
	)
);


auto ColorTable = ColorsFile.Define("Root",
	ColorsFile.Optional(
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
			& ColorsFile.OptionalVariable("$Light Pink")
			& ColorsFile.OptionalVariable("$Yellow")
			& ColorsFile.OptionalVariable("$Bright Yellow")
			& ColorsFile.OptionalVariable("$Orange")
			& ColorsFile.OptionalVariable("$UI Light Green")
			& ColorsFile.OptionalVariable("$UI Green")
			& ColorsFile.OptionalVariable("$UI Light Pink")
			& ColorsFile.OptionalVariable("$UI Pink")
		)
	)
	& ColorsFile.Optional(
		ColorsFile.Section("Team Colors",
			ColorsFile.ListOf(TeamColorInfo, false)
		)
	)
	& ColorsFile.Optional(
		ColorsFile.Section("Interface Colors",
			ColorsFile.OptionalVariable("$Text Normal")
			& ColorsFile.OptionalVariable("$Text Subselected")
			& ColorsFile.OptionalVariable("$Text Selected")
			& ColorsFile.OptionalVariable("$Text Error")
			& ColorsFile.OptionalVariable("$Text Error Highlighted")
			& ColorsFile.OptionalVariable("$Text Active")
			& ColorsFile.OptionalVariable("$Text Active Highlighted")
			& ColorsFile.OptionalVariable("$Text Heading")
			& ColorsFile.OptionalVariable("$More Indicator")
			& ColorsFile.OptionalVariable("$Bright More Indicator")
			& ColorsFile.OptionalVariable("$Bright")
			& ColorsFile.OptionalVariable("$Normal")
			& ColorsFile.OptionalVariable("$Briefing Grid")
		)
	)
	& ColorsFile.Optional(
		ColorsFile.Section("Color Tags",
			ColorsFile.ListOf(ColorTag, true)
		)
	)
	& ColorsFile.Optional(
		ColorsFile.Section("Default Text Colors",
			ColorsFile.OptionalVariable("$Fiction Viewer")
			& ColorsFile.OptionalVariable("$Command Briefing")
			& ColorsFile.OptionalVariable("$Briefing")
			& ColorsFile.OptionalVariable("$Redalert Briefing")
			& ColorsFile.OptionalVariable("$Debriefing")
			& ColorsFile.OptionalVariable("$Recommendation")
			& ColorsFile.OptionalVariable("$Loop Briefing")
		)

	)
);