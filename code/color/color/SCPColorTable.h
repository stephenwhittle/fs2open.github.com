#pragma once
#include "parse/SCPTable.h"
#include "color/alphacolors.h"



class SCPColorSet : public SCPTableBase<SCPColorSet>
{
public:
	SCPTableProperty<color> Blue;
	SCPTableProperty<color> BrightBlue;
	SCPTableProperty<color> Green;
	SCPTableProperty<color> BrightGreen;
	SCPTableProperty<color> Black;
	SCPTableProperty<color> Grey;
	SCPTableProperty<color> Silver;
	SCPTableProperty<color> White;
	SCPTableProperty<color> BrightWhite;
	SCPTableProperty<color> VioletGray;
	SCPTableProperty<color> Violet;
	SCPTableProperty<color> DimRed;
	SCPTableProperty<color> Red;
	SCPTableProperty<color> BrightRed;
	SCPTableProperty<color> Pink;
	SCPTableProperty<color> LightPink;
	SCPTableProperty<color> Yellow;
	SCPTableProperty<color> BrightYellow;
	SCPTableProperty<color> Orange;
	SCPTableProperty<color> UILightGreen;
	SCPTableProperty<color> UIGreen;
	SCPTableProperty<color> UILightPink;
	SCPTableProperty<color> UIPink;
	
	SCPTableProperty<std::map<std::string, team_color>> TeamColors;
	
	SCPTableProperty<SCPColorReference> TextNormal;
	SCPTableProperty<SCPColorReference> TextSubselected;
	SCPTableProperty<SCPColorReference> TextSelected;
	SCPTableProperty<SCPColorReference> TextError;
	SCPTableProperty<SCPColorReference> TextErrorHighlighted;
	SCPTableProperty<SCPColorReference> TextActive;
	SCPTableProperty<SCPColorReference> TextActiveHighlighted;
	SCPTableProperty<SCPColorReference> TextHeading;
	SCPTableProperty<SCPColorReference> MoreIndicator;
	SCPTableProperty<SCPColorReference> BrightMoreIndicator;
	SCPTableProperty<SCPColorReference> Bright;
	SCPTableProperty<SCPColorReference> Normal;
	SCPTableProperty<SCPColorReference> BriefingGrid;

	SCPTableProperty<SCPColorReference> FictionViewerTag;
	SCPTableProperty<SCPColorReference> CommandBriefingTag;
	SCPTableProperty<SCPColorReference> BriefingTag;
	SCPTableProperty<SCPColorReference> RedalertBriefingTag;
	SCPTableProperty<SCPColorReference> DebriefingTag;
	SCPTableProperty<SCPColorReference> RecommendationTag;
	SCPTableProperty<SCPColorReference> LoopBriefingTag;
	SCPTableProperty<std::map<std::string, SCPColorReference>> TaggedColorReferences;

	static SCPColorSet DefaultColors();
};

