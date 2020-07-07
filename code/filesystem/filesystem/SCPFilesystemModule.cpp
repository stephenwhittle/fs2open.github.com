#include "filesystem/SCPFilesystemModule.h"
#include "SCPApplication.h"
#include "FSOutputDeviceBase.h"

bool SCPFilesystemModule::StartupModule() 
{ 
	SCPPath ExePath = SCPApplication::Get().ExecutablePath;

	if (ExePath.has_extension()) {
		ExePath.remove_filename();
	}

	if (ExePath.root_path() == ExePath) {
		GOutputDevice->Message("Freespace2/FRED cannot be run from a drive root!");
	}

	WorkingDirectory.Set(ExePath);
	return true; 
}
