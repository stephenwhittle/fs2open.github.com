#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "filesystem/SCPDirectoryIterator.h"
#include "filesystem/SCPFilesystemModule.h"
#include "filesystem/SCPFile.h"
#include "filesystem/SCPPath.h"

TEST_SUITE("Filesystem Module")
{
	tl::optional<SCPFilesystemModule&> ModuleHandle;

	TEST_CASE("Filesystem module init")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPFilesystemModule>();
		REQUIRE(ModuleHandle);
	}
	TEST_CASE("SCPFile")
	{
		SUBCASE("File Exists")
		{

		}
		SUBCASE("Read Only") {}
		SUBCASE("Last Modified") {}
		SUBCASE("File Size") {}

	}
	TEST_CASE("SCPPath")
	{
		//compare, case sensitive
		//compare, case insensitive
		//get filename component
		//Add extension if missing
		//create directories, filename included
		//create directories, no filename;
	}
	TEST_CASE("SCPDirectoryIterator")
	{

	}
}