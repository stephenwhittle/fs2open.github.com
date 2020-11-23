#include "config/ConfigProfile.h"
#include "config/SCPConfigModule.h"
#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "filesystem/SCPFile.h"

TEST_SUITE("Config Module")
{
	tl::optional<SCPConfigModule&> ModuleHandle;
	TEST_CASE("Config Module Init")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPConfigModule>();
		REQUIRE(ModuleHandle);
	}
	TEST_CASE("Reading value from default config returns empty value")
	{
		tl::optional<SCP_string> TestValue =
			ModuleHandle->ReadConfigValue<SCP_string>("SectionThatDoesntExist", "KeyThatDoesn'tExist");
		REQUIRE(!TestValue);
	}
	TEST_CASE("Writing string to config")
	{
		ModuleHandle->WriteConfigValue("DefaultSection", "TestString", "TestValue");
	}
	TEST_CASE("Writing uint32_t to config")
	{
		ModuleHandle->WriteConfigValue("DefaultSection", "TestInteger", 3);
	}
	TEST_CASE("Writing bool to config")
	{
		ModuleHandle->WriteConfigValue("DefaultSection", "TestBool", false);
	}
	TEST_CASE("Read string from config")
	{
		tl::optional<SCP_string> TestValue = ModuleHandle->ReadConfigValue<SCP_string>("DefaultSection", "TestString");
		REQUIRE(TestValue);
		REQUIRE(TestValue == "TestValue");
	}
	TEST_CASE("Read uint32_t from config")
	{
		tl::optional<uint32_t> TestValue = ModuleHandle->ReadConfigValue<uint32_t>("DefaultSection", "TestInteger");
		REQUIRE(TestValue);
		REQUIRE(TestValue == 3);
	}
	TEST_CASE("Read bool from config")
	{
		tl::optional<bool> TestValue = ModuleHandle->ReadConfigValue<bool>("DefaultSection", "TestBool");
		REQUIRE(TestValue);
		REQUIRE(TestValue == false);
	}
	TEST_CASE("Save and load to/from disk")
	{
		SUBCASE("Save")
		{
			SCP::ConfigProfile TestConfig("TestConfig.ini");

			ModuleHandle->WriteConfigValue("DefaultSection", "TestString", "TestValue");
			ModuleHandle->WriteConfigValue("DefaultSection", "TestInteger", 3);
			ModuleHandle->WriteConfigValue("DefaultSection", "TestBool", false);
		}

		SUBCASE("Load")
		{
			SCP::ConfigProfile TestConfig("TestConfig.ini");
			SUBCASE("String")
			{
				tl::optional<SCP_string> TestValue =
					ModuleHandle->ReadConfigValue<SCP_string>("DefaultSection", "TestString");
				REQUIRE(TestValue);
				REQUIRE(TestValue == "TestValue");
			}
			SUBCASE("Integer")
			{
				tl::optional<uint32_t> TestValue =
					ModuleHandle->ReadConfigValue<uint32_t>("DefaultSection", "TestInteger");
				REQUIRE(TestValue);
				REQUIRE(TestValue == 3);
			}
			SUBCASE("Bool")
			{
				tl::optional<bool> TestValue = ModuleHandle->ReadConfigValue<bool>("DefaultSection", "TestBool");
				REQUIRE(TestValue);
				REQUIRE(TestValue == false);
			}
			SCPFile::Delete("TestConfig.ini");
		}
	}
}