#include "doctest/doctest.h"
#include "module/SCPModuleManager.h"
#include "log/SCPLogModule.h"
TEST_SUITE("SCPLogModule Tests")
{
	tl::optional<SCPLogModule&> LogModuleHandle;

	TEST_CASE("SCPLogModule Init")
	{
		LogModuleHandle = SCPModuleManager::GetModule<SCPLogModule>();
		REQUIRE(LogModuleHandle);
	}
	TEST_CASE("LogPrintF Events")
	{
		LogModuleHandle->LogPrintF(SCPLogType::Events, "TestEvent %s", "TestArgument");
	}
	TEST_CASE("LogPrintF Multi")
	{
		LogModuleHandle->LogPrintF(SCPLogType::Multiplayer, "TestMulti %s", "TestArgument");
	}
}