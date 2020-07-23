#pragma once
//needs to go into core_interface
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


//put in map of default path etc here?