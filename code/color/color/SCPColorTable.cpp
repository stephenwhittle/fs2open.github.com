#include "SCPColorTable.h"
#include "parse/SCPTableFormatDescriptor.h"


SCPColorSet SCPColorSet::DefaultColors()
{
	SCPColorSet DefaultColorSet;
	DefaultColorSet.Blue         = color(93, 93, 128, 255);
	DefaultColorSet.BrightBlue   = color(185, 185, 255, 255);
	DefaultColorSet.Green        = color(0, 120, 0, 255);
	DefaultColorSet.BrightGreen  = color(50, 190, 50, 255);
	DefaultColorSet.Black        = color(0, 0, 0, 255);
	DefaultColorSet.Grey         = color(65, 65, 65, 255);
	DefaultColorSet.Silver       = color(160, 160, 160, 255);
	DefaultColorSet.White        = color(105, 105, 105, 255);
	DefaultColorSet.BrightWhite  = color(255, 255, 255, 255);
	DefaultColorSet.VioletGray   = color(160, 144, 160, 255);
	DefaultColorSet.Violet       = color(192, 104, 192, 255);
	DefaultColorSet.DimRed       = color(80, 6, 6, 255);
	DefaultColorSet.Red          = color(126, 6, 6, 255);
	DefaultColorSet.BrightRed    = color(200, 0, 0, 255);
	DefaultColorSet.Pink         = color(185, 150, 150, 255);
	DefaultColorSet.LightPink    = color(230, 190, 190, 255);
	DefaultColorSet.Yellow       = color(255, 255, 122, 255);
	DefaultColorSet.BrightYellow = color(255, 255, 0, 255);
	DefaultColorSet.Orange       = color(255, 127, 0, 255);
	DefaultColorSet.UILightGreen = color(161, 184, 161, 255);
	DefaultColorSet.UIGreen      = color(190, 228, 190, 255);
	DefaultColorSet.UILightPink  = color(184, 161, 161, 255);
	DefaultColorSet.UIPink       = color(228, 190, 190, 255);

	DefaultColorSet.TextNormal = SCPColorReference("White");
	DefaultColorSet.TextSubselected = SCPColorReference("Blue");
	DefaultColorSet.TextSelected = SCPColorReference("Bright Blue");
	DefaultColorSet.TextError = SCPColorReference("Red");
	DefaultColorSet.TextErrorHighlighted = SCPColorReference("Bright Red");
	DefaultColorSet.TextActive = SCPColorReference("Bright White");
	DefaultColorSet.TextActiveHighlighted = SCPColorReference("Bright White");
	DefaultColorSet.TextHeading = SCPColorReference("Violet Gray");
	DefaultColorSet.MoreIndicator = SCPColorReference("Red");
	DefaultColorSet.BrightMoreIndicator = SCPColorReference("Bright Red");
	DefaultColorSet.Bright = SCPColorReference("Bright Blue");
	DefaultColorSet.Normal = SCPColorReference("White");
	DefaultColorSet.BriefingGrid = SCPColorReference("Grey");

	DefaultColorSet.FictionViewerTag = SCPColorReference('w');
	DefaultColorSet.CommandBriefingTag = SCPColorReference('w');
	DefaultColorSet.BriefingTag = SCPColorReference('w');
	DefaultColorSet.RedalertBriefingTag = SCPColorReference('w');
	DefaultColorSet.DebriefingTag = SCPColorReference('w');
	DefaultColorSet.RecommendationTag = SCPColorReference('r');
	DefaultColorSet.LoopBriefingTag = SCPColorReference('w');

	return DefaultColorSet;
}


void DeserializeChildren(SCPColorSet* ClassInstance, const SCPParsedTableData& InData)
{
	for (auto Node : InData.nodes)
	{
		ClassInstance->Deserialize(Node->name, *Node);
	}
}

void DeserializeTeamColors(SCPColorSet* ClassInstance, const SCPParsedTableData& InData)
{
	std::map<std::string, team_color >& InstanceTeamColors = ClassInstance->TeamColors;
	for (auto Node : InData.nodes)
	{
		auto NewColor = construct<team_color>(*Node);
		if (NewColor.has_value())
		{
			tl::optional<std::string> TeamName = construct<std::string>(*(Node->nodes[0]));
			if (TeamName.has_value())
			{
				InstanceTeamColors[*TeamName] = *NewColor;
			}
			
		}
	}
}

void DeserializeColorTags(SCPColorSet* ClassInstance, const SCPParsedTableData& InData)
{
	std::map<std::string, SCPColorReference>& InstanceTaggedColors= ClassInstance->TaggedColorReferences;
	for (auto Node : InData.nodes) {
		auto NewColor = construct<SCPColorReference>(*Node);
		if (NewColor.has_value()) {
			tl::optional<std::string> Tag = construct<std::string>(*(Node->nodes[0]));
			if (Tag.has_value()) {
				InstanceTaggedColors[*Tag] = *NewColor;
			}
		}
	}
}

