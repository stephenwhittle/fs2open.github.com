#pragma once

#include "peglib.h"
#include "tl/optional.hpp"
#include "FSStdTypes.h"
/// <summary>
/// Concatenate two grammar symbols into a sequence
/// Overloads are included to prepend and append symbols into a sequence, as well as concatenating two sequences into a
/// new sequence
/// </summary>
/// <param name="a">The first symbol or sequence</param>
/// <param name="b">The second symbol or sequence</param>
/// <returns>A new sequence containing elements from both A and B, in that order</returns>
std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Sequence>& a, const std::shared_ptr<peg::Ope>& b);
std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Ope>& a, const std::shared_ptr<peg::Sequence>& b);
std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Sequence>& a,
										 const std::shared_ptr<peg::Sequence>& b);
std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Ope>& a, const std::shared_ptr<peg::Ope>& b);

/// <summary>
/// Object containing a grammar describing the file format for a modular table
/// </summary>
class SCPTableFormatDescriptor
{
	peg::Grammar g;

public:
	SCPTableFormatDescriptor();

	// PEG element matching a literal string (case insensitive)
	auto Literal(const std::string& String)
	{
		return std::static_pointer_cast<peg::Ope>(std::make_shared<peg::LiteralString>(String, true));
	}
	auto LiteralToken(const std::string& String)
	{
		return peg::seq(peg::ign(g[peg::WHITESPACE_DEFINITION_NAME]),
						std::make_shared<peg::LiteralString>(String, true),
						peg::ign(g[peg::WHITESPACE_DEFINITION_NAME]));
	}
	// PEG element matching a single character
	auto Character(char Char)
	{
		return std::static_pointer_cast<peg::Ope>(std::make_shared<peg::Character>(Char));
	}
	static std::string GetVariableName(const std::string RawName)
	{
		return "Var_" + RawName;
	}
	static std::string GetSectionName(const std::string RawName)
	{
		return "Section_" + RawName;
	}
	static std::string GetLiteralName(const std::string RawName)
	{
		return "Literal_" + RawName;
	}
	// Matches an optional 'VarName : Value' pair. Silently continues if missing
	std::shared_ptr<peg::Ope> OptionalVariable(const std::string& VariableName);
	// Matches a 'VarName : Value' pair. If not present the parsing will fail
	std::shared_ptr<peg::Ope> RequiredVariable(const std::string& VariableName);
	// Matches 'VarName :\r\n'
	std::shared_ptr<peg::Ope> ObjectHeader(const std::string& Name);
	// Matches '$End VarName'
	std::shared_ptr<peg::Ope> EndObject(const std::string& Name = "");

	// Optionally matches the enclosed operand
	std::shared_ptr<peg::Ope> Optional(const std::shared_ptr<peg::Ope>& Entry);

	// Matches the first valid element of a list of alternatives. Fails if none of them match
	template<typename... Args>
	std::shared_ptr<peg::Ope> OneOf(Args&&... args)
	{
		return std::make_shared<peg::PrioritizedChoice>(static_cast<std::shared_ptr<peg::Ope>>(args)...);
	}

	// Matches a sequence containing comments and the specified element in arbitrary order. If EmptyAllowed = true, list
	// may be missing (empty)
	std::shared_ptr<peg::Ope> ListOf(const std::shared_ptr<peg::Ope>& ope, bool EmptyAllowed)
	{
		return EmptyAllowed ? peg::Repetition::zom(peg::cho(GetDecl("Comment"), ope))
							: peg::Repetition::oom(peg::cho(GetDecl("Comment"), ope));
	}

	std::shared_ptr<peg::Ope> WordDelimited(std::string DelimiterClass)
	{
		return peg::oom(peg::ncls(DelimiterClass));
	}

	// Matches a section ('#SectionName <Content elements> #End SectionName')
	std::shared_ptr<peg::Ope> Section(const std::string SectionName, const std::shared_ptr<peg::Ope>& Contents);

	// Defines a symbol so it can be referred to by name elsewhere in the descriptor
	std::shared_ptr<peg::Ope> Define(const std::string SymbolName, const std::shared_ptr<peg::Ope>& Definition);

	// Helper function to add a literal string to the symbol table inline
	std::shared_ptr<peg::Ope> DefinedLiteral(const std::string SymbolName);

	std::shared_ptr<peg::Ope> Repetition(std::size_t MinReps, std::size_t MaxReps,
										 const std::shared_ptr<peg::Ope>& Contents);

	std::shared_ptr<peg::Ope> XMTNoCreateFlag()
	{
		return DefinedLiteral("+nocreate");
	}

	// Retrieves a symbol previously defined by Define
	std::shared_ptr<peg::Ope> GetDecl(const std::string SymbolName)
	{
		return g[SymbolName];
	}

	// Attempt to parse the input data, storing a tree of parse results into AstPtr
	template<typename T>
	bool UseRuleToParseInput(const std::string RuleName, const SCP_buffer& Input, T& AstPtr)
	{
		for (auto& Def : g)
		{
			if (Def.second.name.empty())
			{
				Def.second.name = Def.first;
			}
			peg::add_ast_action(Def.second);
		}
		g[RuleName].whitespaceOpe = peg::wsp(g[peg::WHITESPACE_DEFINITION_NAME].get_core_operator());

		peg::Definition::Result results = g[RuleName].parse_and_get_value(Input.Data(), AstPtr);
		return results.ret;
	}
	// Enables console mode tracing of parsing rules. Intended for debugging only
	void EnableTracingForRule(bool bEnableTracing, const std::string RuleName);

	void EnableTracingAllRules(bool bEnableTracing);
};