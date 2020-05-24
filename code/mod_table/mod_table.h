/*
 * Created by Hassan "Karajorma" Kazmi for the FreeSpace2 Source Code Project.
 * You may not sell or otherwise commercially exploit the source or things you
 * create based on the source.
 */


#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "globalincs/systemvars.h"



void mod_table_init();

/**
 * @brief Resets the mod values back to their default values
 *
 * This is mostly useful for the unit tests where mod value changes can interfere with other tests
 */
void mod_table_reset();

/**
 * @brief Checks if the mod specified support for the given engine version
 *
 * This is the function for implementing backwards-incompatible changes while not actually breaking backwards
 * compatibility. If you want to introduce a change that may impact compatibility with an earlier version (e.g. retail)
 * then you can use this function to check if the current mod targets a recent enough version.
 *
 * @param major The major version to check support for
 * @param minor The minor version to check support for
 * @param build The build version to check support for
 * @return @c true if the mod specified support for this or a later version, @c false otherwise
 */
bool mod_supports_version(int major, int minor, int build);
