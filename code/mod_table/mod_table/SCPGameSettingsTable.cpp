#include "SCPGameSettingsTable.h"
#include "parse/SCPTableFormatDescriptor.h"

// clang-format off
	const DeserializationHandlers<SCP::GameSettingsSubsection> SCPTableBase<SCP::GameSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("+Major"), ReplaceValue(&SCP::GameSettingsSubsection::MajorVersion)},
		{SCPTableFormatDescriptor::GetVariableName("+Minor"), ReplaceValue(&SCP::GameSettingsSubsection::MinorVersion)},
		{SCPTableFormatDescriptor::GetVariableName("+Build"), ReplaceValue(&SCP::GameSettingsSubsection::BuildVersion)},
		{SCPTableFormatDescriptor::GetVariableName("+Revision"), ReplaceValue(&SCP::GameSettingsSubsection::Revision)},
		{SCPTableFormatDescriptor::GetVariableName("$Window Title"), ReplaceValue(&SCP::GameSettingsSubsection::WindowTitle)},
		{SCPTableFormatDescriptor::GetVariableName("$Window Icon"), ReplaceValue(&SCP::GameSettingsSubsection::WindowIcon)},
		{SCPTableFormatDescriptor::GetVariableName("$Unicode Mode"), ReplaceValue(&SCP::GameSettingsSubsection::UnicodeMode)},
		{SCPTableFormatDescriptor::GetVariableName("$Use tabled strings for the default language"), ReplaceValue(&SCP::GameSettingsSubsection::UseStringTableForEnglish)}
	};

	const DeserializationHandlers<SCP::CampaignSettingsSubsection> SCPTableBase<SCP::CampaignSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Default Campaign File Name"), ReplaceValue(&SCP::CampaignSettingsSubsection::DefaultCampaignFileName)},
		{SCPTableFormatDescriptor::GetVariableName("$Campaign File Name"), PushBack(&SCP::CampaignSettingsSubsection::IgnoredCampaignFileNames)},
		{SCPTableFormatDescriptor::GetVariableName("$Mission File Name"), PushBack(&SCP::CampaignSettingsSubsection::IgnoredMissionFileNames)},
		{SCPTableFormatDescriptor::GetVariableName("$Red-alert applies to delayed ships"), ReplaceValue(&SCP::CampaignSettingsSubsection::RedAlertAppliedToDelayedShips)}
		
	};

	const DeserializationHandlers<SCP::HUDSettingsSubsection> SCPTableBase<SCP::HUDSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Directive Wait Time"), ReplaceValue(&SCP::HUDSettingsSubsection::DirectiveWaitTime)},
		{SCPTableFormatDescriptor::GetVariableName("$Cutscene camera displays HUD"), ReplaceValue(&SCP::HUDSettingsSubsection::CutsceneCameraDisplaysHUD)},
		{SCPTableFormatDescriptor::GetVariableName("$Cutscene camera disables HUD"), ReplaceValue(&SCP::HUDSettingsSubsection::CutsceneCameraDisplaysHUD)},
		{SCPTableFormatDescriptor::GetVariableName("$Full color head animation"), ReplaceValue(&SCP::HUDSettingsSubsection::FullColorHeadAnimations)},
		{SCPTableFormatDescriptor::GetVariableName("$Color head animations with hud colors"), ReplaceValue(&SCP::HUDSettingsSubsection::FullColorHeadAnimations)},
	};

	const DeserializationHandlers<SCP::SexpSettingsSubsection> SCPTableBase<SCP::SexpSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Loop SEXPs Then Arguments"), ReplaceValue(&SCP::SexpSettingsSubsection::LoopSexpThenArgs)},
		{SCPTableFormatDescriptor::GetVariableName("$Use Alternate Chaining Behavior"), ReplaceValue(&SCP::SexpSettingsSubsection::UseAlternateChainingBehaviour)}
	};


