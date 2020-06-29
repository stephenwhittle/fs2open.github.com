#pragma once
#include "peglib.h"
#include "tl/optional.hpp"
class SCPCmdLineParser
{
	peg::Grammar CommandLineGrammar;
	std::shared_ptr<peg::Ast> ParseResults;

public:
	SCPCmdLineParser();
	std::map<std::string, tl::optional<std::string>> ParseCmdLine(int argc, char* const argv[]);
	
	std::map<std::string, std::function<void(SCPCmdLineOptions* ClassInstance, std::string InRawData)>> VariableParsers;

};



class SCPCmdLineOptions
{
public:
	//So long as you want all the options to be universally accessible these will be actual variables stored here rather than 
	//stored in their respective modules

	tl::optional<std::string> AutoConnectAddress;
	tl::optional<std::string> MultiplayerGameName;
	tl::optional<std::string> MultiplayerGamePassword;
	tl::optional<std::string> MultiplayerMinimumRank;
	tl::optional<std::string> MultiplayerMaximumRank;
	tl::optional<bool> bStandalone;
	tl::optional<bool> bNoSound;
	tl::optional<bool> bNoMusic;
	tl::optional<bool> bNoEnhancedSound;
	tl::optional<bool> bStartGame;
	tl::optional<bool> bClosedGame;
	tl::optional<bool> bRestrictedGame;
	tl::optional<int> MultiplayerPort;
	tl::optional<bool> MultiLog;
	tl::optional<bool> bClientDamage;
	tl::optional<bool> bPofSpew;
	tl::optional<bool> bWeaponSpew;
	tl::optional<bool> bMouseCoords;
	tl::optional<int> MultiplayerTimeout;
	tl::optional<bool> b32BitMode;

};

template <typename T>
tl::optional<T> ParseOption(std::string InRawData)
{
	return tl::optional<T>();
}

template<>
tl::optional<std::string> ParseOption(std::string InRawData)
{
	return InRawData;
}


template < typename T>
auto ParseIntoField(tl::optional<T> SCPCmdLineOptions::*Field)
{
	return [Field](SCPCmdLineOptions* ClassInstance, std::string InRawData) {
		ClassInstance->*Field = ParseOption<T>(InRawData);
	};
}

auto SetFlag(tl::optional<bool> SCPCmdLineOptions::* Field, bool FlagValue)
{
	return[Field, FlagValue](SCPCmdLineOptions* ClassInstance, std::string InRawData) {
		ClassInstance->*Field = FlagValue;
	};
}