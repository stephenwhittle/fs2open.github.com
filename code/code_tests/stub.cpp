#include "FSOutputDeviceBase.h"
#include "FSStdTypes.h"
#include "FSIntegerTypes.h"
#include "FSAssert.h"
#include "fmt/core.h"

class SCPConsoleOutputDevice : public FSOutputDeviceBase
{

protected:
	void PrintImpl(const char* ID, const char* Message) override
	{
		fmt::print(Message);
	}


	void AssertImpl(const char* Expression, const char* File, int Line, const char* Message) override
	{
		fmt::print(Expression);
		if (Message != nullptr) fmt::print(Message);
		throw;
	}


	void WarningImpl(const char* File, int Line, const char* Message) override
	{
		fmt::print(Message);
	}


	void ErrorImpl(const char* FileOrMessage, int Line = -1, const char* FormattedMessage = nullptr) override
	{
		if (FormattedMessage)
		{
			fmt::print("{}@L{} : {}", FileOrMessage, Line, FormattedMessage);
		}
		else
		{
			fmt::print(FileOrMessage);
		}
	}


	void MessageImpl(const char* Message) override
	{
		fmt::print(Message);
	}

public:
	void Init() override
	{
		std::ios::sync_with_stdio(false);
		fmt::print("Console Output Initialized");
	}


	void Close() override
	{
		fmt::print("Console Output Closed");
	}

};

FSOutputDeviceBase* GOutputDevice = new SCPConsoleOutputDevice();



int Directive_wait_time;
bool True_loop_argument_sexps;
bool Fixed_turret_collisions;
bool Damage_impacted_subsystem_first;
bool Cutscene_camera_displays_hud;
bool Alternate_chaining_behavior;
int Default_ship_select_effect;
int Default_weapon_select_effect;
int Default_fiction_viewer_ui;
bool Enable_external_shaders;
bool Enable_external_default_scripts;
int Default_detail_level;
bool Full_color_head_anis;
bool Weapons_inherit_parent_collision_group;
bool Flight_controls_follow_eyepoint_orientation;
int FS2NetD_port;
float Briefing_window_FOV;
bool Disable_hc_message_ani;
bool Red_alert_applies_to_delayed_ships;
bool Beams_use_damage_factors;
float Generic_pain_flash_factor;
float Shield_pain_flash_factor;
//gameversion::version Targetted_version; // Defaults to retail
SCP_string Window_title;
bool Unicode_text_mode;
bool Use_tabled_strings_for_default_language;
SCP_string Movie_subtitle_font;
bool Enable_scripts_in_fred; // By default FRED does not initialize the scripting system
SCP_string Window_icon_path;
bool Disable_built_in_translations;
bool Weapon_shockwaves_respect_huge;
bool Using_in_game_options;
float Dinky_shockwave_default_multiplier;
std::tuple<ubyte, ubyte, ubyte> Arc_color_damage_p1;
std::tuple<ubyte, ubyte, ubyte> Arc_color_damage_p2;
std::tuple<ubyte, ubyte, ubyte> Arc_color_damage_s1;
std::tuple<ubyte, ubyte, ubyte> Arc_color_emp_p1;
std::tuple<ubyte, ubyte, ubyte> Arc_color_emp_p2;
std::tuple<ubyte, ubyte, ubyte> Arc_color_emp_s1;
bool Use_engine_wash_intensity;
bool Cmdline_alternate_registry_path;
bool Cmdline_portable_mode;
char* Cmdline_mod = "blueplanetcomplete-1.0.10\0MVPS-4.2.3\0";


const char *stristr(const char *str, const char *substr)
{
	// check for null and insanity
	Assert(str);
	Assert(substr);
	if (str == NULL || substr == NULL || *substr == '\0')
		return NULL;

	// save both a lowercase and an uppercase version of the first character of substr
	char substr_ch_lower = (char)tolower(*substr);
	char substr_ch_upper = (char)toupper(*substr);

	// find the maximum distance to search
	const char *upper_bound = str + strlen(str) - strlen(substr);

	// loop through every character of str
	for (const char *start = str; start <= upper_bound; start++)
	{
		// check first character of substr
		if ((*start == substr_ch_upper) || (*start == substr_ch_lower))
		{
			// first character matched, so check the rest
			for (const char *str_ch = start+1, *substr_ch = substr+1; *substr_ch != '\0'; str_ch++, substr_ch++)
			{
				// character match?
				if (*str_ch == *substr_ch)
					continue;

				// converted character match?
				if (tolower(*str_ch) == tolower(*substr_ch))
					continue;

				// mismatch
				goto stristr_continue_outer_loop;
			}

			// finished inner loop with success!
			return start;
		}

stristr_continue_outer_loop:
		/* NO-OP */ ;
	}

	// no match
	return NULL;
}

// non-const version
char *stristr(char *str, const char *substr)
{
	// check for null and insanity
	Assert(str);
	Assert(substr);
	if (str == NULL || substr == NULL || *substr == '\0')
		return NULL;

	// save both a lowercase and an uppercase version of the first character of substr
	char substr_ch_lower = (char)tolower(*substr);
	char substr_ch_upper = (char)toupper(*substr);

	// find the maximum distance to search
	const char *upper_bound = str + strlen(str) - strlen(substr);

	// loop through every character of str
	for (char *start = str; start <= upper_bound; start++)
	{
		// check first character of substr
		if ((*start == substr_ch_upper) || (*start == substr_ch_lower))
		{
			// first character matched, so check the rest
			for (const char *str_ch = start+1, *substr_ch = substr+1; *substr_ch != '\0'; str_ch++, substr_ch++)
			{
				// character match?
				if (*str_ch == *substr_ch)
					continue;

				// converted character match?
				if (tolower(*str_ch) == tolower(*substr_ch))
					continue;

				// mismatch
				goto stristr_continue_outer_loop;
			}

			// finished inner loop with success!
			return start;
		}

stristr_continue_outer_loop:
		/* NO-OP */ ;
	}

	// no match
	return NULL;
}