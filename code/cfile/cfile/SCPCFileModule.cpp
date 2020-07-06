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
	
	InitializationGuard = true;
	return true;
}
