#pragma once

#include "FSAssert.h"
#include "FSColorTypes.h"
#include "parse/SCPTableFormatDescriptor.h"
#include "peglib.h"
#include "tl/optional.hpp"
#include <functional>
#include <map>

// https://dev.to/tmr232/that-overloaded-trick-overloading-lambdas-in-c17
// https://gist.github.com/pyrtsa/2945472

// table object that defines an ordered list of base table, override 1, override 2, etc

using SCPParsedTableData = peg::Ast;

std::shared_ptr<SCPParsedTableData> FindFirstNode(const SCPParsedTableData& Parent,
												  std::function<bool(std::shared_ptr<SCPParsedTableData>)> Predicate);

bool HasNodeWithName(const SCPParsedTableData& Parent, std::string Name, bool WholeExactMatch = true);

// specialise this template for each object that needs to be constructed - this can be defined wherever we want
// meaning that the parsing lib doesn't have to know anything about the object we are creating an instance of
template<typename T>
tl::optional<T> construct(const SCPParsedTableData& InData)
{
	return tl::optional<T>();
}

template<>
inline tl::optional<bool> construct(const SCPParsedTableData& InData)
{
	if ((InData.nodes[0]->nodes[0]->nodes[0]->token == "true") || (InData.nodes[0]->nodes[0]->nodes[0]->token == "YES"))
	{
		return true;
	}
	else if ((InData.nodes[0]->nodes[0]->nodes[0]->token == "false") ||
			 (InData.nodes[0]->nodes[0]->nodes[0]->token == "NO"))
	{
		return false;
	}
	else
	{
		return {};
	}
}

template<>
inline tl::optional<color> construct(const SCPParsedTableData& InData)
{
	SCPTableFormatDescriptor ColorRule;
	// clang-format off
	ColorRule.Define("Color", 
		ColorRule.Literal("(") 
		& ColorRule.GetDecl("Float") 
		& ColorRule.Literal(",") 
		& ColorRule.GetDecl("Float") 
		& ColorRule.Literal(",") 
		& ColorRule.GetDecl("Float") 
		& ColorRule.Literal(")")
	);
	// clang-format on

	std::shared_ptr<SCPParsedTableData> my_ast;
	const std::string& ColorText = InData.nodes[0]->nodes[0]->token;
	SCP_buffer ColorInput(ColorText.size() + 1);
	std::copy(ColorText.begin(), ColorText.end(), ColorInput.begin());
	if (ColorRule.UseRuleToParseInput("Color", std::move(ColorInput), my_ast))
	{
		int RValue = (int) std::atof(my_ast->nodes[0]->token.c_str());
		int GValue = (int) std::atof(my_ast->nodes[1]->token.c_str());
		int BValue = (int) std::atof(my_ast->nodes[2]->token.c_str());
		return color(RValue, GValue, BValue, 1);
	}
	else
	{
		return {};
	}
}

template<>
inline tl::optional<float> construct(const SCPParsedTableData& InData)
{
	return tl::optional<float>((float) std::atof(InData.nodes[0]->nodes[0]->token.c_str()));
}

template<>
inline tl::optional<int> construct(const SCPParsedTableData& InData)
{
	return tl::optional<int>(std::atoi(InData.nodes[0]->nodes[0]->token.c_str()));
}

template<>
inline tl::optional<std::string> construct(const SCPParsedTableData& InData)
{
	std::string TrimmedValue = InData.nodes[0]->nodes[0]->token;
	TrimmedValue.erase(std::find_if_not(TrimmedValue.rbegin(), TrimmedValue.rend(), std::isspace).base(),
					   TrimmedValue.end());
	return tl::optional<std::string>(std::move(TrimmedValue));
}

// all of the deserialization handlers should be modifying fields on the same object
// so we can erase the container needing to know about the type of the field being altered
// by wrapping a reference to the field in a std::function that is only dependent on the class the field is defined in
// this means that all the deserialization handlers will be operating on fields belonging to the same type
// but honestly thats a limitation I can live with for now. Worst case change the signature on DeserializeToField to
// take in an instance which is then captured by reference, but I think this is safer
template<typename T>
using DeserializationHandlers =
	std::map<std::string, std::function<void(T* TargetObject, const SCPParsedTableData& InData)>>;

// this will need to support XMT tables later (need a function to find the first valid value going back through the
// hierarchy)

// possibly replace the CRTP with a virtual template member that can return DeserializationHandlers<T>
template<class T>
class SCPTableBase
{
	static const DeserializationHandlers<T> Deserializers;

protected:
	bool bIsModularOverride = false;

public:
	bool IsModularOverride()
	{
		return bIsModularOverride;
	}
	void Deserialize(std::string Symbol, const SCPParsedTableData& InData)
	{
		Symbol.erase(std::find_if_not(Symbol.rbegin(), Symbol.rend(), std::isspace).base(), Symbol.end());

		// Special handling for the nocreate metadata flag
		if (Symbol == "Literal_+nocreate")
		{
			bIsModularOverride = true;
			return;
		}

		T* Object = static_cast<T*>(this);
		auto HandlerIt = Deserializers.find(Symbol);
		if (HandlerIt != Deserializers.end())
		{
			HandlerIt->second(Object, InData);
		}
	}
};

