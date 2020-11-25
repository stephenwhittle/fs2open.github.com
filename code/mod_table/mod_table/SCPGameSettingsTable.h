#pragma once

#include "FSColorTypes.h"
#include "FSStdTypes.h"
#include "parse/SCPTable.h"

namespace SCP
{
	class GameSettingsSubsection : public SCPTableBase<GameSettingsSubsection>
	{
	public:
		GameSettingsSubsection()
			: MajorVersion(0),
			  MinorVersion(0),
			  BuildVersion(0),
			  Revision(0),
			  WindowTitle(""),
			  WindowIcon("app_icon_sse"),
			  UnicodeMode(false),
			  UseStringTableForEnglish(false)
		{}
		SCPTableProperty<uint8_t> MajorVersion;
		SCPTableProperty<uint8_t> MinorVersion;
		SCPTableProperty<uint8_t> BuildVersion;
		SCPTableProperty<uint32_t> Revision;
		SCPTableProperty<SCP_string> WindowTitle;
		SCPTableProperty<SCP_string> WindowIcon;
		SCPTableProperty<bool> UnicodeMode;
		SCPTableProperty<bool> UseStringTableForEnglish;
	};

	class CampaignSettingsSubsection : public SCPTableBase<CampaignSettingsSubsection>
	{
	public:
		CampaignSettingsSubsection() : RedAlertAppliedToDelayedShips(false) {};
		SCPTableProperty<SCP_string> DefaultCampaignFileName;
		SCPTableProperty<SCP_vector<SCP_string>> IgnoredCampaignFileNames;
		SCPTableProperty<SCP_vector<SCP_string>> IgnoredMissionFileNames;
		SCPTableProperty<bool> RedAlertAppliedToDelayedShips;
	};

	class HUDSettingsSubsection : public SCPTableBase<HUDSettingsSubsection>
	{
	public:
		HUDSettingsSubsection()
			: DirectiveWaitTime(3000),
			  CutsceneCameraDisplaysHUD(false),
			  FullColorHeadAnimations(false) {};
		SCPTableProperty<uint32_t> DirectiveWaitTime;
		SCPTableProperty<bool> CutsceneCameraDisplaysHUD;
		SCPTableProperty<bool> FullColorHeadAnimations;
	};

	class SexpSettingsSubsection : public SCPTableBase<SexpSettingsSubsection>
	{
	public:
		SexpSettingsSubsection() : LoopSexpThenArgs(false), UseAlternateChainingBehaviour(false) {};
		SCPTableProperty<bool> LoopSexpThenArgs;
		SCPTableProperty<bool> UseAlternateChainingBehaviour;
	};

	class GraphicsSettingsSubsection : public SCPTableBase<GraphicsSettingsSubsection>
	{
	public:
		GraphicsSettingsSubsection()
			: EnableExternalShaders(false),
			  DefaultDetailLevel(3),
			  BriefingWindowFOV(0.29375f),
			  GenericPainFlashFactor(1.f),
			  ShieldPainFlashFactor(0.f),
			  EMPArc_PrimaryColor1(color(64, 64, 5, 255)),
			  EMPArc_PrimaryColor2(color(128, 128, 10, 255)),
			  EMPArc_SecondaryColor1(color(255, 255, 10, 255)),
			  DamageArc_PrimaryColor1(color(64, 64, 5, 255)),
			  DamageArc_PrimaryColor2(color(128, 128, 255, 255)),
			  DamageArc_SecondaryColor1(color(64, 64, 5, 255)) {};
		SCPTableProperty<bool> EnableExternalShaders;
		SCPTableProperty<uint8_t> DefaultDetailLevel;
		SCPTableProperty<float> BriefingWindowFOV;
		SCPTableProperty<float> GenericPainFlashFactor;
		SCPTableProperty<float> ShieldPainFlashFactor;
		SCPTableProperty<uint32_t> BMPManSlotLimit;
		SCPTableProperty<color> EMPArc_PrimaryColor1;
		SCPTableProperty<color> EMPArc_PrimaryColor2;
		SCPTableProperty<color> EMPArc_SecondaryColor1;
		SCPTableProperty<color> DamageArc_PrimaryColor1;
		SCPTableProperty<color> DamageArc_PrimaryColor2;
		SCPTableProperty<color> DamageArc_SecondaryColor1;
	};

	class NetworkSettingsSubsection : public SCPTableBase<NetworkSettingsSubsection>
	{
	public:
		SCPTableProperty<uint32_t> FS2NetDPort;
	};

	class SoundSettingsSubsection : public SCPTableBase<SoundSettingsSubsection>
	{
	public:
		SoundSettingsSubsection() : DefaultSoundVolume(1.0f), DefaultMusicVolume(0.5f), DefaultVoiceVolume(0.7f) {};
		SCPTableProperty<float> DefaultSoundVolume;
		SCPTableProperty<float> DefaultMusicVolume;
		SCPTableProperty<float> DefaultVoiceVolume;
	};