//uses ReplaceValue for fields so we get the merged results of the color tables rather than a stack of overrides
const DeserializationHandlers<SCPColorSet> SCPTableBase<SCPColorSet>::Deserializers =
{
	{SCPTableFormatDescriptor::GetVariableName("$Blue"), ReplaceValue(&SCPColorSet::Blue)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright Blue"), ReplaceValue(&SCPColorSet::BrightBlue)},
	{SCPTableFormatDescriptor::GetVariableName("$Green"), ReplaceValue(&SCPColorSet::Green)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright Green"), ReplaceValue(&SCPColorSet::BrightGreen)},
	{SCPTableFormatDescriptor::GetVariableName("$Black"), ReplaceValue(&SCPColorSet::Black)},
	{SCPTableFormatDescriptor::GetVariableName("$Grey"), ReplaceValue(&SCPColorSet::Grey)},
	{SCPTableFormatDescriptor::GetVariableName("$Silver"), ReplaceValue(&SCPColorSet::Silver)},
	{SCPTableFormatDescriptor::GetVariableName("$White"), ReplaceValue(&SCPColorSet::White)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright White"), ReplaceValue(&SCPColorSet::BrightWhite)},
	{SCPTableFormatDescriptor::GetVariableName("$Violet Gray"), ReplaceValue(&SCPColorSet::VioletGray)},
	{SCPTableFormatDescriptor::GetVariableName("$Violet"), ReplaceValue(&SCPColorSet::Violet)},
	{SCPTableFormatDescriptor::GetVariableName("$Dim Red"), ReplaceValue(&SCPColorSet::DimRed)},
	{SCPTableFormatDescriptor::GetVariableName("$Red"), ReplaceValue(&SCPColorSet::Red)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright Red"), ReplaceValue(&SCPColorSet::BrightRed)},
	{SCPTableFormatDescriptor::GetVariableName("$Pink"), ReplaceValue(&SCPColorSet::Pink)},
	{SCPTableFormatDescriptor::GetVariableName("$Light Pink"), ReplaceValue(&SCPColorSet::LightPink)},
	{SCPTableFormatDescriptor::GetVariableName("$Yellow"), ReplaceValue(&SCPColorSet::Yellow)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright Yellow"), ReplaceValue(&SCPColorSet::BrightYellow)},
	{SCPTableFormatDescriptor::GetVariableName("$Orange"), ReplaceValue(&SCPColorSet::Orange)},
	{SCPTableFormatDescriptor::GetVariableName("$UI Light Green"), ReplaceValue(&SCPColorSet::UILightGreen)},
	{SCPTableFormatDescriptor::GetVariableName("$UI Green"), ReplaceValue(&SCPColorSet::UIGreen)},
	{SCPTableFormatDescriptor::GetVariableName("$UI Light Pink"), ReplaceValue(&SCPColorSet::UILightPink)},
	{SCPTableFormatDescriptor::GetVariableName("$UI Pink"), ReplaceValue(&SCPColorSet::UIPink)},

	{SCPTableFormatDescriptor::GetVariableName("$Text Normal"), ReplaceValue(&SCPColorSet::TextNormal)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Subselected"), ReplaceValue(&SCPColorSet::TextSubselected)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Selected"), ReplaceValue(&SCPColorSet::TextSelected)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Error"), ReplaceValue(&SCPColorSet::TextError)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Error Highlighted"), ReplaceValue(&SCPColorSet::TextErrorHighlighted)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Active"), ReplaceValue(&SCPColorSet::TextActive)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Active Highlighted"), ReplaceValue(&SCPColorSet::TextActiveHighlighted)},
	{SCPTableFormatDescriptor::GetVariableName("$Text Heading"), ReplaceValue(&SCPColorSet::TextHeading)},
	{SCPTableFormatDescriptor::GetVariableName("$More Indicator"), ReplaceValue(&SCPColorSet::MoreIndicator)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright More Indicator"), ReplaceValue(&SCPColorSet::BrightMoreIndicator)},
	{SCPTableFormatDescriptor::GetVariableName("$Bright"), ReplaceValue(&SCPColorSet::Bright)},
	{SCPTableFormatDescriptor::GetVariableName("$Normal"), ReplaceValue(&SCPColorSet::Normal)},
	{SCPTableFormatDescriptor::GetVariableName("$Briefing Grid"), ReplaceValue(&SCPColorSet::BriefingGrid)},

	{SCPTableFormatDescriptor::GetVariableName("$Fiction Viewer"), ReplaceValue(&SCPColorSet::FictionViewerTag)},
	{SCPTableFormatDescriptor::GetVariableName("$Command Briefing"), ReplaceValue(&SCPColorSet::CommandBriefingTag)},
	{SCPTableFormatDescriptor::GetVariableName("$Briefing"), ReplaceValue(&SCPColorSet::BriefingTag)},
	{SCPTableFormatDescriptor::GetVariableName("$Redalert Briefing"), ReplaceValue(&SCPColorSet::RedalertBriefingTag)},
	{SCPTableFormatDescriptor::GetVariableName("$Debriefing"), ReplaceValue(&SCPColorSet::DebriefingTag)},
	{SCPTableFormatDescriptor::GetVariableName("$Recommendation"), ReplaceValue(&SCPColorSet::RecommendationTag)},
	{SCPTableFormatDescriptor::GetVariableName("$Loop Briefing"), ReplaceValue(&SCPColorSet::LoopBriefingTag)},

	{SCPTableFormatDescriptor::GetSectionName("Start Colors"), DeserializeChildren},
	{SCPTableFormatDescriptor::GetSectionName("Team Colors"), DeserializeTeamColors},
	{SCPTableFormatDescriptor::GetSectionName("Interface Colors"), DeserializeChildren},
	{SCPTableFormatDescriptor::GetSectionName("Color Tags"), DeserializeColorTags}
	{SCPTableFormatDescriptor::GetSectionName("Default Text Colors"), DeserializeChildren}


};