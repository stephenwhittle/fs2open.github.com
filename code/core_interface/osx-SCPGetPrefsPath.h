#pragma once
#include "FSStdTypes.h"
#include "FSAssert.h"
#include <sysdir.h>
inline SCP_string SCPGetPrefsPath()
{
    sysdir_search_path_enumeration_state SearchState;
    SearchState = sysdir_start_search_path_enumeration(SYSDIR_DIRECTORY_APPLICATION_SUPPORT, SYSDIR_DOMAIN_MASK_USER);
    SCP_buffer PathBuffer = SCP_buffer(PATH_MAX);
    Assertion(sysdir_get_next_search_path_enumeration(state,PathBuffer.Data() ), "Unable to get preferences path!");
    
    return SCP_string(SCP_buffer.Data());
}