	class FredSettingsSubsection : public SCPTableBase<FredSettingsSubsection>
	{
	public:
		FredSettingsSubsection() : DisableHardcodedMessageHeadAnimations(false), EnableScriptsInFred(false) {};
		SCPTableProperty<bool> DisableHardcodedMessageHeadAnimations;
		SCPTableProperty<bool> EnableScriptsInFred;
	};

	class OtherSettingsSubsection : public SCPTableBase<OtherSettingsSubsection>
	{
	public:
		OtherSettingsSubsection()
			: FixedTurretCollisions(false),
			  DamageImpactedSubsystemFirst(false),
			  DefaultShipSelectEffect("FS2"),
			  DefaultWeaponSelectEffect("FS2"),
			  WeaponsInheritParentCollisionGroup(false),
			  FlightControlsFollowEyeOrientation(false),
			  BeamsUseDamageFactors(false),
			  DefaultFictionViewerUI("auto"),
			  DisableBuiltinTranslations(false),
			  WeaponShockwaveRespectsHugeFlag(false),
			  EnableExternalDefaultScripts(false),
			  PlayerWarpoutSpeed(40.f),
			  TargetWarpoutMatchPercent(0.05f),
			  MinimumPlayerWarpoutTime(3.f),
			  EnableIngameOptions(false),
			  DinkyShockwaveDefaultMultiplier(1.f),
			  ShockwavesAlwaysDamageBombs(false),
			  ShockwavesDamageAllObjectsOnce(false),
			  UseEngineWashIntensity(false),
			  SwarmersLeadTargets(false),
			  DamageThresholdAffectTurretAccuracy(0.7f),
			  AIFramerateIndependentTurning(false),
			  AIRespectTabledTurnTime(false) {};
		SCPTableProperty<bool> FixedTurretCollisions;
		SCPTableProperty<bool> DamageImpactedSubsystemFirst;
		SCPTableProperty<SCP_string> DefaultShipSelectEffect;
		SCPTableProperty<SCP_string> DefaultWeaponSelectEffect;
		SCPTableProperty<bool> WeaponsInheritParentCollisionGroup;
		SCPTableProperty<bool> FlightControlsFollowEyeOrientation;
		SCPTableProperty<bool> BeamsUseDamageFactors;
		SCPTableProperty<SCP_string> DefaultFictionViewerUI;
		SCPTableProperty<SCP_string> MovieSubtitleFont;
		SCPTableProperty<bool> DisableBuiltinTranslations;
		SCPTableProperty<bool> WeaponShockwaveRespectsHugeFlag;
		SCPTableProperty<bool> EnableExternalDefaultScripts;
		SCPTableProperty<float> PlayerWarpoutSpeed;
		SCPTableProperty<float> TargetWarpoutMatchPercent;
		SCPTableProperty<float> MinimumPlayerWarpoutTime;
		SCPTableProperty<bool> EnableIngameOptions;
		SCPTableProperty<float> DinkyShockwaveDefaultMultiplier;
		SCPTableProperty<bool> ShockwavesAlwaysDamageBombs;
		SCPTableProperty<bool> ShockwavesDamageAllObjectsOnce;
		SCPTableProperty<bool> UseEngineWashIntensity;
		SCPTableProperty<bool> SwarmersLeadTargets;
		SCPTableProperty<float> DamageThresholdAffectTurretAccuracy;
		SCPTableProperty<bool> AIFramerateIndependentTurning;
		SCPTableProperty<bool> AIRespectTabledTurnTime;
	};

	class GameSettingsTable : public SCPTableBase<GameSettingsTable>
	{
	public:
		GameSettingsTable()
			: GameSettings(GameSettingsSubsection()),
			  CampaignSettings(CampaignSettingsSubsection()),
			  HudSettings(HUDSettingsSubsection()),
			  SexpSettings(SexpSettingsSubsection()),
			  GraphicsSettings(GraphicsSettingsSubsection()),
			  NetworkSettings(NetworkSettingsSubsection()),
			  SoundSettings(SoundSettingsSubsection()),
			  FredSettings(FredSettingsSubsection()),
			  OtherSettings(OtherSettingsSubsection()) {};
		SCPTableProperty<GameSettingsSubsection> GameSettings;
		SCPTableProperty<CampaignSettingsSubsection> CampaignSettings;
		SCPTableProperty<HUDSettingsSubsection> HudSettings;
		SCPTableProperty<SexpSettingsSubsection> SexpSettings;
		SCPTableProperty<GraphicsSettingsSubsection> GraphicsSettings;
		SCPTableProperty<NetworkSettingsSubsection> NetworkSettings;
		SCPTableProperty<SoundSettingsSubsection> SoundSettings;
		SCPTableProperty<FredSettingsSubsection> FredSettings;
		SCPTableProperty<OtherSettingsSubsection> OtherSettings;
	};

	
} // namespace SCP

template<>
tl::optional<SCP::GameSettingsTable> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::GameSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::CampaignSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::HUDSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::SexpSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::GraphicsSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::NetworkSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::FredSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::SoundSettingsSubsection> construct(const SCPParsedTableData& InData);

template<>
tl::optional<SCP::OtherSettingsSubsection> construct(const SCPParsedTableData& InData);