#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPRootInfo.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/cfilesystem.h"
#include "filesystem/SCPPath.h"
#include "tl/optional.hpp"
#include "../sqlite3/sqlite3.h"
#include "SQLiteCpp/VariadicBind.h"
#include "SCPProfiling.h"

 SCPCFileDatabase::SCPCFileDatabase()
	: InternalDB(":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE)
{
	InternalDB.exec(R"(
	CREATE TABLE roots 
	(
		uid INTEGER PRIMARY KEY AUTOINCREMENT,
		Path TEXT,
		Type INTEGER,
		LocationFlags INTEGER
	)	
	)");

	InternalDB.exec(R"(
	CREATE TABLE files 
	(
		uid INTEGER PRIMARY KEY AUTOINCREMENT,
		NameExt TEXT,
		RootUID INTEGER,
		PathType INTEGER,
		WriteTime INTEGER,
		Size INTEGER,
		PackOffset INTEGER,
		FullPath TEXT,
		DataPtr INTEGER
	)	
	)");

	
	
	
	

	/// CUSTOM SQL FUNCTIONS
	auto HasFlag = [](sqlite3_context* context, int argc, sqlite3_value** argv) {
		if (argc == 2) {
			int Value       = sqlite3_value_int(argv[0]);
			int DesiredFlag = sqlite3_value_int(argv[1]);

			if ((Value & DesiredFlag) == DesiredFlag) {
				sqlite3_result_int(context, 1);
			} else {
				sqlite3_result_int(context, 0);
			}
		}
		sqlite3_result_error(context, "Invalid number of arguments to HAS_FLAG", -1);
	};

	sqlite3_create_function(InternalDB.getHandle(),
							"HAS_FLAG",
							2,
							SQLITE_UTF8 | SQLITE_DETERMINISTIC,
							nullptr,
							HasFlag,
							nullptr,
							nullptr);

	auto DirFlagFilter = [](sqlite3_context* context, int argc, sqlite3_value** argv) {
		if (argc == 2) {
			SCPCFileLocationFlags Value(static_cast<SCPCFileLocation>(sqlite3_value_int(argv[0])));
			SCPCFileLocationFlags DesiredFlags(static_cast<SCPCFileLocation>(sqlite3_value_int(argv[1])));

			if (SCPCFileModule::CheckLocationFlags(Value, DesiredFlags)) {
				sqlite3_result_int(context, 1);
			} else {
				sqlite3_result_int(context, 0);
			}
		}
		sqlite3_result_error(context, "Invalid number of arguments to DIR_FILTER", -1);
	};

	sqlite3_create_function(InternalDB.getHandle(),
							"DIR_FILTER",
							2,
							SQLITE_UTF8 | SQLITE_DETERMINISTIC,
							nullptr,
							DirFlagFilter,
							nullptr,
							nullptr);
	auto ExtensionFilter = [](sqlite3_context* context, int argc, sqlite3_value** argv) {
		if (argc < 2)
		{
			sqlite3_result_error(context, "Invalid number of arguments to EXT_FILTER", -1);
		}
		size_t FilenameLength = sqlite3_value_bytes(argv[0]);
		const char* RawFileName = (const char*) sqlite3_value_text(argv[0]);
		SCPPath Filename = SCPPath::FromU8(std::string(RawFileName, RawFileName + FilenameLength));
		if (Filename.has_extension())
		{
			for (int ExtensionIndex = 1; ExtensionIndex < argc; ExtensionIndex++)
			{
				size_t ExtensionLength   = sqlite3_value_bytes(argv[ExtensionIndex]);
				const char* RawExtension = (const char*)sqlite3_value_text(argv[ExtensionIndex]);
				if (Filename.extension().string() == std::string(RawExtension, RawExtension + ExtensionLength))
				{
					sqlite3_result_int(context, 1);
				}
			}
		}
		sqlite3_result_int(context, 0);
	};

	sqlite3_create_function(InternalDB.getHandle(), "EXT_FILTER", -1, SQLITE_UTF8, nullptr, ExtensionFilter, nullptr, nullptr);

	auto RegexMatch = [](sqlite3_context* context, int argc, sqlite3_value** argv) {
		if (argc < 2) {
			sqlite3_result_error(context, "Invalid number of arguments to REGEXP", -1);
		}
		size_t PatternLength   = sqlite3_value_bytes(argv[0]);
		const char* Pattern = (const char*)sqlite3_value_text(argv[0]);

		size_t InputLength = sqlite3_value_bytes(argv[1]);
		const char* Input = (const char*)sqlite3_value_text(argv[1]);
	};

	sqlite3_create_function(InternalDB.getHandle(), "regexp", -1, SQLITE_UTF8, nullptr, RegexMatch, nullptr, nullptr);
}

uint32_t SCPCFileDatabase::AddRoot(SCPRootInfo NewRoot)
{
	if (NewRoot.Type != SCPRootType::InMemory) {
		GOutputDevice->Message("Adding Root %s\r\n", NewRoot.Path.string());
	}
	else
	{
		GOutputDevice->Message("Adding In-memory Root");
	}
	auto AddRootStatement = SQLite::Statement(InternalDB, R"(INSERT INTO roots (Path, Type, LocationFlags) VALUES (?,?,?); )");
	SQLite::bind(AddRootStatement,
		 NewRoot.Path.string(),
		 static_cast<uint32_t>(NewRoot.Type),
		 NewRoot.location_flags.RawValue());
	AddRootStatement.exec();
	AddRootStatement.reset();
	AddRootStatement.clearBindings();
	return InternalDB.getLastInsertRowid();
}

uint32_t SCPCFileDatabase::AddFile(SCPCFileInfo NewFile)
{
	FS2_PROF_EVENT();
	GOutputDevice->Message("Adding File %s\r\n", NewFile.GetFullPath());
	auto AddFileStatement = SQLite::Statement(InternalDB, R"(INSERT INTO files (NameExt, RootUID, PathType, WriteTime, Size, PackOffset, FullPath, DataPtr) VALUES (?,?,?,?,?,?,?,?); )");

	SQLite::bind(AddFileStatement,
		 NewFile.name_ext,
		 NewFile.root_index,
		 static_cast<uint32_t>(NewFile.pathtype_index),
		 NewFile.write_time,
		 static_cast<intmax_t>(NewFile.size),
		 static_cast<intmax_t>(NewFile.pack_offset),
		 NewFile.real_name.string(),
		 (uintptr_t)NewFile.data);
	AddFileStatement.exec();
	AddFileStatement.reset();
	AddFileStatement.clearBindings();
	return InternalDB.getLastInsertRowid();
}

tl::optional<SCPRootInfo> SCPCFileDatabase::GetRootByID(uint32_t RootUID)
{
	FS2_PROF_EVENT();
	auto GetRootByIDStatement = SQLite::Statement(InternalDB, R"(SELECT * from roots WHERE uid = ?)");
	GetRootByIDStatement.bind(1, RootUID);
	if (GetRootByIDStatement.executeStep()) {
		return GetRootByIDStatement.getColumns<SCPRootInfo, 4>();
	}
	return {};
}

tl::optional<SCPCFileInfo> SCPCFileDatabase::GetFileByID(uint32_t FileUID)
{
	FS2_PROF_EVENT();
	auto GetFileByIDStatement = SQLite::Statement(InternalDB, R"(SELECT * from files WHERE uid = ?)");
	GetFileByIDStatement.bind(1, FileUID);
	if (GetFileByIDStatement.executeStep()) {
		return GetFileByIDStatement.getColumns<SCPCFileInfo, 9>();
	}
	return {};
}

SCPCFileDatabase::RootQuery SCPCFileDatabase::AllRootsOfType(SCPRootType Type)
{
	SQLite::Statement QueryRootsByType(InternalDB, R"(SELECT * from roots WHERE Type = ?)");
	QueryRootsByType.bind(1,static_cast<uint32_t>(Type));
	//QueryRootsByType.exec();
	return RootQuery(std::move(QueryRootsByType));
}

SCPCFileDatabase::RootQuery SCPCFileDatabase::AllRoots()
{
	return RootQuery(std::move(SQLite::Statement(InternalDB, R"(SELECT * from roots)")));
}

SCPCFileDatabase::FileQuery SCPCFileDatabase::AllFilesWhere(std::string WhereClause) 
{
	return FileQuery(std::move(SQLite::Statement(InternalDB, "SELECT * from files WHERE" + WhereClause)));
}


