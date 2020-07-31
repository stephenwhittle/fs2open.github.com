#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "cmdline/SCPCmdlineModule.h"
#include "cmdline/SCPCmdLine.h"

TEST_SUITE("Cmdline Module")
{
	tl::optional<SCPCmdlineModule&> ModuleHandle;
	TEST_CASE("Cmdline Module Init")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPCmdlineModule>();
		REQUIRE(ModuleHandle);
	}
	TEST_CASE("Cmdline Module has Options")
	{
		REQUIRE(ModuleHandle->CurrentOptions);
	}
	TEST_CASE("SCPCmdlineParser tests")
	{
		std::vector<std::string> TestArguments
		{ 
			"-window", 
			"-fxaa_preset", "6", 
			"-ambient_factor", "100", 
			"-spec_point", "1.2", 
			"-spec_static", "1.5", 
			"-ogl_spec", "20", 
			"-bloom_intensity","45", 
			"-bloom_intensity","50",
			"-mod","cfile"
		};
		
		SCPCmdLineParser Parser;
		SCPCmdLineOptions ParsedOptions = Parser.GetOptions(TestArguments);
		SUBCASE("Bool flag enabled by being present")
		{
			REQUIRE(ParsedOptions.bWindowed);
			REQUIRE(ParsedOptions.bWindowed.value());
		}
		SUBCASE("Missing bool flag not set")
		{
			REQUIRE_FALSE(ParsedOptions.b3dShipChoice);
		}
		SUBCASE("Integer value parsed correctly")
		{
			REQUIRE(ParsedOptions.FXAAPreset);
			REQUIRE(ParsedOptions.FXAAPreset.value() == 6);
		}
		SUBCASE("Floating-point value parsed correctly")
		{
			REQUIRE(ParsedOptions.SpecPoint);
			REQUIRE(ParsedOptions.SpecPoint.value() == doctest::Approx(1.2));
		}
		SUBCASE("Modlist parsed correctly")
		{
			REQUIRE(ParsedOptions.ModList );
			REQUIRE(ParsedOptions.ModList.value() == "cfile");
		}
		SUBCASE("Repeated options override earlier values by default")
		{
			REQUIRE(ParsedOptions.BloomIntensity);
			REQUIRE(ParsedOptions.BloomIntensity.value() == 50);
		}
	}
}