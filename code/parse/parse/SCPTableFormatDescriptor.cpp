
#include "SCPTableFormatDescriptor.h"
#include <memory>
#include <vector>
#include <utility>
#include "peglib.h"
std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Sequence>& a, const std::shared_ptr<peg::Ope>& b)
{
	std::vector<std::shared_ptr<peg::Ope>> JoinedSequence;
	JoinedSequence = a->opes_;
	JoinedSequence.push_back(b);

	return std::make_shared<peg::Sequence>(std::forward< std::vector<std::shared_ptr<peg::Ope>>>(JoinedSequence));
}

std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Ope>& a, const std::shared_ptr<peg::Sequence>& b)
{
	std::vector<std::shared_ptr<peg::Ope>> JoinedSequence;
	JoinedSequence.push_back(a);
	JoinedSequence.insert(JoinedSequence.end(), b->opes_.begin(), b->opes_.end());

	return std::make_shared<peg::Sequence>(std::forward< std::vector<std::shared_ptr<peg::Ope>>>(JoinedSequence));
}

std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Sequence>& a, const std::shared_ptr<peg::Sequence>& b)
{
	std::vector<std::shared_ptr<peg::Ope>> JoinedSequence;
	JoinedSequence = a->opes_;
	JoinedSequence.insert(JoinedSequence.end(), b->opes_.begin(), b->opes_.end());

	return std::make_shared<peg::Sequence>(std::forward< std::vector<std::shared_ptr<peg::Ope>>>(JoinedSequence));
}

std::shared_ptr<peg::Sequence> operator&(const std::shared_ptr<peg::Ope>& a, const std::shared_ptr<peg::Ope>& b)
{
	return std::static_pointer_cast<peg::Sequence>(peg::seq(a, b));
}

