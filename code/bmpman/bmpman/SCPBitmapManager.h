#pragma once

#include "module/SCPModuleBase.h"

namespace SCP
{
	class BitmapManagerModule : public SCPModuleBase<BitmapManagerModule>
	{
	public:
		static BitmapManagerModule ConstructModule() {};

		BitmapID LoadBitmap(SCP_string BitmapFilename);
		BitmapID CreateBitmap(std::uint32_t BitDepth, std::uint32_t Width, std::uint32_t Height, SCP_buffer BitmapData,
							  BitmapFlags BmpFlags);
		bool ReleaseBitmap(BitmapID BitmapToRelease);

		BitmapDescriptor LoadAnimation(SCP_string BitmapFilename, SCPCFilePathTypeID DirectoryType);
		BitmapDescriptor GetInfo(BitmapID IDToQuery);

		std::shared_ptr<BitmapData> LockBitmap(BitmapID BitmapToLock);


	};


}