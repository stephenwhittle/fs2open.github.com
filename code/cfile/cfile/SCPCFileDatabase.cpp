#include "cfile/SCPCFileDatabase.h"
#include "cfile/SCPRootInfo.h"
#include "cfile/SCPCFileInfo.h"
#include "cfile/SCPCFileModule.h"
#include "cfile/cfilesystem.h"
#include "tl/optional.hpp"
#include "../sqlite3/sqlite3.h"

 SCPCFileDatabase::SCPCFileDatabase()
	: InternalDB(":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE),
	  AddRootStatement(InternalDB, R"(INSERT INTO roots VALUES ?,?,?,?)"),
	  AddFileStatement(InternalDB, R"(INSERT INTO files VALUES ?,?,?,?,?,?,?,?,?)"),
	  GetRootByIDStatement(InternalDB, R"(SELECT * from roots WHERE uid = ?)"),
	  GetFileByIDStatement(InternalDB, R"(SELECT * from files WHERE uid = ?)")
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
}

uint32_t SCPCFileDatabase::AddRoot(SCPRootInfo NewRoot)
{
	bind(AddRootStatement,
		 NewRoot.uid,
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
	bind(AddFileStatement,
		 NewFile.uid,
		 NewFile.name_ext,
		 NewFile.root_index,
		 static_cast<uint32_t>(NewFile.pathtype_index),
		 NewFile.write_time,
		 NewFile.size,
		 NewFile.pack_offset,
		 NewFile.real_name.string(),
		 (uintptr_t)NewFile.data);
	AddFileStatement.exec();
	AddFileStatement.reset();
	AddFileStatement.clearBindings();
	return InternalDB.getLastInsertRowid();
}

tl::optional<SCPRootInfo> SCPCFileDatabase::GetRootByID(uint32_t RootUID)
{
	GetRootByIDStatement.bind(0, RootUID);
	if (GetRootByIDStatement.executeStep()) {
		return GetRootByIDStatement.getColumns<SCPRootInfo, 4>();
	}
	return {};
}

tl::optional<SCPCFileInfo> SCPCFileDatabase::GetFileByID(uint32_t FileUID)
{
	GetFileByIDStatement.bind(0, FileUID);
	if (GetFileByIDStatement.executeStep()) {
		return GetFileByIDStatement.getColumns<SCPCFileInfo, 9>();
	}
	return {};
}

SCPCFileDatabase::RootQuery SCPCFileDatabase::AllRootsOfType(SCPRootType Type)
{
	SQLite::Statement QueryRootsByType(InternalDB, R"(SELECT * from roots WHERE Type = ?)");
	QueryRootsByType.bind(static_cast<uint32_t>(Type));
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


