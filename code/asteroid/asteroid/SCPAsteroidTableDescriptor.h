#pragma once

#include "parse/SCPTableFormatDescriptor.h"

SCPTableFormatDescriptor AsteroidsFile;
// clang-format off

auto AsteroidSplitInfo = AsteroidsFile.Define("AsteroidSplitInfo", 
	AsteroidsFile.RequiredVariable("$Split") &
	AsteroidsFile.OptionalVariable("+Min") &
	AsteroidsFile.OptionalVariable("+Max")
	);

auto AsteroidDataEntry = AsteroidsFile.Define(
	"AsteroidData", 
	AsteroidsFile.RequiredVariable("$Name") & 
	AsteroidsFile.RequiredVariable("$POF file1") &
	AsteroidsFile.RequiredVariable("$POF file2") & 
	AsteroidsFile.OptionalVariable("$POF file3") &
	AsteroidsFile.RequiredVariable("$Detail distance") &
	AsteroidsFile.RequiredVariable("$Max Speed") & 
	AsteroidsFile.OptionalVariable("$Damage Type") &
	AsteroidsFile.OptionalVariable("$Explosion Animations") &
	AsteroidsFile.OptionalVariable("$Explosion Radius Mult") &
	AsteroidsFile.RequiredVariable("$Expl inner rad") &
	AsteroidsFile.RequiredVariable("$Expl outer rad") &
	AsteroidsFile.RequiredVariable("$Expl damage") &
	AsteroidsFile.RequiredVariable("$Expl blast") &
	AsteroidsFile.RequiredVariable("$Hitpoints") &
	AsteroidsFile.ListOf(AsteroidSplitInfo, true ));

auto AsteroidGrouping = AsteroidsFile.Define("AsteroidDataGroup",
	AsteroidDataEntry &
	AsteroidDataEntry &
	AsteroidDataEntry);

auto AsteroidsTable = AsteroidsFile.Define("Root", 
	AsteroidsFile.Section("Asteroid Types",
		AsteroidsFile.ListOf(AsteroidGrouping, false)) &
	AsteroidsFile.RequiredVariable("$Impact Explosion") &
	AsteroidsFile.RequiredVariable("$Impact Explosion Radius") &
	AsteroidsFile.OptionalVariable("$Briefing Icon Closeup Model") &
	AsteroidsFile.OptionalVariable("$Briefing Icon Closeup Position") &
	AsteroidsFile.OptionalVariable("$Briefing Icon Closeup Zoom"));

// clang-format on