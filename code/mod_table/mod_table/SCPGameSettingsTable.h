#pragma once

#include "parse/SCPTable.h"

namespace SCP
{
	

	class GameSettingsSubsection : public SCPTableBase<GameSettingsSubsection>
	{
	public:
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
		SCPTableProperty<SCP_string> DefaultCampaignFileName;
		SCPTableProperty<SCP_vector<SCP_string>> IgnoredCampaignFileNames;
		SCPTableProperty<SCP_vector<SCP_string>> IgnoredMissionFileNames;
		SCPTableProperty<bool> RedAlertAppliedToDelayedShips;
	};

	class HUDSettingsSubsection : public SCPTableBase<HUDSettingsSubsection>
	{
	public:
		SCPTableProperty<uint32_t> DirectiveWaitTime;
		SCPTableProperty<bool> CutsceneCameraDisplaysHUD;
		SCPTableProperty<bool> FullColorHeadAnimations;
	};

	class SexpSettingsSubsection : public SCPTableBase<SexpSettingsSubsection>
	{
	public:
		SCPTableProperty<bool> LoopSexpThenArgs;
		SCPTableProperty<bool> UseAlternateChainingBehaviour;
	};


	class GameSettingsTable : public SCPTableBase<GameSettingsTable>
	{
	public:
		SCPTableProperty<GameSettingsSubsection> GameSettings;
		SCPTableProperty<CampaignSettingsSubsection> CampaignSettings;
		SCPTableProperty<HUDSettingsSubsection> HudSettings;
		SCPTableProperty<SexpSettingsSubsection> SexpSettings;
		SCPTableProperty<GraphicsSettingsSubsection> GraphicsSettings;
		SCPTableProperty<NetworkSettingsSubsection> NetworkSettings;
		SCPTableProperty<FredSettingsSubsection> FredSettings;
		SCPTableProperty<OtherSettingsSubsection> OtherSettings;
	};
}