SCPTableFormatDescriptor::SCPTableFormatDescriptor()
{
	g["Comment"] <= peg::tok(peg::seq(peg::chr(';'), peg::zom(peg::ncls("\r\n")), peg::cls("\r\n")));
	g["Comment"].name = "Comment";

	g[peg::WHITESPACE_DEFINITION_NAME] <= OneOf(
		peg::zom(peg::cls(" \t\r\n")),
		g["Comment"]
	);
	g["Number"] <=
		peg::opt(peg::chr('-'))
		& peg::oom(peg::cls("0-9"));
	g["Float"] <=
		peg::tok(
			peg::seq(
				peg::opt(peg::chr('-')),
				peg::oom(peg::cls("0-9")),
				Optional(
					peg::chr('.')
					& peg::oom(peg::cls("0-9"))
				),
				peg::apd(peg::oom(peg::cls(" \t\r\n")))
			)
		);
	g["QuotedString"] <=
		peg::tok(
			peg::chr('"')
			& peg::oom(peg::ncls("\""))
			& peg::chr('"')
		);

	g["XSTR"] <=
		Literal("XSTR")
		& peg::chr('(')
		& g["QuotedString"]
		& peg::chr(',')
		& g["Number"]
		& peg::chr(')')
		& Literal("$end_multi_text");

	g["True"] <=
		OneOf(
			peg::tok(Literal("yes")),
			peg::tok(Literal("true"))
		);
	g["False"] <=
		OneOf(
			peg::tok(Literal("no")),
			peg::tok(Literal("false"))
		);
	g["Boolean"] <=
		OneOf(g["True"], g["False"]);

	g["String"] <=
		peg::tok(peg::oom(peg::ncls("$:#;+\r\n")));

	g["Value"] <=
		OneOf(
			g["Float"],
			g["Boolean"],
			g["XSTR"],
			g["String"]

		);

	g["VarSeparator"] <= peg::seq(peg::ign(g[peg::WHITESPACE_DEFINITION_NAME]), Character(':'), peg::ign(g[peg::WHITESPACE_DEFINITION_NAME]));
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::OptionalVariable(const std::string& VariableName)
{
	std::string MangledVariableName = GetVariableName(VariableName);

	g[MangledVariableName] <= peg::seq(Literal(VariableName), peg::ign(g["VarSeparator"]), g["Value"]);
	g[MangledVariableName].name = MangledVariableName;
	return peg::opt(g[MangledVariableName]);
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::RequiredVariable(const std::string& VariableName)
{
	std::string MangledVariableName = GetVariableName(VariableName);

	g[MangledVariableName] <= peg::seq(Literal(VariableName), peg::ign(g["VarSeparator"]), g["Value"]);
	g[MangledVariableName].name = MangledVariableName;
	return g[MangledVariableName];
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::ObjectHeader(const std::string& Name)
{
	return std::make_shared<peg::Sequence>(Literal(Name), Character(':'), peg::ign(g[peg::WHITESPACE_DEFINITION_NAME]));
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::EndObject(const std::string& Name /*= ""*/)
{
	std::string EndString = "$End";
	if (!Name.empty())
	{
		EndString += " " + Name;
	}
	return std::make_shared<peg::Sequence>(Literal(EndString), peg::ign(g[peg::WHITESPACE_DEFINITION_NAME]));
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::Optional(const std::shared_ptr<peg::Ope>& Entry)
{
	return peg::opt(Entry);
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::Section(const std::string SectionName, const std::shared_ptr<peg::Ope>& Contents)
{
	std::shared_ptr<peg::Ope> CaptureOp = peg::cap(Literal(SectionName), [SectionName](const char* a_s, size_t a_n, peg::Context& c) {
		auto& cs = c.capture_scope_stack[c.capture_scope_stack_size - 1];
		cs[SectionName] = std::string(a_s, a_n);
		});
	
	std::string MangledSectionName = GetSectionName(SectionName);

	g[MangledSectionName] <= peg::csc(
		peg::seq(
			peg::seq(peg::chr('#'), CaptureOp),
			Contents,
			peg::seq(Literal("#End"), Optional(peg::bkr(SectionName)))
		));
	g[MangledSectionName].name = MangledSectionName;
	return g[MangledSectionName];
}

std::shared_ptr<peg::Ope> SCPTableFormatDescriptor::Define(const std::string SymbolName, const std::shared_ptr<peg::Ope>& Definition)
{
	g[SymbolName] <= Definition;
	g[SymbolName].name = SymbolName;
	return g[SymbolName];
}

void SCPTableFormatDescriptor::EnableTracingForRule(bool bEnableTracing, const std::string RuleName)
{
	auto& rule = g[RuleName];
	size_t prev_pos = 0;
	rule.tracer_enter = [&](const char* name, const char* s, size_t /*n*/,
		const peg::SemanticValues& /*sv*/, const peg::Context& c,
		const peg::any& /*dt*/) {
			auto pos = static_cast<size_t>(s - c.s);
			auto backtrack = (pos < prev_pos ? "*" : "");
			std::string indent;
			auto level = c.trace_ids.size() - 1;
			while (level--) {
				indent += "|";
			}
			std::cout << "E " << pos << backtrack << "\t" << indent << name
				<< " #" << c.trace_ids.back() << std::endl;
			prev_pos = static_cast<size_t>(pos);
	};
	rule.tracer_leave = [&](const char* name, const char* s, size_t /*n*/,
		const peg::SemanticValues& sv, const peg::Context& c,
		const peg::any& /*dt*/, size_t len) {
			auto pos = static_cast<size_t>(s - c.s);
			if (len != static_cast<size_t>(-1)) { pos += len; }
			std::string indent;
			auto level = c.trace_ids.size() - 1;
			while (level--) {
				indent += "|";
			}
			std::stringstream choice;
			if (sv.choice_count() > 0) {
				choice << " " << sv.choice() << "/" << sv.choice_count();
			}
			std::string token;
			if (!sv.tokens.empty()) {
				const auto& tok = sv.tokens[0];
				token += " '" + std::string(tok.first, tok.second) + "'";
			}
			if (len > 0)
			{
				std::cout << "L " << pos << "\t" << indent << name << " #"
					<< c.trace_ids.back() << choice.str() << token << (sv.c_str() != nullptr ? sv.c_str() : "") << std::endl;
			}
			else
			{
				std::cout << "L " << pos << "\t" << indent << name << " Failed" << std::endl;
			}
	};
}


