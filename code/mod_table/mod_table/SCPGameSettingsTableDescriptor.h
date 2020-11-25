#pragma once

#include "parse/SCPTableFormatDescriptor.h"

namespace SCP
{
	SCPTableFormatDescriptor GameSettingsFile;
	// clang-format off
	auto GameSettingsSection = GameSettingsFile.Define("GameSettingsSection",
		GameSettingsFile.LiteralToken("#GAME SETTINGS")
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
		GameSettingsFile.LiteralToken("#CAMPAIGN SETTINGS")
		& GameSettingsFile.OptionalVariable("$Default Campaign File Name")

		& GameSettingsFile.Optional(
			GameSettingsFile.Literal("#Ignored Campaign File Names")
			& GameSettingsFile.ListOf(
				GameSettingsFile.RequiredVariable("$Campaign File Name"), true
			)
		)
		& GameSettingsFile.Optional(
			GameSettingsFile.Literal("#Ignored Mission File Names")
			& GameSettingsFile.ListOf(
				GameSettingsFile.RequiredVariable("$Mission File Name"), true
			)
		)
		& GameSettingsFile.OptionalVariable("$Red-alert applies to delayed ships")
	);

	auto HudSettingsSection = GameSettingsFile.Define("HudSettingsSection",
		GameSettingsFile.LiteralToken("#HUD SETTINGS")
		& GameSettingsFile.OptionalVariable("$Directive Wait Time")
		& GameSettingsFile.OptionalVariable("$Cutscene camera displays HUD")
		& GameSettingsFile.OptionalVariable("$Cutscene camera disables HUD")
		& GameSettingsFile.OptionalVariable("$Full color head animations")
		& GameSettingsFile.OptionalVariable("$Color head animations with hud colors")
	);

	auto SEXPSettingsSection = GameSettingsFile.Define("SEXPSettingsSection",
		GameSettingsFile.LiteralToken("#SEXP SETTINGS")
		& GameSettingsFile.OptionalVariable("$Loop SEXPs Then Arguments")
		& GameSettingsFile.OptionalVariable("$Use Alternate Chaining Behavior")
		);

	auto GraphicsSettingsSection = GameSettingsFile.Define("GraphicsSettingsSection",
		GameSettingsFile.LiteralToken("#GRAPHICS SETTINGS")
		& GameSettingsFile.OptionalVariable("$Enable External Shaders")
		& GameSettingsFile.OptionalVariable("$Default Detail Level")
		& GameSettingsFile.OptionalVariable("$Briefing Window FOV")
		& GameSettingsFile.OptionalVariable("$Generic Pain Flash Factor")
		& GameSettingsFile.OptionalVariable("$Shield Pain Flash Factor")
		& GameSettingsFile.OptionalVariable("$BMPMAN Slot Limit")
		& GameSettingsFile.Optional(
			GameSettingsFile.Define("EMP Color Section",
				GameSettingsFile.ObjectHeader("$EMP Arc Color")
				& GameSettingsFile.OptionalVariable("+Primary Color Option 1")
				& GameSettingsFile.OptionalVariable("+Primary Color Option 2")
				& GameSettingsFile.OptionalVariable("+Secondary Color Option 1")
			)
		)
		& GameSettingsFile.Optional(
			GameSettingsFile.Define("Damage Color Section",
				GameSettingsFile.ObjectHeader("$Damage Arc Color")
				& GameSettingsFile.OptionalVariable("+Primary Color Option 1")
				& GameSettingsFile.OptionalVariable("+Primary Color Option 2")
				& GameSettingsFile.OptionalVariable("+Secondary Color Option 1")
			)
		)
	);

	auto NetworkSettingsSection = GameSettingsFile.Define("NetworkSettingsSection",
		GameSettingsFile.LiteralToken("#NETWORK SETTINGS")
		& GameSettingsFile.OptionalVariable("$FS2NetD port")
	);

	auto SoundSettingsSection = GameSettingsFile.Define("SoundSettingsSection",
		GameSettingsFile.LiteralToken("#SOUND SETTINGS")
		& GameSettingsFile.OptionalVariable("$Default Sound Volume")
		& GameSettingsFile.OptionalVariable("$Default Music Volume")
		& GameSettingsFile.OptionalVariable("$Default Voice Volume")
	);

	auto FredSettingsSection = GameSettingsFile.Define("FredSettingsSection",
		GameSettingsFile.LiteralToken("#FRED SETTINGS")
		& GameSettingsFile.OptionalVariable("$Disable Hard Coded Message Head Ani Files")
		& GameSettingsFile.OptionalVariable("$Enable scripting in FRED")
	);

	auto OtherSettingsSection = GameSettingsFile.Define("OtherSettingsSection",
		GameSettingsFile.LiteralToken("#OTHER SETTINGS")
		& GameSettingsFile.OptionalVariable("$Fixed Turret Collisions")
		& GameSettingsFile.OptionalVariable("$Damage Impacted Subsystem First")
		& GameSettingsFile.OptionalVariable("$Default ship select effect")
		& GameSettingsFile.OptionalVariable("$Default weapon select effect")
		& GameSettingsFile.OptionalVariable("$Weapons inherit parent collision group")
		& GameSettingsFile.OptionalVariable("$Flight controls follow eyepoint orientation")
		& GameSettingsFile.OptionalVariable("$Beams Use Damage Factors")
		& GameSettingsFile.OptionalVariable("$Default fiction viewer UI")
		& GameSettingsFile.OptionalVariable("$Movie subtitle font")
		& GameSettingsFile.OptionalVariable("$Disable built-in translations")
		& GameSettingsFile.OptionalVariable("$Weapon shockwave damage respects huge ship flags")
		& GameSettingsFile.OptionalVariable("$Enable external default scripts")
		& GameSettingsFile.OptionalVariable("$Player warpout speed")
		& GameSettingsFile.OptionalVariable("$Target warpout match percent")
		& GameSettingsFile.OptionalVariable("$Minimum player warpout time")
		& GameSettingsFile.OptionalVariable("$Enable in-game options")
		& GameSettingsFile.OptionalVariable("$Dinky Shockwave Default Multiplier")
		& GameSettingsFile.OptionalVariable("$Shockwaves Always Damage Bombs")
		& GameSettingsFile.OptionalVariable("$Shockwaves Damage All Object Types Once")
		& GameSettingsFile.OptionalVariable("$Use Engine Wash Intensity")
		& GameSettingsFile.OptionalVariable("$Swarmers Lead Targets")
		& GameSettingsFile.OptionalVariable("$Damage Threshold for Weapons Subsystems to Trigger Turret Inaccuracy")
		& GameSettingsFile.OptionalVariable("$AI use framerate independent turning")
		& GameSettingsFile.OptionalVariable("+AI respect tabled turn time and rotdamp")
	);

	auto GameSettingsTopLevel = GameSettingsFile.Define("Root", 
		GameSettingsFile.Optional(GameSettingsSection)
		& GameSettingsFile.Optional(CampaignSettingsSection)
		& GameSettingsFile.Optional(HudSettingsSection)
		& GameSettingsFile.Optional(SEXPSettingsSection)
		& GameSettingsFile.Optional(GraphicsSettingsSection)
		& GameSettingsFile.Optional(NetworkSettingsSection)
		& GameSettingsFile.Optional(SoundSettingsSection)
		& GameSettingsFile.Optional(FredSettingsSection)
		& GameSettingsFile.Optional(OtherSettingsSection)
		& GameSettingsFile.LiteralToken("#END")
	);

	// clang-format on
}