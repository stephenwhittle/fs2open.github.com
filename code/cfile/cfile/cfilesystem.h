/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on the 
 * source.
 *
*/ 



#ifndef _CFILESYSTEM_H
#define _CFILESYSTEM_H

#include "filesystem/SCPPath.h"


enum class SCPCFilePathTypeID
{
	Invalid = 0,
	Root = 1, // Root must be 1!!
	Data = 2,
	Maps = 3,
	Text = 4,
	Models = 5,
	Tables = 6,
	Sounds = 7,
	Sounds8B22K = 8,
	Sounds16B11K = 9,
	Voice = 10,
	VoiceBriefings = 11,
	VoiceCmdBrief = 12,
	VoiceDebriefings = 13,
	VoicePersonas = 14,
	VoiceSpecial = 15,
	VoiceTraining = 16,
	Music = 17,
	Movies = 18,
	Interface = 19,
	Font = 20,
	Effects = 21,
	HUD = 22,
	Players = 23,
	PlayerImages = 24,
	SquadImages = 25,
	SinglePlayers = 26,
	MultiPlayers = 27,
	Cache = 28,
	MultiCache = 29,
	Missions = 30,
	Config = 31,
	Demos = 32,
	CBAnims = 33,
	IntelAnims = 34,
	Scripts = 35,
	Fiction = 36,
	FredDocs = 37,
	InterfaceMarkup = 38,
	InterfaceCSS = 39

};









// Internal stuff
typedef struct cf_pathtype {
	int			index;					// To verify that the CF_TYPE define is correctly indexed into this array
	const char*	path;					// Path relative to FreeSpace root, has ending backslash.
	const char*	extensions;				// Extensions used in this pathtype, separated by spaces
	int			parent_index;			// Index of this directory's parent.  Used for creating directories when writing.
} cf_pathtype;


