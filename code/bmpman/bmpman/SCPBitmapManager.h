#pragma once

#include "module/SCPModuleBase.h"
#include <memory>
#include "FSStdTypes.h"
#include <map>

#undef LoadBitmap

namespace SCP
{
	class BitmapID;
	struct BitmapData;
	class BitmapFlags;
	struct BitmapDescriptor;

	class BitmapManagerModule : public SCPModule<BitmapManagerModule>
	{
	public:
		static BitmapManagerModule ConstructModule() {};

		std::shared_ptr<BitmapDescriptor> LoadBitmap(SCP_string BitmapFilename);
		std::shared_ptr<BitmapDescriptor> CreateBitmap(std::uint32_t BitDepth, std::uint32_t Width, std::uint32_t Height,
									  SCP_buffer BitmapData,
							  BitmapFlags BmpFlags);
		//bool ReleaseBitmap(BitmapID BitmapToRelease);

		std::shared_ptr<BitmapDescriptor> LoadAnimation(SCP_string BitmapFilename, SCPCFilePathTypeID DirectoryType);
		//was GetInfo
		std::shared_ptr<BitmapDescriptor> LoadBitmap(BitmapID IDToQuery);

		//std::shared_ptr<BitmapData> LockBitmap(BitmapID BitmapToLock);

		bool StartupModule() override;

		void ShutdownModule() override;

	private:
		std::map<SCP_string, std::weak_ptr<BitmapDescriptor>> LoadedBitmaps;
	};

} // namespace SCP