/// <summary>
/// Class that allows for values to 'stack' in tables. This means we can set sane defaults, or load those from a default
/// table, then 'overlay' the user/mod tables over the top.
/// </summary>
/// <typeparam name="T">The underlying value for this particular table property</typeparam>
template<typename T>
class SCPTableProperty
{
	std::vector<T> PropertyValues;
	// tl::optional<SCPVersionInfo> ValidFromVersion;
public:
	SCPTableProperty() {};
	SCPTableProperty(const T& InitialValue)
	{
		PropertyValues.push_back(InitialValue);
	}
	bool IsSet()
	{
		return PropertyValues.size() > 0;
	}
	operator tl::optional<T&>()
	{
		if (PropertyValues.size() > 0)
		{
			return PropertyValues.back();
		}
		else
		{
			return tl::optional<T&>();
		}
	}
	operator T&()
	{
		Assert(PropertyValues.size() > 0);
		return PropertyValues.back();
	}
	T& operator->()
	{
		Assert(PropertyValues.size() > 0);
		return PropertyValues.back();
	}
	SCPTableProperty<T>& operator=(const T& OverrideValue)
	{
		PropertyValues.push_back(OverrideValue);
		return *this;
	}
	SCPTableProperty<T>& operator=(const tl::optional<T>& OverrideValue)
	{
		if (OverrideValue.has_value())
		{
			PropertyValues.push_back(OverrideValue.value());
		}

		return *this;
	}
	void ReplaceValue(const T& OverrideValue)
	{
		if (PropertyValues.size())
		{
			PropertyValues.back() = OverrideValue;
		}
		else
		{
			PropertyValues.push_back(OverrideValue);
		}
	}

	void ReplaceValue(const tl::optional<T>& OverrideValue)
	{
		if (OverrideValue.has_value())
		{
			ReplaceValue(OverrideValue.value());
		}
	}
	// need to override assignment operator
	// need to implement override method?
};

// can create other functions instead so long as they maintain the void(ClassType* ClassInstance, const
// SCPParsedTableData& InData) signature
template<typename ClassType, typename FieldType>
auto DeserializeToField(SCPTableProperty<FieldType> ClassType::*Field)
{
	return [Field](ClassType* ClassInstance, const SCPParsedTableData& InData) {
		ClassInstance->*Field = construct<FieldType>(InData);
	};
}

template<typename ClassType, typename FieldType>
auto ReplaceValue(SCPTableProperty<FieldType> ClassType::*Field)
{
	return [Field](ClassType* ClassInstance, const SCPParsedTableData& InData) {
		(ClassInstance->*Field).ReplaceValue(construct<FieldType>(InData));
	};
}

template<typename ClassType, typename FieldType>
auto PushBack(SCPTableProperty<std::vector<FieldType>> ClassType::*Field)
{
	return [Field](ClassType* ClassInstance, const SCPParsedTableData& InData) {
		if (!(ClassInstance->*Field).IsSet())
		{
			(ClassInstance->*Field) = std::vector<FieldType>();
		}
		std::vector<FieldType>& Container = ClassInstance->*Field;
		tl::optional<FieldType> ParsedValue = construct<FieldType>(InData);
		if (ParsedValue)
		{
			Container.push_back(*ParsedValue);
		}
	};
}

template<typename ClassType, typename FieldType, typename XMTKeyType>
auto PushBackXMT(SCPTableProperty<std::vector<SCPTableBase<FieldType>>> ClassType::*Field,
				 SCPTableProperty<XMTKeyType> FieldType::*XMTKey)
{
	// Dynamically generate deserializer as a lambda
	return [Field, XMTKey](ClassType* ClassInstance, const SCPParsedTableData& InData) {
		if (!(ClassInstance->*Field).IsSet())
		{
			(ClassInstance->*Field) = std::vector<FieldType>();
		}
		std::vector<FieldType>& Container = ClassInstance->*Field;
		// Parse our AST into our data object
		tl::optional<FieldType> ParsedValue = construct<FieldType>(InData);

		if (ParsedValue)
		{
			// if that was successful, check if the object was a modular override (ie its nocreate flag was set) and
			// apply the override if we can find a matching object in the container, using the specified key
			SCPTableBase<FieldType>& EntryRef = static_cast<SCPTableBase<FieldType>>(*ParsedValue);
			if (EntryRef.IsModularOverride())
			{
				auto BaseValue =
					std::find_if(Container.begin(), Container.end(), [XMTKey, &ParsedValue](const FieldType& Value) {
						FieldType* ValuePtr = &Value;
						FieldType* NewPtr = &(ParsedValue.value());
						return (ValuePtr->*XMTKey) == NewPtr->*XMTKey;
					});
				if (BaseValue != Container.end())
				{
					// iterate over all the fields and merge ParsedValue's overrides over BaseValues ones
					// perhaps implement template specialization here ala ::construct, called
					// ::override or similar (I should probably namespace both of those)
				}
			}
		}
	}
}

template<typename ClassType, typename FieldType>
auto Merge(SCPTableProperty<FieldType> ClassType::*Field)
{
	return [Field](ClassType* ClassInstance, const SCPParsedTableData& InData) {
		if (!(ClassInstance->*Field).IsSet())
		{
			(ClassInstance->*Field) = construct<FieldType>(InData);
			return;
		}
		SCPTableBase<FieldType>& Instance = static_cast<SCPTableBase<FieldType>&>(ClassInstance->*Field);

		for (auto Node : InData.nodes)
		{
			Instance.Deserialize(Node->name, *Node);
		}
	};
}

class SCPVersionInfo
{
public:
	SCPVersionInfo(int Major, tl::optional<int> Minor = tl::nullopt, tl::optional<int> Build = tl::nullopt,
				   tl::optional<int> Revision = tl::nullopt)
		: Major(Major),
		  Minor(Minor),
		  Build(Build),
		  Revision(Revision) {};

private:
	int Major;
	tl::optional<int> Minor;
	tl::optional<int> Build;
	tl::optional<int> Revision;
};