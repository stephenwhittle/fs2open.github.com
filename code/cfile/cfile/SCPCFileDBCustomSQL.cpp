
//Relative path to header file as a dirty hack around SQLiteCpp insisting I install their lib in order to access the headers
#include "../sqlite3/sqlite3.h"
#include "cfile/SCPCFileModule.h"




void HasFlag(sqlite3_context* context, int argc, sqlite3_value** argv)
{
	if (argc == 2)
	{
		int Value = sqlite3_value_int(argv[0]);
		int DesiredFlag = sqlite3_value_int(argv[1]);

		if ((Value & DesiredFlag) == DesiredFlag)
		{
			sqlite3_result_int(context, 1);
		}
		else
		{
			sqlite3_result_int(context, 0);
		}
	}
	sqlite3_result_error(context, "Invalid number of arguments to HAS_FLAG", -1);
}

void DirFlagMatch(sqlite3_context* context, int argc, sqlite3_value** argv)
{
	if (argc == 2)
	{
		SCPCFileLocationFlags Value(static_cast<SCPCFileLocation>(sqlite3_value_int(argv[0])));
		SCPCFileLocationFlags DesiredFlags(static_cast<SCPCFileLocation>(sqlite3_value_int(argv[1])));

		if (SCPCFileModule::CheckLocationFlags(Value, DesiredFlags))
		{
			sqlite3_result_int(context, 1);
		}
		else
		{
			sqlite3_result_int(context, 0);
		}
	}
	sqlite3_result_error(context, "Invalid number of arguments to DIR_FILTER", -1);
}