#pragma once
#include "peglib.h"
#include "tl/optional.hpp"

class SCPCmdLineOptions;
class SCPCmdLineParser
{
	peg::Grammar CommandLineGrammar;
	std::shared_ptr<peg::Ast> ParseResults;

public:
	SCPCmdLineParser();
	std::map<std::string, tl::optional<std::string>> ParseCmdLine(int argc, char* const argv[]);
	const SCPCmdLineOptions GetOptions(int argc, char* const argv[]);
	std::map<std::string, std::function<void(SCPCmdLineOptions* ClassInstance, std::string InRawData)>> VariableParsers;

	std::map<std::string, std::function<void(std::string InRawData)>> RegisteredOptionHandlers;

	void RegisterOptionHandler(std::string Option, std::function<void(std::string InRawData)> Handler)
	{
		RegisteredOptionHandlers[Option] = Handler;
	}
};
enum class FlagGroup
{
	Default,
	HighMemFeaturesOff,
	HighMemFeaturesOn,
	AllOn,
	None
};

enum class OptionsCategory
{
	Graphics,
	HUD,
	GameSpeed,
	Gameplay,
	Audio,
	Launcher,
	Multiplayer,
	Troubleshooting,
	Experimental,
	DevTool,
	Misc
};

template<typename T>
class SCPCmdLineOption : public tl::optional<T>
{
	const char* Name;
	const char* HelpText;
	const bool FSOOnly;
	const FlagGroup Group;
	const OptionsCategory Category;

public:
	using tl::optional<T>::operator=;
	SCPCmdLineOption(const char* Name, const char* HelpText, bool FSOOnly, OptionsCategory Category = OptionsCategory::Misc, FlagGroup Group = FlagGroup::None)
		: tl::optional<T>(),
		Name(Name),
		HelpText(HelpText),
		FSOOnly(FSOOnly),
		Group(Group),
		Category(Category){};
	auto ParseIntoThis()
	{
		return[&this](SCPCmdLineOptions*, std::string InRawData)
		{
			*this = ParseOption<T>(InRawData);
		}
	}
};

