#pragma once
#include <memory>
#include "cfile/cfilesystem.h"
#include "SQLiteCPP/SQLiteCpp.h"
#include "SQLiteCPP/VariadicBind.h"

class SCPCFileDatabase
{
	SQLite::Database InternalDB;
	SQLite::Statement AddRootStatement;
	SQLite::Statement AddFileStatement;
	SQLite::Statement GetRootByIDStatement;
	SQLite::Statement GetFileByIDStatement;

public:
	SCPCFileDatabase()
		:InternalDB(":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE),
		AddRootStatement(InternalDB, R"(INSERT INTO roots VALUES ?,?,?,?)"),
		AddFileStatement(InternalDB, R"(INSERT INTO files VALUES ?,?,?,?,?,?,?,?,?)"),
		GetRootByIDStatement(InternalDB, R"(SELECT * from roots WHERE uid = ?)"),
		GetFileByIDStatement(InternalDB, R"(SELECT * from files WHERE uid = ?)")
    {
		InternalDB.exec( R"(
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
		
	}

	uint32_t AddRoot(SCPRootInfo NewRoot)
	{
		bind(AddRootStatement, NewRoot.uid, NewRoot.Path.string(), static_cast<uint32_t>(NewRoot.Type), NewRoot.location_flags.RawValue());
		AddRootStatement.exec();
		AddRootStatement.reset();
		AddRootStatement.clearBindings();
		return InternalDB.getLastInsertRowid();
	}
	uint32_t AddFile(SCPCFileInfo NewFile)
	{
		bind(AddFileStatement, NewFile.uid, NewFile.name_ext, NewFile.root_index, static_cast<uint32_t>(NewFile.pathtype_index), NewFile.write_time, NewFile.size, NewFile.pack_offset, NewFile.real_name.string(), (uintptr_t)NewFile.data);
		AddFileStatement.exec();
		AddFileStatement.reset();
		AddFileStatement.clearBindings();
		return InternalDB.getLastInsertRowid();
	}
	tl::optional<SCPRootInfo> GetRootByID(uint32_t RootUID)
	{
		GetRootByIDStatement.bind(0, RootUID);
		if (GetRootByIDStatement.executeStep())
		{
			return GetRootByIDStatement.getColumns<SCPRootInfo, 4>();
		}
		return {};
	}
	tl::optional<SCPCFileInfo> GetFileByID(uint32_t FileUID)
	{
		GetFileByIDStatement.bind(0, FileUID);
		if (GetFileByIDStatement.executeStep()) {
			return GetFileByIDStatement.getColumns<SCPCFileInfo, 9>();
		}
		return {};
	}
};

