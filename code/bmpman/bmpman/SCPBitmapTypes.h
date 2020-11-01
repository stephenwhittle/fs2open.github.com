#pragma once
#include "SCPStrongInteger.h"
#include "SCPFlags.h"
#include "FSStdTypes.h"

namespace SCP
{
	class BitmapID : public SCP::StrongInteger<std::uint32_t>
	{};

	class BitmapFlags : public SCPFlags<std::uint32_t>
	{};

	struct BitmapDescriptor
	{
		BitmapID FirstBitmap;
		std::uint32_t NumFrames;
		std::uint32_t KeyframeIndex;
		std::uint32_t FrameRatePerSecond;
		float Duration;
		bool bDropFramesAllowed;
		SCP_string Filename;
	};


	struct BitmapData
	{
		std::uint16_t Width;
		std::uint16_t Height;
		std::uint16_t RowSize;
		std::uint8_t RequestedBitDepth;
		std::uint8_t ActualBitDepth;
		BitmapFlags Flags;
		std::uintptr_t Data;
		std::uintptr_t Palette;
	};
}