//clang-format off
class SCPCmdLineOptions
{
public:
	//So long as you want all the options to be universally accessible these will be actual variables stored here rather than 
	//stored in their respective modules
	//Retail Options
	SCPCmdLineOption<std::string> AutoConnectAddress = { "Autoconnect Address", "Automatically connect to multiplayer IP:PORT", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<std::string> MultiplayerGameName = { "Game Name", "Set multiplayer game name", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<std::string> MultiplayerGamePassword = { "Game Password", "Set multiplayer game password", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<std::string> MultiplayerMinimumRank = { "Minimum Rank", "Ranks above this can join multi", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<std::string> MultiplayerMaximumRank = { "Maximum Rank", "Ranks below this can join multi", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> bStandalone = { "Standalone Mode", "Run as standalone server", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> bNoSound = { "Disable Sound", "Disable all sound", false, OptionsCategory::Audio };
	SCPCmdLineOption<bool> bNoMusic = { "Disable Music", "Disable music", false, OptionsCategory::Audio };
	SCPCmdLineOption<bool> bNoEnhancedSound = { "Disable Enhanced Sound", "Disable enhanced sound", false, OptionsCategory::Audio };
	SCPCmdLineOption<bool> bStartGame = { "Start Game", "Skip main menu and start hosting", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> bClosedGame = { "Closed Game", "Start hosted server as closed", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> bRestrictedGame = { "Restricted Mode", "Host must confirm all join requests", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<int> MultiplayerPort = { "Multiplayer Port", "Multiplayer network port", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> MultiLog = { "Multilog", "", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> bClientDamage = { "ClientDoDamage", "", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> bPofSpew = { "pofspew", "Dump model information to pofspew.txt", false, OptionsCategory::DevTool };
	SCPCmdLineOption<bool> bWeaponSpew = { "weaponspew", "Dump weapon stats and spreadsheets", false, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bMouseCoords = { "coords", "Show coordinates", false, OptionsCategory::DevTool };
	SCPCmdLineOption<int> MultiplayerTimeout = { "Multiplayer Timeout", "Multiplayer network timeout (seconds)", false, OptionsCategory::Multiplayer };
	SCPCmdLineOption<bool> b32BitMode = { "32bit", "deprecated", false, OptionsCategory::Misc };

	SCPCmdLineOption<float> FOV = { "Field Of View", "Vertical field-of-view factor", true, OptionsCategory::Graphics };
	SCPCmdLineOption<float> ClipDistance = { "Clip Distance", "Changes the distance from the viewport for the near-clipping plane", true, OptionsCategory::Graphics };
	SCPCmdLineOption<float> SpecStatic = { "Sun Specular Contribution", "Adjusts sun contribution to specular highlights", true, OptionsCategory::Graphics };
	SCPCmdLineOption<float> SpecPoint = { "Laser Specular Contribution", "Adjusts laser weapon contribution to specular highlights", true, OptionsCategory::Graphics };
	SCPCmdLineOption<float> SpecTube = {"Beam Weapon Specular Contribution", "Adjusts beam weapons contribution to specular highlights", true, OptionsCategory::Graphics};
	SCPCmdLineOption<float> AmbientFactor = { "Ambient light factor", "Adjusts ambient light applied to all parts of a ship", true, OptionsCategory::Graphics };
	SCPCmdLineOption<bool> bNoEnv         = {"No env maps", "deprecated", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bNoGlow         = {"No glow", "deprecated", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bNoMotionDebris = { "No motion debris", "Removes floating debris", true, OptionsCategory::Graphics };
	SCPCmdLineOption<bool> bNoScaleVid = { "No video scale", "Do not scale videos to fit the screen resolution", true, OptionsCategory::Graphics };
	SCPCmdLineOption<bool> bNoSpec = { "No specular","deprecated", true, OptionsCategory::Graphics };
	SCPCmdLineOption<bool> bEmissive = { "Emissive", "Enable emissive light from ships", true, OptionsCategory::Graphics };
	SCPCmdLineOption<bool> bNoNormals = { "No normals", "Disable normal mapping", true, OptionsCategory::Graphics };
	SCPCmdLineOption<bool> bNoHeight   = {"No height", "Disable height mapping", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> b3dShockwave   = {"3d shockwave", "Enable 3d shockwave", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bSoftParticles      = {"Soft particles", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bNoPostProcess = {"No post-processing", "Disables post-processing", true, OptionsCategory::Graphics};
	SCPCmdLineOption<int> BloomIntensity = {"Bloom Intensity", "Bloom intensity (requires post-processing)", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bPostAA = {"Enable Anti-aliasing", "Enable post-process AA", true, OptionsCategory::Graphics};
	SCPCmdLineOption<int> AAPreset = {"AA Preset", "Sets AA type/level, see wiki for values", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bFXAA	= {"Enable FXAA", "deprecated", true, OptionsCategory::Graphics};
	SCPCmdLineOption<int> FXAAPreset	= {"FXAA level", "deprecated", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bSMAA = {"Enable SMAA", "deprecated", true, OptionsCategory::Graphics};
	SCPCmdLineOption<int> SMAAPreset	= {"SMAA level", "deprecated", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bFBExplosions = {"Framebuffer Explosions", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bFBThrusters	= {"Framebuffer Thrusters", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bNoLightshafts	= {"No lightshafts", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<int> ShadowQuality = {"Shadow Quality", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bEnableShadows = {"Enable Shadows", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<bool> bNoDeferred = {"Disable deferred lighting", "", true, OptionsCategory::Graphics};
	SCPCmdLineOption<int> AnisotropyLevel = {"Anisotropy Level", "", true, OptionsCategory::Graphics};

	SCPCmdLineOption<bool> bDisableFPSCap = { "Disable FSP limit", "", true, OptionsCategory::GameSpeed };
	SCPCmdLineOption<bool> bDisableVSync = {"Disable VSync", "", true, OptionsCategory::GameSpeed};

	SCPCmdLineOption<bool> bEnableBallisticGauge = {"Enable Ballistic Gauge", "", true, OptionsCategory::HUD};
	SCPCmdLineOption<bool> bEnableDualScanLines = {"Enable Dual Scanlines", "", true, OptionsCategory::HUD};
	SCPCmdLineOption<bool> bEnableOrbRadar = {"Enable Orb Radar", "", true, OptionsCategory::HUD};
	SCPCmdLineOption<bool> bEnableRearmTimer = {"Enable Rearm Timer", "", true, OptionsCategory::HUD};
	SCPCmdLineOption<bool> bEnableTargetInfo = {"Enable Target Info", "", true, OptionsCategory::HUD};

	SCPCmdLineOption<bool> bUse3dWarp = {"Use 3d warp", "", true, OptionsCategory::Gameplay};
	SCPCmdLineOption<bool> b3dShipChoice = {"Show 3d ship choice", "", true, OptionsCategory::Gameplay };
	SCPCmdLineOption<bool> b3dWeaponChoice = {"Show 3d weapon choice", "", true, OptionsCategory::Gameplay };
	SCPCmdLineOption<bool> bEnableWarpFlash = {"Enable warp flash FX", "", true, OptionsCategory::Gameplay };
	SCPCmdLineOption<bool> bDisableAutopilotInterrupt = {"Allow interrupting autopilot", "", true, OptionsCategory::Gameplay };
	SCPCmdLineOption<bool> bStretchMenuToFit = {"Stretch menu to fit screen", "", true, OptionsCategory::Gameplay };

	SCPCmdLineOption<bool> bEnableVoiceRecognition = {"Enable voice recognition", "", true, OptionsCategory::Audio};

	SCPCmdLineOption<std::string> ModList = {"Mods", "List of folders to overlay over default data", true, OptionsCategory::Misc};

	SCPCmdLineOption<std::string> AutoloadMission = {"Autoload multiplayer mission", "Name of mission to automatically load for multiplayer", true, OptionsCategory::Multiplayer};
	SCPCmdLineOption<bool> bAllowIngameJoin = {"In-game join", "", true, OptionsCategory::Multiplayer};
	SCPCmdLineOption<bool> bDisallowReturn = {"Disallow return", "Hides 'Return to flight deck' option in respawn dialog", true, OptionsCategory::Multiplayer};
	SCPCmdLineOption<int> ObjectUpdateCap = {"Object update cap", "Cap multiplayer object updates (0-3)", true, OptionsCategory::Multiplayer};
	SCPCmdLineOption<std::string> GatewayIP = {"Gateway IP address", "", true, OptionsCategory::Multiplayer};

	SCPCmdLineOption<bool> bEnablePortableMode = {"Enable portable mode", "", true, OptionsCategory::Launcher};

	SCPCmdLineOption<bool> bLoadAllWeapons = {"Load all weapons", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bNoMovies = {"No Movies", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bNoSetGamma= {"Don't set Gamma", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bDisableFBO = {"Disable FBOs", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bDisablePBO = {"Disable PBOs", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bMipmap= {"Mipmaps", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bATISwap= {"ATI Swap", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bNo3dSound= {"No 3d sound", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bUseglDrawElements= {"Use glDrawElements", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<std::string> KeyboardLayout = {"Keyboard Layout", "Specify keyboard layout (\"qwertz\" or \"azerty\")", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bglFinish= {"glFinish", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bNoGeoEffects = {"Disable Geometry Effects", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bSetCPUAffinity = {"Set CPU Affinity", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bNoGrab = {"No Grab", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bNoShaderCache = {"No Shader Cache", "", true, OptionsCategory::Troubleshooting};
	SCPCmdLineOption<bool> bFixRegistry = {"Fix Registry (win32)", "", true, OptionsCategory::Troubleshooting};

	SCPCmdLineOption<std::string> AutoStartMission = {"Start Mission", "Skip mainhall and run this mission", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bDisableCollisions = {"Disable Collisions", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bDisableWeapons = {"Disable Weapons", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bNoParseErrors = {"No Parse Errors", "Disables parser error notification", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bEnableExtraWarnings = {"Enable extra warnings", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bShowFPS = {"Show FPS", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bShowBMPManUsage= {"Show BMPMan memory usage", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bShowPos= {"Show Position", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bShowStats = {"Show Stats", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bSaveRenderTarget = {"Save Render Targets", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bWindowed = {"Windowed Mode", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bWindowedFullscreen= {"Windowed Fullscreen mode", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<std::string> Resolution = {"Resolution", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<std::string> CenterMonitorResolution = {"Center Monitor Resolution", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bVerifyVPs= {"Verify VPs", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bParseCmdlineOnly = {"Parse cmd line args only", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bReparseMainhall = {"Reparse Mainhall", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bProfileWriteFile = {"Frame Profile output", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bNoUnfocusedPause = {"No Unfocused Pause", "Don't pause the game when it loses focus", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bBenchmarkMode = {"Benchmark Mode", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bNonInteractiveMode = {"Non Interactive Mode", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bJSONProfiling = {"Profile JSON", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bShowVideoInfo = {"Show Video Info", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bProfileFrameTime = {"Profile Frame Time", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bEnableDebugWindow = {"Enable Debug Window", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bEnableGraphicsDebugOutput = {"Enable Graphics debug output", "", true, OptionsCategory::DevTool};
	SCPCmdLineOption<bool> bLogToStdout = {"Log to stdout", "", true, OptionsCategory::DevTool};

	SCPCmdLineOption<std::string> FlagsFilePath = {"Output launcher flags", "Save the launcher flags to file", true, OptionsCategory::Misc};
	SCPCmdLineOption<bool> bOutputSEXPs = { "Output SEXPs", "Saves all SEXPs to sexps.html", true, OptionsCategory::Misc };
	SCPCmdLineOption<bool> bOutputScripting = {"Output Scripting", "", true, OptionsCategory::Misc};
	SCPCmdLineOption<bool> bOutputScriptingJSON = {"Output Scripting JSON", "", true, OptionsCategory::Misc};

};

template <typename T>
tl::optional<T> ParseOption(std::string InRawData);


template <>
tl::optional<std::string> ParseOption(std::string InRawData);



template <typename T>
auto ParseIntoField(SCPCmdLineOption<T> SCPCmdLineOptions::*Field)
{
	return [Field](SCPCmdLineOptions* ClassInstance, std::string InRawData) {
		ClassInstance->*Field = ParseOption<T>(InRawData);
	};
}

auto SetFlag(SCPCmdLineOption<bool> SCPCmdLineOptions::*Field, bool FlagValue)
{
	return [Field, FlagValue](SCPCmdLineOptions* ClassInstance, std::string InRawData) {
		ClassInstance->*Field = FlagValue;
	};
}