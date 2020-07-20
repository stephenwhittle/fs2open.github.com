#pragma once
#include <memory>
#include "SQLiteCPP/SQLiteCpp.h"
#include "SQLiteCPP/VariadicBind.h"
#include "tl/optional.hpp"
enum class SCPRootType;

template <typename DataType, uint32_t NumFields>
class DBResultIterator;

template<typename DataType, uint32_t NumFields>
class DBQuery
{
	SQLite::Statement InternalStatement;
public:
	DBQuery(SQLite::Statement&& QueryStatement)
		: InternalStatement(QueryStatement) {};
	auto operator*() const
	{
		return InternalStatement.getColumns<DataType, NumFields>();
	}
	bool operator++()
	{
		if (SQLite::OK == InternalStatement.tryExecuteStep()) {
			return true;
		}
		else {
			return false;
		}
	}
	
	auto begin()
	{
		return DBResultIterator<DataType, NumFields>(*this);
	}

	auto end()
	{
		return DBResultIterator<DataType, NumFields>();
	}
};

template<typename DataType, uint32_t NumFields>
class DBResultIterator
{
	class DBQuery<DataType, NumFields>* Query = nullptr;
	bool RecordsRemaining = false;
public:
	DBResultIterator(DBQuery<DataType, NumFields>& Query)
		: Query(&Query)
	{
		
	}
	DBResultIterator()
	{
	}
	bool operator==(DBResultIterator const& Other)
	{
		return RecordsRemaining == Other.RecordsRemaining;
	}
	bool operator!=(DBResultIterator const& Other)
	{
		return RecordsRemaining != Other.RecordsRemaining;
	}
	DBResultIterator& operator++()
	{
		if (Query == nullptr)
		{
			RecordsRemaining = false;
			return *this;
		}
		RecordsRemaining = Query++;

	}

	auto operator*() const
	{
		return *Query;
	}
};


class SCPCFileDatabase
{
	SQLite::Database InternalDB;
	SQLite::Statement AddRootStatement;
	SQLite::Statement AddFileStatement;
	SQLite::Statement GetRootByIDStatement;
	SQLite::Statement GetFileByIDStatement;

public:
	using StatementType = SQLite::Statement;
	
	SCPCFileDatabase()
		:InternalDB(":memory:", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE),
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

	};

	uint32_t AddRoot(class SCPRootInfo NewRoot);
	uint32_t AddFile(class SCPCFileInfo NewFile);
	tl::optional<SCPRootInfo> GetRootByID(uint32_t RootUID);

	tl::optional<SCPCFileInfo> GetFileByID(uint32_t FileUID);
	using RootQuery = DBQuery<SCPRootInfo, 4>;
	RootQuery AllRootsOfType(SCPRootType Type);
	RootQuery AllRoots();

};

