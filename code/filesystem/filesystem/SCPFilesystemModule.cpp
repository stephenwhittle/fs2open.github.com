#include "filesystem/SCPFilesystemModule.h"
#include "SCPApplication.h"
#include "FSOutputDeviceBase.h"

bool SCPFilesystemModule::StartupModule() 
{ 
	//SCPPath ExePath = SCPApplication::Get().ExecutablePath;
	
	SCPPath CurrentWorkingDirectory = WorkingDirectory.GetCurrent();
	if (CurrentWorkingDirectory.root_path() == CurrentWorkingDirectory) {
		GOutputDevice->Message("Freespace2/FRED cannot be run from a drive root!");
		return false;
	}
	GOutputDevice->Message("Running Freespace2/FRED from %s", CurrentWorkingDirectory);
	return true; 
}