class SCPCFilePathType
{
public:
	SCPCFilePathTypeID Type;
	SCPPath Path;
	SCP_vector<SCP_string> Extensions;
	SCPCFilePathTypeID Parent;
};
//clang-format off
const std::map<SCPCFilePathTypeID, SCPCFilePathType> PathTypes = 
{
	{SCPCFilePathTypeID::Invalid,			{SCPCFilePathTypeID::Invalid,			SCPPath(),										{},										SCPCFilePathTypeID::Invalid} },
	{ SCPCFilePathTypeID::Root,				{SCPCFilePathTypeID::Root,				SCPPath(),										{".mve", ".ogg"},						SCPCFilePathTypeID::Root} },
	{ SCPCFilePathTypeID::Data,				{SCPCFilePathTypeID::Data,				SCPPath("data"),								{".cfg", ".txt"},						SCPCFilePathTypeID::Root} },
	{ SCPCFilePathTypeID::Maps,				{SCPCFilePathTypeID::Maps,				SCPPath("data") / "maps",						{".pcx", ".ani", ".eff", ".tga", ".jpg", ".png", ".dds"}, SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Text,				{SCPCFilePathTypeID::Maps,				SCPPath("data") / "text",						{".txt", ".net"},								SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Models,			{SCPCFilePathTypeID::Models,			SCPPath("data") / "models",						{".pof"},										SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Tables,			{SCPCFilePathTypeID::Tables,			SCPPath("data") / "tables",						{".tbl", ".tbm", ".lua"},						SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Sounds,			{SCPCFilePathTypeID::Sounds,			SCPPath("data") / "sounds",						{".wav", ".ogg"},								SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Sounds8B22K,		{SCPCFilePathTypeID::Sounds8B22K,		SCPPath("data") / "sounds" / "8b22k",			{".wav", ".ogg"},								SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Sounds16B11K,		{SCPCFilePathTypeID::Sounds16B11K,		SCPPath("data") / "sounds" / "16b11k",			{".wav", ".ogg"},								SCPCFilePathTypeID::Data} },
	{ SCPCFilePathTypeID::Voice,			{SCPCFilePathTypeID::Voice,				SCPPath("data") / "voice",						{},												SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::VoiceBriefings,	{SCPCFilePathTypeID::VoiceBriefings,	SCPPath("data") / "voice" / "briefing",			{".wav", ".ogg"},								SCPCFilePathTypeID::Voice	} },
	{ SCPCFilePathTypeID::VoiceCmdBrief,	{SCPCFilePathTypeID::VoiceCmdBrief,		SCPPath("data") / "voice" / "command_briefings",{".wav", ".ogg"},								SCPCFilePathTypeID::Voice	} },
	{ SCPCFilePathTypeID::VoiceDebriefings,	{SCPCFilePathTypeID::VoiceDebriefings,	SCPPath("data") / "voice" / "debriefing",		{".wav", ".ogg"},								SCPCFilePathTypeID::Voice	} },
	{ SCPCFilePathTypeID::VoicePersonas,	{SCPCFilePathTypeID::VoicePersonas,		SCPPath("data") / "voice" / "personas",			{".wav", ".ogg"},								SCPCFilePathTypeID::Voice	} },
	{ SCPCFilePathTypeID::VoiceSpecial,		{SCPCFilePathTypeID::VoiceSpecial,		SCPPath("data") / "voice" / "special",			{".wav", ".ogg"},								SCPCFilePathTypeID::Voice	} },
	{ SCPCFilePathTypeID::VoiceTraining,	{SCPCFilePathTypeID::VoiceTraining,		SCPPath("data") / "voice" / "training",			{".wav", ".ogg"},								SCPCFilePathTypeID::Voice	} },
	{ SCPCFilePathTypeID::Music,			{SCPCFilePathTypeID::Music,				SCPPath("data") / "music",						{".wav", ".ogg"},								SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Movies,			{SCPCFilePathTypeID::Movies,			SCPPath("data") / "movies",						{".mve", ".msb", ".ogg", ".mp4", ".srt", ".webm", ".png"},SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Interface,		{SCPCFilePathTypeID::Interface,			SCPPath("data") / "interface",					{".pcx", ".ani", ".dds", ".tga", ".eff", ".png", ".jpg"},	SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Font,				{SCPCFilePathTypeID::Font,				SCPPath("data") / "fonts",						{".vf", ".ttf"},								SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Effects,			{SCPCFilePathTypeID::Effects,			SCPPath("data") / "effects",					{".ani", ".eff", ".pcx", ".neb", ".tga", ".jpg", ".png", ".dds", ".sdr"},	SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::HUD,				{SCPCFilePathTypeID::HUD,				SCPPath("data") / "hud",						{".pcx", ".ani", ".eff", ".tga", ".jpg", ".png", ".dds"},	SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Players,			{SCPCFilePathTypeID::Players,			SCPPath("data") / "players",					{".hcf"},										SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::PlayerImages,		{SCPCFilePathTypeID::PlayerImages,		SCPPath("data") / "players" / "images",			{".pcx", ".png", ".dds"},						SCPCFilePathTypeID::Players	} },
	{ SCPCFilePathTypeID::SquadImages,		{SCPCFilePathTypeID::SquadImages,		SCPPath("data") / "players" / "squads",			{".pcx", ".png", ".dds"},						SCPCFilePathTypeID::Players	} },
	{ SCPCFilePathTypeID::SinglePlayers,	{SCPCFilePathTypeID::SinglePlayers,		SCPPath("data") / "players" / "single",			{".pl2", ".cs2", ".plr", ".csg", ".css", ".json"},	SCPCFilePathTypeID::Players	} },
	{ SCPCFilePathTypeID::MultiPlayers,		{SCPCFilePathTypeID::MultiPlayers,		SCPPath("data") / "players" / "multi",			{".plr", ".json"},								SCPCFilePathTypeID::Players	} },
	{ SCPCFilePathTypeID::Cache,			{SCPCFilePathTypeID::Cache,				SCPPath("data") / "cache",						{".clr", ".tmp", ".bx"},						SCPCFilePathTypeID::Data	} }, 	//clr=cached color
	{ SCPCFilePathTypeID::MultiCache,		{SCPCFilePathTypeID::MultiCache,		SCPPath("data") / "multidata",					{".pcx", ".png", ".dds", ".fs2", ".txt"},		SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Missions,			{SCPCFilePathTypeID::Missions,			SCPPath("data") / "missions",					{".fs2", ".fc2", ".ntl", ".ssv"},				SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Config,			{SCPCFilePathTypeID::Config,			SCPPath("data") / "config",						{".cfg", ".tbl", ".tbm", ".xml", ".csv"},		SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Demos,			{SCPCFilePathTypeID::Demos,				SCPPath("data") / "demos",						{".fsd"},										SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::CBAnims,			{SCPCFilePathTypeID::CBAnims,			SCPPath("data") / "cbanims",					{".pcx", ".ani", ".eff", ".tga", ".jpg", ".png", ".dds"},	SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::IntelAnims,		{SCPCFilePathTypeID::IntelAnims,		SCPPath("data") / "intelanims",					{".pcx", ".ani", ".eff", ".tga", ".jpg", ".png", ".dds"},	SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Scripts,			{SCPCFilePathTypeID::Scripts,			SCPPath("data") / "scripts",					{".lua", ".lc"},								SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::Fiction,			{SCPCFilePathTypeID::Fiction,			SCPPath("data") / "fiction",					{".txt"},										SCPCFilePathTypeID::Data	} },
	{ SCPCFilePathTypeID::FredDocs,			{SCPCFilePathTypeID::FredDocs,			SCPPath("data") / "freddocs",					{".html"},										SCPCFilePathTypeID::Data	}},
	{SCPCFilePathTypeID::InterfaceMarkup,	{SCPCFilePathTypeID::InterfaceMarkup,	SCPPath("data") / "interface" / "markup",		{".rml"},										SCPCFilePathTypeID::Interface	}},
	{SCPCFilePathTypeID::InterfaceCSS,		{SCPCFilePathTypeID::InterfaceCSS,		SCPPath("data") / "interface" / "css",			{".rcss"},										SCPCFilePathTypeID::Interface	}}
};
/*

// During cfile_init, verify that Pathtypes[n].index == n for each item
extern cf_pathtype Pathtypes[CF_MAX_PATH_TYPES];

bool cf_check_location_flags(uint32_t check_flags, uint32_t desired_flags);

// Returns the default storage path for files given a 
// particular pathtype.   In other words, the path to 
// the unpacked, non-cd'd, stored on hard drive path.
// If filename isn't null it will also tack the filename
// on the end, creating a completely valid filename.
// Input:   pathtype  - CF_TYPE_??
//			path_max  - Maximum characters in the path
//          filename  - optional, if set, tacks the filename onto end of path.
// Output:  path      - Fully qualified pathname.
//Returns 0 if result would be too long (invalid result)
int cf_create_default_path_string(char* path, uint path_max, int pathtype, const char* filename = nullptr,
                                  bool localize = false, uint32_t location_flags = CF_LOCATION_ALL, SCP_string LanguagePrefix = "");
int cf_create_default_path_string(SCP_string& path, int pathtype, const char* filename = nullptr, bool localize = false,
                                  uint32_t location_flags = CF_LOCATION_ALL, SCP_string LanguagePrefix = "");


*/



#endif	//_CFILESYSTEM_H
