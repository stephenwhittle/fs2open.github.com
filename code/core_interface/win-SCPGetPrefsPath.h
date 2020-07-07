#pragma once
#include "FSStdTypes.h"
#include "FSAssert.h"

#include "ShlObj.h"
#undef max

inline SCP_string SCPGetPrefsPath()
{
    SCP_buffer path(MAX_PATH);
    SHGetFolderPathA(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, path.Data());
    return SCP_string(path.Data());
}