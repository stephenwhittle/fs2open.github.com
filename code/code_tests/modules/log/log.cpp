#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "log/SCPLogModule.h"
TEST_SUITE("SCPLogModule Tests")
{
	tl::optional<SCPLogModule&> ModuleHandle;

	TEST_CASE("SCPLogModule Init")
	{
		ModuleHandle = SCPModuleManager::GetModule<SCPLogModule>();
		REQUIRE(ModuleHandle);
	}
	TEST_CASE("LogPrintF Events")
	{
		ModuleHandle->LogPrintF(SCPLogType::Events, "TestEvent %s", "TestArgument");
	}
	TEST_CASE("LogPrintF Multi")
	{
		ModuleHandle->LogPrintF(SCPLogType::Multiplayer, "TestMulti %s", "TestArgument");
	}
}