namespace
{
	auto DeserializeEMPColorSection = [](SCP::GraphicsSettingsSubsection* ClassInstance, const SCPParsedTableData& InData) 
	{
		
	};
	auto DeserializeDamageColorSection = [](SCP::GraphicsSettingsSubsection* ClassInstance, const SCPParsedTableData& InData) 
	{
		
	};
	
} // namespace

	const DeserializationHandlers<SCP::GraphicsSettingsSubsection> SCPTableBase<SCP::GraphicsSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Enable External Shaders"), ReplaceValue(&SCP::GraphicsSettingsSubsection::EnableExternalShaders)},
		{SCPTableFormatDescriptor::GetVariableName("$Default Detail Level"), ReplaceValue(&SCP::GraphicsSettingsSubsection::DefaultDetailLevel)},
		{SCPTableFormatDescriptor::GetVariableName("$Briefing Window FOV"), ReplaceValue(&SCP::GraphicsSettingsSubsection::BriefingWindowFOV)},
		{SCPTableFormatDescriptor::GetVariableName("$Generic Pain Flash Factor"), ReplaceValue(&SCP::GraphicsSettingsSubsection::GenericPainFlashFactor)},
		{SCPTableFormatDescriptor::GetVariableName("$Shield Pain Flash Factor"), ReplaceValue(&SCP::GraphicsSettingsSubsection::ShieldPainFlashFactor)},
		{SCPTableFormatDescriptor::GetVariableName("$BMPMAN Slot Limit"), ReplaceValue(&SCP::GraphicsSettingsSubsection::BMPManSlotLimit)},
		{"EMP Color Section", DeserializeEMPColorSection},
		{"Damage Color Section", DeserializeDamageColorSection}
	};


	const DeserializationHandlers<SCP::NetworkSettingsSubsection> SCPTableBase<SCP::NetworkSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$FS2NetD port"), ReplaceValue(&SCP::NetworkSettingsSubsection::FS2NetDPort)}		
	};

	const DeserializationHandlers<SCP::SoundSettingsSubsection> SCPTableBase<SCP::SoundSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Default Sound Volume"), ReplaceValue(&SCP::SoundSettingsSubsection::DefaultSoundVolume)},
		{SCPTableFormatDescriptor::GetVariableName("$Default Music Volume"), ReplaceValue(&SCP::SoundSettingsSubsection::DefaultMusicVolume)},
		{SCPTableFormatDescriptor::GetVariableName("$Default Voice Volume"), ReplaceValue(&SCP::SoundSettingsSubsection::DefaultVoiceVolume)}
	};

	const DeserializationHandlers<SCP::FredSettingsSubsection> SCPTableBase<SCP::FredSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Disable Hard Coded Message Head Ani Files"), ReplaceValue(&SCP::FredSettingsSubsection::DisableHardcodedMessageHeadAnimations)},
		{SCPTableFormatDescriptor::GetVariableName("$Enable scripting in FRED"), ReplaceValue(&SCP::FredSettingsSubsection::EnableScriptsInFred)}
	};

	const DeserializationHandlers<SCP::OtherSettingsSubsection> SCPTableBase<SCP::OtherSettingsSubsection>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetVariableName("$Fixed Turret Collisions"), ReplaceValue(&SCP::OtherSettingsSubsection::FixedTurretCollisions)},
		{SCPTableFormatDescriptor::GetVariableName("$Damage Impacted Subsystem First"), ReplaceValue(&SCP::OtherSettingsSubsection::DamageImpactedSubsystemFirst)},
		{SCPTableFormatDescriptor::GetVariableName("$Default ship select effect"), ReplaceValue(&SCP::OtherSettingsSubsection::DefaultShipSelectEffect)},
		{SCPTableFormatDescriptor::GetVariableName("$Default weapon select effect"), ReplaceValue(&SCP::OtherSettingsSubsection::DefaultWeaponSelectEffect)},
		{SCPTableFormatDescriptor::GetVariableName("$Weapons inherit parent collision group"), ReplaceValue(&SCP::OtherSettingsSubsection::WeaponsInheritParentCollisionGroup)},
		{SCPTableFormatDescriptor::GetVariableName("$Flight controls follow eyepoint orientation"), ReplaceValue(&SCP::OtherSettingsSubsection::FlightControlsFollowEyeOrientation)},
		{SCPTableFormatDescriptor::GetVariableName("$Beams Use Damage Factors"), ReplaceValue(&SCP::OtherSettingsSubsection::BeamsUseDamageFactors)},
		{SCPTableFormatDescriptor::GetVariableName("$Default fiction viewer UI"), ReplaceValue(&SCP::OtherSettingsSubsection::DefaultFictionViewerUI)},
		{SCPTableFormatDescriptor::GetVariableName("$Movie subtitle font"), ReplaceValue(&SCP::OtherSettingsSubsection::MovieSubtitleFont)},
		{SCPTableFormatDescriptor::GetVariableName("$Disable built-in translations"), ReplaceValue(&SCP::OtherSettingsSubsection::DisableBuiltinTranslations)},
		{SCPTableFormatDescriptor::GetVariableName("$Weapon shockwave damage respects huge ship flags"), ReplaceValue(&SCP::OtherSettingsSubsection::WeaponShockwaveRespectsHugeFlag)},
		{SCPTableFormatDescriptor::GetVariableName("$Enable external default scripts"), ReplaceValue(&SCP::OtherSettingsSubsection::EnableExternalDefaultScripts)},
		{SCPTableFormatDescriptor::GetVariableName("$Player warpout speed"), ReplaceValue(&SCP::OtherSettingsSubsection::PlayerWarpoutSpeed)},
		{SCPTableFormatDescriptor::GetVariableName("$Target warpout match percent"), ReplaceValue(&SCP::OtherSettingsSubsection::TargetWarpoutMatchPercent)},
		{SCPTableFormatDescriptor::GetVariableName("$Minimum player warpout time"), ReplaceValue(&SCP::OtherSettingsSubsection::MinimumPlayerWarpoutTime)},
		{SCPTableFormatDescriptor::GetVariableName("$Enable in-game options"), ReplaceValue(&SCP::OtherSettingsSubsection::EnableIngameOptions)},
		{SCPTableFormatDescriptor::GetVariableName("$Dinky Shockwave Default Multiplier"), ReplaceValue(&SCP::OtherSettingsSubsection::DinkyShockwaveDefaultMultiplier)},
		{SCPTableFormatDescriptor::GetVariableName("$Shockwaves Always Damage Bombs"), ReplaceValue(&SCP::OtherSettingsSubsection::ShockwavesAlwaysDamageBombs)},
		{SCPTableFormatDescriptor::GetVariableName("$Shockwaves Damage All Object Types Once"), ReplaceValue(&SCP::OtherSettingsSubsection::ShockwavesDamageAllObjectsOnce)},
		{SCPTableFormatDescriptor::GetVariableName("$Use Engine Wash Intensity"), ReplaceValue(&SCP::OtherSettingsSubsection::UseEngineWashIntensity)},
		{SCPTableFormatDescriptor::GetVariableName("$Swarmers Lead Targets"), ReplaceValue(&SCP::OtherSettingsSubsection::SwarmersLeadTargets)},
		{SCPTableFormatDescriptor::GetVariableName("$Damage Threshold for Weapons Subsystems to Trigger Turret Inaccuracy"), ReplaceValue(&SCP::OtherSettingsSubsection::DamageThresholdAffectTurretAccuracy)},
		{SCPTableFormatDescriptor::GetVariableName("$AI use framerate independent turning"), ReplaceValue(&SCP::OtherSettingsSubsection::AIFramerateIndependentTurning)},
		{SCPTableFormatDescriptor::GetVariableName("+AI respect tabled turn time and rotdamp"), ReplaceValue(&SCP::OtherSettingsSubsection::AIRespectTabledTurnTime)}
	};

	const DeserializationHandlers<SCP::GameSettingsTable> SCPTableBase<SCP::GameSettingsTable>::Deserializers = 
	{
		{SCPTableFormatDescriptor::GetSectionName("GameSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::GameSettings)},
		{SCPTableFormatDescriptor::GetSectionName("CampaignSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::CampaignSettings)},
		{SCPTableFormatDescriptor::GetSectionName("HudSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::HudSettings)},
		{SCPTableFormatDescriptor::GetSectionName("SEXPSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::SexpSettings)},
		{SCPTableFormatDescriptor::GetSectionName("GraphicsSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::GraphicsSettings)},
		{SCPTableFormatDescriptor::GetSectionName("NetworkSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::NetworkSettings)},
		{SCPTableFormatDescriptor::GetSectionName("FredSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::SoundSettings)},
		{SCPTableFormatDescriptor::GetSectionName("SoundSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::FredSettings)},
		{SCPTableFormatDescriptor::GetSectionName("OtherSettingsSection"), DeserializeToField(&SCP::GameSettingsTable::OtherSettings)}
	};
// clang-format on

template<>
tl::optional<SCP::GameSettingsTable> construct(const SCPParsedTableData& InData)
{
	SCP::GameSettingsTable Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::GameSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::GameSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::CampaignSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::CampaignSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::HUDSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::HUDSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::SexpSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::SexpSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::GraphicsSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::GraphicsSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::NetworkSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::NetworkSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::FredSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::FredSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::SoundSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::SoundSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}

template<>
tl::optional<SCP::OtherSettingsSubsection> construct(const SCPParsedTableData& InData)
{
	SCP::OtherSettingsSubsection Table;
	for (auto Node : InData.nodes)
	{
		Table.Deserialize(Node->name, *Node);
	}
	return Table;
}