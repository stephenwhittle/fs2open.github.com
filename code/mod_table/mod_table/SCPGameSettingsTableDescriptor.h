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

	auto CampaignSettingsSection = GameSettingsFile.Define("CampaignSettingsSection",
		GameSettingsFile.Literal("#CAMPAIGN SETTINGS")
		& GameSettingsFile.OptionalVariable("$Default Campaign File Name")

		& GameSettingsFile.Optional(
			GameSettingsFile.RequiredVariable("#Ignored Campaign File Names")
			& GameSettingsFile.ListOf(
				GameSettingsFile.RequiredVariable("$Campaign File Name")
			)
		)
		& GameSettingsFile.Optional(
			GameSettingsFile.RequiredVariable("#Ignored Mission File Names")
			& GameSettingsFile.ListOf(
				GameSettingsFile.RequiredVariable("$Mission File Name")
			)
		)
		& GameSettingsFile.OptionalVariable("$Red-alert applies to delayed ships")
	);

	auto HudSettingsSection = GameSettingsFile.Define("HudSettingsSection",
		GameSettingsFile.Literal("#HUD SETTINGS")
		& GameSettingsFile.OptionalVariable("$Directive Wait Time")
		& GameSettingsFile.OptionalVariable("$Cutscene camera displays HUD")
		& GameSettingsFile.OptionalVariable("$Cutscene camera disables HUD")
		& GameSettingsFile.OptionalVariable("$Full color head animations")
		& GameSettingsFile.OptionalVariable("$Color head animations with hud colors")
	);

	auto SEXPSettingsSection = GameSettingsFile.Define("SEXPSettingsSection",
		GameSettingsFile.Literal("#SEXP SETTINGS")
		& GameSettingsFile.OptionalVariable("$Loop SEXPs Then Arguments")
		& GameSettingsFile.OptionalVariable("$Use Alternate Chaining Behavior")
		);

	auto GameSettingsTable = GameSettingsFile.Define("Root", 
		GameSettingsFile.Optional(GameSettingsSection));

	// clang-format on
}