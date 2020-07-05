#include "SCPCmdLine.h"
#include "SCPStringUtils.h"
#include <algorithm>
SCPCmdLineParser::SCPCmdLineParser()
{
	CommandLineGrammar["ParamName"] <= peg::seq(peg::chr('-'), peg::tok(peg::oom(peg::ncls("-\r\n\t "))));
	CommandLineGrammar["ParamName"].name = "ParamName";
	CommandLineGrammar["ParamValue"] <= peg::tok(peg::cho(peg::seq(peg::chr('"'), peg::zom(peg::ncls("\"")), peg::chr('"')), peg::seq(peg::npd(peg::chr('-')), peg::oom(peg::ncls("\r\n ")))));
	CommandLineGrammar["ParamValue"].name = "ParamValue";
	CommandLineGrammar["Param"] <=
		peg::seq(CommandLineGrammar["ParamName"], peg::opt(CommandLineGrammar["ParamValue"]));
	CommandLineGrammar["Param"].name = "Param";
	CommandLineGrammar["Root"] <= peg::zom(CommandLineGrammar["Param"]);
	CommandLineGrammar["Root"].name = "Root";
	CommandLineGrammar[peg::WHITESPACE_DEFINITION_NAME] <= peg::zom(peg::cls(" \t\r\n"));
	CommandLineGrammar[peg::WHITESPACE_DEFINITION_NAME].name = peg::WHITESPACE_DEFINITION_NAME;

	for (auto& Rule : CommandLineGrammar) {
		if (Rule.second.name.empty()) {
			Rule.second.name = Rule.first;
		}
		peg::add_ast_action(Rule.second);
	}
	CommandLineGrammar["Root"].whitespaceOpe = peg::wsp(CommandLineGrammar[peg::WHITESPACE_DEFINITION_NAME].get_core_operator());

	//this will need to include the other information regarding the parsers I guess
	//clang-format off
	VariableParsers = {
		{"connect", ParseIntoField(&SCPCmdLineOptions::AutoConnectAddress)},
		{"gamename", ParseIntoField(&SCPCmdLineOptions::MultiplayerGameName)},
		{"password", ParseIntoField(&SCPCmdLineOptions::MultiplayerGamePassword)},
		{"allowabove", ParseIntoField(&SCPCmdLineOptions::MultiplayerMinimumRank)},
		{"allowbelow", ParseIntoField(&SCPCmdLineOptions::MultiplayerMaximumRank)},
		{"standalone", SetFlag(&SCPCmdLineOptions::bStandalone, true)},
		{"nosound", SetFlag(&SCPCmdLineOptions::bNoSound, true)},
		{"nomusic", SetFlag(&SCPCmdLineOptions::bNoSound, true)},
		{"no_enhanced_sound", SetFlag(&SCPCmdLineOptions::bNoEnhancedSound, true)},
		{"startgame", SetFlag(&SCPCmdLineOptions::bStartGame, true)},
		{"closed", SetFlag(&SCPCmdLineOptions::bClosedGame, true)},
		{"restricted", SetFlag(&SCPCmdLineOptions::bRestrictedGame, true)},
		{"port", ParseIntoField(&SCPCmdLineOptions::MultiplayerPort)},
		{"multilog", SetFlag(&SCPCmdLineOptions::MultiLog, true)},
		{"clientdamage", SetFlag(&SCPCmdLineOptions::bClientDamage, true)},
		{"pofspew", SetFlag(&SCPCmdLineOptions::bPofSpew, true)},
		{"weaponspew", SetFlag(&SCPCmdLineOptions::bWeaponSpew, true)},
		{"coords", SetFlag(&SCPCmdLineOptions::bMouseCoords, true)},
		{"timeout", ParseIntoField(&SCPCmdLineOptions::MultiplayerTimeout)},
		
		{"fov", ParseIntoField(&SCPCmdLineOptions::FOV)},
		{"clipdist", ParseIntoField(&SCPCmdLineOptions::ClipDistance)},
		{"spec_static", ParseIntoField(&SCPCmdLineOptions::SpecStatic)},
		{"spec_point", ParseIntoField(&SCPCmdLineOptions::SpecPoint)},
		{"spec_tube", ParseIntoField(&SCPCmdLineOptions::SpecTube)},
		{"ambient_factor", ParseIntoField(&SCPCmdLineOptions::AmbientFactor)},
		{"noenv", SetFlag(&SCPCmdLineOptions::bNoEnv, true)},
		{"noglow", SetFlag(&SCPCmdLineOptions::bNoGlow, true)},
		{"nomotiondebris", SetFlag(&SCPCmdLineOptions::bNoMotionDebris, true)},
		{"emissive_light", SetFlag(&SCPCmdLineOptions::bEmissive, true)},
		{"nonormal", SetFlag(&SCPCmdLineOptions::bNoNormals, true)},
		{"noheight", SetFlag(&SCPCmdLineOptions::bNoHeight, true)},
		{"3dshockwave", SetFlag(&SCPCmdLineOptions::b3dShockwave, true)},
		{"soft_particles", SetFlag(&SCPCmdLineOptions::bSoftParticles, true)},
		{"no_post_process", SetFlag(&SCPCmdLineOptions::bNoPostProcess, true)},
		{"bloom_intensity", ParseIntoField(&SCPCmdLineOptions::BloomIntensity)},
		{"aa", SetFlag(&SCPCmdLineOptions::bPostAA, true)},
		{"aa_preset", ParseIntoField(&SCPCmdLineOptions::AAPreset)},
		{"fxaa", SetFlag(&SCPCmdLineOptions::bFXAA, true)},
		{"fxaa_preset", ParseIntoField(&SCPCmdLineOptions::FXAAPreset)},
		{"smaa", SetFlag(&SCPCmdLineOptions::bSMAA, true)},
		{"smaa_preset", ParseIntoField(&SCPCmdLineOptions::SMAAPreset)},
		{"fb_explosions", SetFlag(&SCPCmdLineOptions::bFBExplosions, true)},
		{"fb_thrusters", SetFlag(&SCPCmdLineOptions::bFBThrusters, true)},
		{"nolightshafts", SetFlag(&SCPCmdLineOptions::bNoLightshafts, true)},
		{"shadow_quality", ParseIntoField(&SCPCmdLineOptions::ShadowQuality)},
		{"enable_shadows", SetFlag(&SCPCmdLineOptions::bEnableShadows, true)},
		{"no_deferred", SetFlag(&SCPCmdLineOptions::bNoDeferred, true)},
		{"anisotropic_filter", ParseIntoField(&SCPCmdLineOptions::AnisotropyLevel)},

		{"no_fps_capping", SetFlag(&SCPCmdLineOptions::bDisableFPSCap, true)},
		{"no_vsync", SetFlag(&SCPCmdLineOptions::bDisableVSync, true)},

		{"ballistic_gauge", SetFlag(&SCPCmdLineOptions::bEnableBallisticGauge, true)},
		{"dualscanlines", SetFlag(&SCPCmdLineOptions::bEnableDualScanLines, true)},
		{"orbradar", SetFlag(&SCPCmdLineOptions::bEnableOrbRadar, true)},
		{"rearm_timer", SetFlag(&SCPCmdLineOptions::bEnableRearmTimer, true)},
		{"targetinfo", SetFlag(&SCPCmdLineOptions::bEnableTargetInfo, true)},

		{"3dwarp", SetFlag(&SCPCmdLineOptions::bUse3dWarp, true)},
		{"ship_choice_3d", SetFlag(&SCPCmdLineOptions::b3dShipChoice, true)},
		{"weapon_choice_3d", SetFlag(&SCPCmdLineOptions::b3dWeaponChoice, true)},
		{"warp_flash", SetFlag(&SCPCmdLineOptions::bEnableWarpFlash, true)},
		{"no_ap_interrupt", SetFlag(&SCPCmdLineOptions::bDisableAutopilotInterrupt, true)},
		{"stretch_menu", SetFlag(&SCPCmdLineOptions::bStretchMenuToFit, true)},

		{"voicer", SetFlag(&SCPCmdLineOptions::bEnableVoiceRecognition, true)},

		{"mod", ParseIntoField(&SCPCmdLineOptions::ModList)},

		{"almission", ParseIntoField(&SCPCmdLineOptions::AutoloadMission)},
		{"ingame_join", SetFlag(&SCPCmdLineOptions::bAllowIngameJoin, true)},
		{"mpnoreturn", SetFlag(&SCPCmdLineOptions::bDisallowReturn, true)},
		{"cap_object_update", ParseIntoField(&SCPCmdLineOptions::ObjectUpdateCap)},
		{"gateway_ip", ParseIntoField(&SCPCmdLineOptions::GatewayIP)},

		{"portable_mode", SetFlag(&SCPCmdLineOptions::bEnablePortableMode, true)},

		{"loadallweps", SetFlag(&SCPCmdLineOptions::bLoadAllWeapons, true)},
		{"nomovies", SetFlag(&SCPCmdLineOptions::bNoMovies, true)},
		{"no_set_gamma", SetFlag(&SCPCmdLineOptions::bNoSetGamma, true)},
		{"disable_fbo", SetFlag(&SCPCmdLineOptions::bDisableFBO, true)},
		{"disable_pbo", SetFlag(&SCPCmdLineOptions::bDisablePBO, true)},
		{"mipmap", SetFlag(&SCPCmdLineOptions::bMipmap, true)},
		{"ati_swap", SetFlag(&SCPCmdLineOptions::bATISwap, true)},
		{"no_3d_sound", SetFlag(&SCPCmdLineOptions::bNo3dSound, true)},
		{"use_gldrawelements", SetFlag(&SCPCmdLineOptions::bUseglDrawElements, true)},
		{"keyboard_layout", ParseIntoField(&SCPCmdLineOptions::KeyboardLayout)},
		{"gl_finish", SetFlag(&SCPCmdLineOptions::bglFinish, true)},
		{"no_geo_effects", SetFlag(&SCPCmdLineOptions::bNoGeoEffects, true)},
		{"set_cpu_affinity", SetFlag(&SCPCmdLineOptions::bSetCPUAffinity, true)},
		{"nograb", SetFlag(&SCPCmdLineOptions::bNoGrab, true)},
		{"noshadercache", SetFlag(&SCPCmdLineOptions::bNoShaderCache, true)},
		{"fix_registry", SetFlag(&SCPCmdLineOptions::bFixRegistry, true)},
		
		{"start_mission", ParseIntoField(&SCPCmdLineOptions::AutoStartMission)},
		{"dis_collisions", SetFlag(&SCPCmdLineOptions::bDisableCollisions, true)},
		{"dis_weapons", SetFlag(&SCPCmdLineOptions::bDisableWeapons, true)},
		{"noparseerrors", SetFlag(&SCPCmdLineOptions::bNoParseErrors, true)},
		{"extra_warn", SetFlag(&SCPCmdLineOptions::bEnableExtraWarnings, true)},
		{"fps", SetFlag(&SCPCmdLineOptions::bShowFPS, true)},
		{"bmpmanuseage", SetFlag(&SCPCmdLineOptions::bShowBMPManUsage, true)},
		{"pos", SetFlag(&SCPCmdLineOptions::bShowPos, true)},
		{"stats", SetFlag(&SCPCmdLineOptions::bShowStats, true)},
		{"save_render_target", SetFlag(&SCPCmdLineOptions::bSaveRenderTarget, true)},
		{"window", SetFlag(&SCPCmdLineOptions::bWindowed, true)},
		{"fullscreen_window", SetFlag(&SCPCmdLineOptions::bWindowedFullscreen, true)},
		{"res", ParseIntoField(&SCPCmdLineOptions::Resolution)},
		{"center_res", ParseIntoField(&SCPCmdLineOptions::CenterMonitorResolution)},
		{"verify_vps", SetFlag(&SCPCmdLineOptions::bVerifyVPs, true)},
		{"parse_cmdline_only", SetFlag(&SCPCmdLineOptions::bParseCmdlineOnly, true)},
		{"reparse_mainhall", SetFlag(&SCPCmdLineOptions::bReparseMainhall, true)},
		{"profile_write_file", SetFlag(&SCPCmdLineOptions::bProfileWriteFile, true)},
		{"no_unfocused_pause", SetFlag(&SCPCmdLineOptions::bNoUnfocusedPause, true)},
		{"benchmark_mode", SetFlag(&SCPCmdLineOptions::bBenchmarkMode, true)},
		{"noninteractive", SetFlag(&SCPCmdLineOptions::bNonInteractiveMode, true)},
		{"json_profiling", SetFlag(&SCPCmdLineOptions::bJSONProfiling, true)},
		{"show_video_info", SetFlag(&SCPCmdLineOptions::bShowVideoInfo, true)},
		{"profile_frame_time", SetFlag(&SCPCmdLineOptions::bProfileFrameTime, true)},
		{"debug_window", SetFlag(&SCPCmdLineOptions::bEnableDebugWindow, true)},
		{"gr_debug", SetFlag(&SCPCmdLineOptions::bEnableGraphicsDebugOutput, true)},
		{"stdout_log", SetFlag(&SCPCmdLineOptions::bLogToStdout, true)},
		{"get_flags", ParseIntoField(&SCPCmdLineOptions::FlagsFilePath)},
		{"output_sexps", SetFlag(&SCPCmdLineOptions::bOutputSEXPs, true)},
		{"output_scripting", SetFlag(&SCPCmdLineOptions::bOutputScripting, true)},
		{"output_script_json", SetFlag(&SCPCmdLineOptions::bOutputScriptingJSON, true)}
	};


}

 std::map<std::string, tl::optional<std::string>> SCPCmdLineParser::ParseCmdLine(int argc, char* const argv[])
{
	std::string CmdLine;
	for (int ArgIndex = 1; ArgIndex < argc; ArgIndex++)
	{
		
		CmdLine.append(argv[ArgIndex]);
		CmdLine.append(" ");
	}

	if (CmdLine.size() == 0)
	{
		return {};
	}
	else
	{
		return ParseCmdLineInternal(CmdLine);
	}

}

