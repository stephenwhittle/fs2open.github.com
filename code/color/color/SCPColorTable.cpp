#include "SCPColorTable.h"



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
	return DefaultColorSet;
}
