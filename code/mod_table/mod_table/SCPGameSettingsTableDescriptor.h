#pragma once

#include "parse/SCPTableFormatDescriptor.h"

namespace SCP
{
	SCPTableFormatDescriptor GameSettingsFile;
	// clang-format off
	auto GameSettingsSection = GameSettingsFile.Define("GameSettingsSection",
		GameSettingsFile.Literal("#GAME SETTINGS")
		& GameSettingsFile.Optional(
			GameSettingsFile.OneOf(
				GameSettingsFile.ObjectHeader("$Minimum Version"),
				GameSettingsFile.ObjectHeader("$Target Version")
			)
			& GameSettingsFile.RequiredVariable("+Major")
			& GameSettingsFile.RequiredVariable("+Minor")
			& GameSettingsFile.RequiredVariable("+Build")
			& GameSettingsFile.OptionalVariable("+Revision")
		)
		& GameSettingsFile.OptionalVariable("$Window Title")
		& GameSettingsFile.OptionalVariable("$Window Icon")
		& GameSettingsFile.OptionalVariable("$Unicode Mode")
		& GameSettingsFile.OptionalVariable("$Use tabled strings for the default language")
	);

	auto GameSettingsTable = GameSettingsFile.Define("Root", 
		GameSettingsFile.Optional(GameSettingsSection));

	// clang-format on
}