std::map<std::string, tl::optional<std::string>> SCPCmdLineParser::ParseCmdLine(std::vector<std::string> Args)
{
	if (Args.size() == 0)
	{
		return {};
	}

	std::string CmdLine;
	for (auto Arg : Args)
	{
		CmdLine.append(Arg);
		CmdLine.append(" ");
	}
	return ParseCmdLineInternal(CmdLine);
}

std::map<std::string, tl::optional<std::string>>
SCPCmdLineParser::ParseCmdLineInternal(std::string& CmdLine)
{
	std::map<std::string, tl::optional<std::string>> Parameters;
	peg::Definition::Result Unused = CommandLineGrammar["Root"].parse_and_get_value(CmdLine.c_str(), ParseResults);
	for (auto Node : ParseResults->nodes) {

		if (Node->nodes.size() > 1) {
			Parameters[Node->nodes[0]->token] = tl::optional<std::string>(Node->nodes[1]->token);
		} else {
			Parameters[Node->nodes[0]->token] = tl::optional<std::string>();
		}
	}
	return Parameters;
}

//common option parser specializations

template <>
tl::optional<std::string> ParseOption(std::string InRawData)
{
	return InRawData;
}

template<>
tl::optional<bool> ParseOption(std::string InRawData)
{
	std::string SanitizedData = StringUtils::ToLower(StringUtils::TrimWhitespace(InRawData));
	
	switch (peg::str2tag(SanitizedData.c_str()))
	{
	case peg::str2tag("true"):
	case peg::str2tag("yes"):
	case peg::str2tag("on"):
		return true;
	case peg::str2tag("false"):
	case peg::str2tag("no"):
	case peg::str2tag("off"):
		return false;
	default:
		return {};
	}
}

