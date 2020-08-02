#include "SCPColor.h"

template <>
tl::optional<SCPColorReference> construct(const SCPParsedTableData& InData)
{
	auto ColorString = construct<std::string>(InData);
	if (ColorString.has_value()) {
		if ((ColorString->size() == 2) && (*(ColorString->begin()) == '$')) {
			// Return a color reference to the specified tag
			return SCPColorReference((*ColorString)[1]);
		} else {
			return SCPColorReference(*ColorString);
		}
	}
	return tl::optional<SCPColorReference>();
}

template <>
tl::optional<team_color> construct(const SCPParsedTableData& InData)
{
	tl::optional<std::string> StripeColor = construct<std::string>(*InData.nodes[1]);
	tl::optional<std::string> BaseColor   = construct<std::string>(*InData.nodes[2]);
	team_color NewColor;
	return NewColor;
}

