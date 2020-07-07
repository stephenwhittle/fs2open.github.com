#include "cfile/SCPCFileModule.h"
#include "SCPApplication.h"
#include "cfile/cfile.h"
#include "FSOutputDeviceBase.h"

#include "module/SCPModuleManager.h"
#include "filesystem/SCPFilesystemModule.h"

bool SCPCFileModule::StartupModule() 
{
	static bool InitializationGuard = false;
	if (InitializationGuard)
	{
		return false;
	}

	SCPPath ExePath = SCPApplication::Get().ExecutablePath;
	if (ExePath.has_extension())
	{
		ExePath.remove_filename();
	}

	if (ExePath.root_path() == ExePath)
	{
		GOutputDevice->Message("Freespace2/FRED cannot be run from a drive root!");
	}
	
	tl::optional<SCPFilesystemModule&> FSModule = SCPModuleManager::GetModule<SCPFilesystemModule>();
	if (!FSModule)
	{
		return false;
	}
	else
	{
		FSModule->WorkingDirectory.Set(ExePath);
	}
	/*
	strcpy_s(Cfile_root_dir, buf);
	strcpy_s(Cfile_user_dir, SCPApplication::Get().GetConfigPath().c_str());

	// Initialize the block list with proper data
	Cfile_block_list.fill({});

	// 32 bit CRC table init
	cf_chksum_long_init();

	Cfile_cdrom_dir = cdrom_dir;
	cf_build_secondary_filelist(Cfile_cdrom_dir);
	*/

	InitializationGuard = true;
	return true;
}