template<>
tl::optional<float> ParseOption(std::string InRawData)
{
	size_t Length = 0;
	float Value = std::stof(InRawData, &Length);
	if (Length > 0)
	{
		return Value;
	}
	else
	{
		return {};
	}
}

template <>
tl::optional<int> ParseOption(std::string InRawData)
{
	size_t Length = 0;
	int Value   = std::stoi(InRawData, &Length);
	if (Length > 0) {
		return Value;
	} else {
		return {};
	}
}

template <typename T>
tl::optional<T> ParseOption(std::string InRawData)
{
	return tl::optional<T>();
}


const SCPCmdLineOptions SCPCmdLineParser::GetOptions(int argc, char* const argv[])
{
	auto ParsedValues = ParseCmdLine(argc, argv);
	SCPCmdLineOptions Options;
	for (auto ParsedValue : ParsedValues) {

		auto Handler = VariableParsers.find(ParsedValue.first);
		if (Handler != VariableParsers.end()) {
			if (ParsedValue.second.has_value()) {
				(*Handler).second(&Options, *ParsedValue.second);
			} else {
				(*Handler).second(&Options, "");
			}
		}
	}
	return Options;
}
const SCPCmdLineOptions SCPCmdLineParser::GetOptions(std::vector<std::string> Args)
{
	auto ParsedValues = ParseCmdLine(Args);
	SCPCmdLineOptions Options;
	for (auto ParsedValue : ParsedValues) {

		auto Handler = VariableParsers.find(ParsedValue.first);
		if (Handler != VariableParsers.end()) {
			if (ParsedValue.second.has_value()) {
				(*Handler).second(&Options, *ParsedValue.second);
			} else {
				(*Handler).second(&Options, "");
			}
		}
	}
	return Options;
}