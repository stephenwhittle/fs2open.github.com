#include "bmpman/SCPBitmapManager.h"
#include "bmpman/SCPBitmapTypes.h"
#include "cfile/SCPCFileModule.h"
#include "module/SCPModuleManager.h"
#include "cfile/SCPCFileInfo.h"

#undef LoadBitmap

std::shared_ptr < SCP::BitmapDescriptor> SCP::BitmapManagerModule::LoadBitmap(SCP_string BitmapFilename)
{
	// if we are in standalone mode
	// then load the 'right_bracket' bitmap
	
	if (LoadedBitmaps.count(BitmapFilename)) {
		std::shared_ptr<SCP::BitmapDescriptor> Bitmap = LoadedBitmaps[BitmapFilename].lock();
		if (Bitmap)
		{
			return Bitmap;
		}
		else
		{
			tl::optional<SCPCFileModule&> CFileModule = SCPModuleManager::GetModule<SCPCFileModule>();
			auto BitmapCFile = CFileModule->FindFileInfo(BitmapFilename, SCPCFilePathTypeID::Any);
			if (BitmapCFile)
			{
				CFileModule->CFileOpen(BitmapCFile.value(), {SCPCFileMode::Read});
				std::shared_ptr<SCP::BitmapDescriptor> NewBitmap = std::make_shared<SCP::BitmapDescriptor>();
				// load the data here
				LoadedBitmaps[BitmapFilename] = NewBitmap;
				return NewBitmap;
			}
		}
	}


	

	
}

std::shared_ptr <
	SCP::BitmapDescriptor> SCP::BitmapManagerModule::CreateBitmap(std::uint32_t BitDepth, std::uint32_t Width,
																 std::uint32_t Height,
													 SCP_buffer BitmapData, SCP::BitmapFlags BmpFlags)
{}


std::shared_ptr<SCP::BitmapDescriptor> SCP::BitmapManagerModule::LoadAnimation(SCP_string BitmapFilename,
															  SCPCFilePathTypeID DirectoryType)
{}

std::shared_ptr<SCP::BitmapDescriptor> SCP::BitmapManagerModule::LoadBitmap(SCP::BitmapID IDToQuery) {}


bool SCP::BitmapManagerModule::StartupModule()
{
	return true;
}

void SCP::BitmapManagerModule::ShutdownModule()
{
	for (auto LoadedBitmapInfo : LoadedBitmaps)
	{
		if (LoadedBitmapInfo.second.lock())
		{
			//log output indicating that a bitmap was not unloaded
		}
	}
}
