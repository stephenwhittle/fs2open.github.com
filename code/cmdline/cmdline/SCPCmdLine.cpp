#include "SCPCmdLine.h"
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
		{ "connect", ParseIntoField(&SCPCmdLineOptions::AutoConnectAddress)},
		{"gamename", ParseIntoField(&SCPCmdLineOptions::MultiplayerGameName)},
		{"password", ParseIntoField(&SCPCmdLineOptions::MultiplayerGamePassword)},
		{"allowabove", ParseIntoField(&SCPCmdLineOptions::MultiplayerMinimumRank)},
		{"allowbelow", ParseIntoField(&SCPCmdLineOptions::MultiplayerMaximumRank)},
		{"standalone", SetFlag(&SCPCmdLineOptions::bStandalone, true)},
		{"nosound", SetFlag(&SCPCmdLineOptions::bNoSound, true)},
		{ "nomusic", SetFlag(&SCPCmdLineOptions::bNoSound, true)},
		{"no_enhanced_sound", SetFlag(&SCPCmdLineOptions::bNoEnhancedSound, true)},
		{"startgame", SetFlag(&SCPCmdLineOptions::bStartGame, true)},
		{"closed", SetFlag(&SCPCmdLineOptions::bClosedGame, true)},
		{"restricted", SetFlag(&SCPCmdLineOptions::bRestrictedGame, true)},
		{"port", ParseIntoField(&SCPCmdLineOptions::MultiplayerPort)},



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
	std::map<std::string, tl::optional<std::string>> Parameters = {};
	if (CmdLine.size() == 0)
	{
		return Parameters;
	}

	peg::Definition::Result Unused = CommandLineGrammar["Root"].parse_and_get_value(CmdLine.c_str(), ParseResults);
	for (auto Node : ParseResults->nodes)
	{
		
		if (Node->nodes.size() > 1)
		{
			Parameters[Node->nodes[0]->token] = tl::optional<std::string>(Node->nodes[1]->token);
		}
		else
		{
			Parameters[Node->nodes[0]->token] = tl::optional<std::string>();
		}
	}
	return Parameters;
}
