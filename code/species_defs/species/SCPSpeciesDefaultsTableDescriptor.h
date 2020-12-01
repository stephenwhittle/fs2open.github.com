#pragma once

#include "parse/SCPTableFormatDescriptor.h"

// clang-format off
namespace SCP
{
	SCPTableFormatDescriptor SpeciesDefaultsFile;
	
	auto SpeciesEntry = SpeciesDefaultsFile.Define("SpeciesEntry", 
		SpeciesDefaultsFile.RequiredVariable("$Species_Name")
		& SpeciesDefaultsFile.Optional(SpeciesDefaultsFile.XMTNoCreateFlag())
		& SpeciesDefaultsFile.OptionalVariable("$Default IFF")
		& SpeciesDefaultsFile.Optional(
			SpeciesDefaultsFile.OneOf(
				SpeciesDefaultsFile.RequiredVariable("$Fred Color"),
				SpeciesDefaultsFile.RequiredVariable("$Fred Colour")
			)
		)
		& SpeciesDefaultsFile.Optional(
			SpeciesDefaultsFile.ObjectHeader("$MiscAnims")
		)
		& SpeciesDefaultsFile.OptionalVariable("+Debris_Texture")
		& SpeciesDefaultsFile.OptionalVariable("+Shield_Hit_ani")
		& SpeciesDefautltsFile.Optional(
			SpeciesDefaultsFile.Define("AnimInfo", 
				SpeciesDefaultsFile.ObjectHeader("$ThrustAnims")
				& SpeciesDefaultsFile.Optional(
					SpeciesDefaultsFile.OneOf(
						SpeciesDefaultsFile.RequiredVariable("+Pri_Normal"),
						SpeciesDefaultsFile.RequiredVariable("+Normal")
					)
				)
				& SpeciesDefaultsFile.Optional(
					SpeciesDefaultsFile.OneOf(
						SpeciesDefaultsFile.RequiredVariable("+Pri_Afterburn"),
						SpeciesDefaultsFile.RequiredVariable("+Afterburn")
					)
				)
				& SpeciesDefaultsFile.OptionalVariable("+Sec_Normal")
				& SpeciesDefaultsFile.OptionalVariable("+Sec_Afterburn")
				& SpeciesDefaultsFile.OptionalVariable("+Ter_Normal")
				& SpeciesDefaultsFile.OptionalVariable("+Ter_Afterburn")
				& SpeciesDefaultsFile.OptionalVariable("+Dist_Normal")
				& SpeciesDefaultsFile.OptionalVariable("+Dist_Afterburn")
			)
		)
		& SpeciesDefaultsFile.Optional(
			SpeciesDefaultsFile.Define("GlowInfo",
				SpeciesDefaultsFile.ObjectHeader("$ThrustGlows")
				& SpeciesDefaultsFile.OptionalVariable("+Normal")
				& SpeciesDefaultsFile.OptionalVariable("+Afterburn")
			)
		)
		& SpeciesDefaultsFile.OptionalVariable("$AwacsMultiplier")
		& SpeciesDefaultsFile.OptionalVariable("$Countermeasure type")
	);
	
	
	auto SpeciesDefaultsTableRoot = SpeciesDefaultsFile.Define("Root", 
		SpeciesDefaultsFile.Section("Species Defs", 
			SpeciesDefaultsFile.OptionalVariable("$NumSpecies")
			&SpeciesDefaultsFile.ListOf(SpeciesEntry, true);
		)
	);
}
// clang-format on