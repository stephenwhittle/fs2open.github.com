#pragma once
#include "FSColorTypes.h"
#include "tl/optional.hpp"
#include <string>
#include "parse/SCPTable.h"
class SCPColorSet;

class SCPColorReference {
	tl::optional<std::string> ColorName;
	tl::optional<char> ColorTag;
	tl::optional<color&> ResolvedColorReference;
	tl::optional<color> SpecifiedColor;

  public:
	SCPColorReference(std::string Name) : ColorName(Name){};

	SCPColorReference(color ColorToUse) : SpecifiedColor(ColorToUse){};

	SCPColorReference(char ColorTag) : ColorTag(ColorTag){};

	SCPColorReference(const SCPColorReference& Other)
		: ColorName(Other.ColorName), ColorTag(Other.ColorTag), ResolvedColorReference(Other.ResolvedColorReference),
		  SpecifiedColor(Other.SpecifiedColor){};

	SCPColorReference() : SpecifiedColor(color()){};

	color& operator=(const SCPColorReference& InColorRef)
	{
		if (SpecifiedColor.has_value()) {
			return *SpecifiedColor;
		}

		if (!ResolvedColorReference.has_value()) {
			if (ColorName.has_value()) {
				// resolve color name;
			} else if (ColorTag.has_value()) {
				// resolve color by tag;
			}
		}

		return *ResolvedColorReference;
	}
};

template <>
tl::optional<SCPColorReference> construct(const SCPParsedTableData& InData);

template <>
tl::optional<team_color> construct(const SCPParsedTableData& InData);
