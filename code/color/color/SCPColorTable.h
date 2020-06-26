#pragma once
#include "parse/SCPTable.h"
#include "color/alphacolors.h"

class SCPColorSet : SCPTableBase<SCPColorSet>
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

	static SCPColorSet DefaultColors();
};

class SCPColorTable : public SCPTableBase<SCPColorTable>
{
public:
	SCPTableProperty<SCPColorSet> Colors;
	SCPColorTable()
		: Colors(SCPColorSet::DefaultColors()) {}
};

