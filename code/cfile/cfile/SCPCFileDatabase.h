#pragma once
#include <memory>
#include "SQLiteCPP/SQLiteCpp.h"
#include "SQLiteCPP/VariadicBind.h"
#include "FSStdTypes.h"
#include "SCPCFilePathType.h"
#include "cfile/SCPCFile.h"
#include "sql.h"
#include "tl/optional.hpp"
#include "fmt/ranges.h"
enum class SCPRootType;

template<typename DataType, uint32_t NumFields>
class DBQuery;

template<typename DataType, uint32_t NumFields>
class DBResultIterator
{
	DBQuery<DataType, NumFields>* const QueryBeingIterated = nullptr;
	bool HasMoreResults = false;

public:
	DBResultIterator(DBQuery<DataType, NumFields>* Query)
		:QueryBeingIterated(Query)
	{
		HasMoreResults = true;
	}
	DBResultIterator()
	{
		HasMoreResults = false;
	}
	DataType operator*()
	{
		return *(*QueryBeingIterated);
	}
	bool operator != (const DBResultIterator& Other) const
	{
		if (QueryBeingIterated == nullptr || Other.QueryBeingIterated == nullptr)
		{
			return HasMoreResults != Other.HasMoreResults;
		}
		else
		{
			return HasMoreResults == Other.HasMoreResults || QueryBeingIterated == Other.QueryBeingIterated;
		}
	}
	void operator++()
	{
		if (QueryBeingIterated)
		{
			HasMoreResults = ++(*QueryBeingIterated);
		}
		else
		{
			HasMoreResults = false;
		}
	}
};


template<typename DataType, uint32_t NumFields>
class DBQuery
{
	SQLite::Statement InternalStatement;
public:
	DBQuery(SQLite::Statement&& QueryStatement)
		: InternalStatement(std::move(QueryStatement)) 
	{
		InternalStatement.executeStep();
	};
	DataType operator*()
	{
		return InternalStatement.getColumns<DataType, NumFields>();
	}
	bool operator++()
	{
		auto QueryStatus = InternalStatement.tryExecuteStep();
		//Dirty hack because SQLiteCPP does not define the return codes
		if ( QueryStatus == SQLite::OK || QueryStatus == 100) {
			return true;
		}
		else {
			return false;
		}
	}
	
	DBResultIterator<DataType, NumFields> begin()
	{
		DBResultIterator<DataType, NumFields> Iterator(this);
		return Iterator;
	}

	DBResultIterator<DataType, NumFields> end() const
	{
		return DBResultIterator<DataType, NumFields>();
	}
};


class FileFilter : sql::SelectModel {
  public:
	using sql::SelectModel::limit;
	using sql::SelectModel::order_by;
	using sql::SelectModel::select;
	using sql::SelectModel::str;
	using sql::SelectModel::where;
	FileFilter() : sql::SelectModel() { select("*").from("files"); }
	FileFilter& FilenameIs(SCP_string Filename)
	{
		where(sql::column("NameExt") == Filename);
		return *this;
	}
	FileFilter& FullPathIs(SCP_string FullPath)
	{
		where(sql::column("FullPath") == FullPath);
		return *this;
	}
	FileFilter& PathTypeIs(SCPCFilePathTypeID PathType)
	{
		where(sql::column("PathType") == static_cast<uint32_t>(PathType));
		return *this;
	}
	FileFilter& InRoot(uint32_t RootUID)
	{
		where(sql::column("RootUID") == RootUID);
		return *this;
	}
	FileFilter& LocationMatches(SCPCFileLocationFlags LocationFilter)
	{
		where(fmt::format("RootUID IN (select uid from roots where DIR_FILTER(LocationFlags, {}) = 1)", LocationFilter.RawValue()));
		return *this;
	}
	FileFilter& SortByFilenameAscending(bool Ascending)
	{
		order_by(fmt::format("NameExt {}", Ascending ? "ASC" : "DESC"));
		return *this;
	}
	FileFilter& SortByPathTypeAscending(bool Ascending) 
	{ 
		order_by(fmt::format("PathType {}", Ascending ? "ASC" : "DESC"));
		return *this;
	}
	FileFilter& ExtensionIsOneOf(std::vector<std::string> Extensions)
	{
		std::string ExtensionList = fmt::format("{},", Extensions);
		ExtensionList.pop_back();
		where(fmt::format("EXT_FILTER(NameExt, {})", ExtensionList));
		return *this;
	}
	FileFilter& ExtensionMatchesRegex(std::string Regexp)
	{
		where(fmt::format("NameExt REGEXP {}", Regexp));
		return *this;
	}
};

class RootFilter : sql::SelectModel {
  public:
	using sql::SelectModel::limit;
	using sql::SelectModel::order_by;
	using sql::SelectModel::select;
	using sql::SelectModel::str;
	using sql::SelectModel::where;
	using sql::SelectModel::SelectModel;
	RootFilter() : sql::SelectModel() { select("*").from("roots"); }
	RootFilter(RootFilter& Other) = default;
	RootFilter& TypeIs(SCPRootType RootType)
	{
		where(sql::column("Type") == static_cast<uint32_t>(RootType));
		return *this;
	}

	RootFilter& LocationMatches(SCPCFileLocationFlags LocationFilter)
	{
		where(fmt::format("DIR_FILTER(LocationFlags, {}) = 1", LocationFilter.RawValue()));
		return *this;
	}
};

class SCPCFileDatabase
{
	SQLite::Database InternalDB;
	

public:
	using StatementType = SQLite::Statement;
	
	SCPCFileDatabase();

	uint32_t AddRoot(class SCPRootInfo NewRoot);
	uint32_t AddFile(class SCPCFileInfo NewFile);
	tl::optional<SCPRootInfo> GetRootByID(uint32_t RootUID);

	tl::optional<SCPCFileInfo> GetFileByID(uint32_t FileUID);
	using RootQuery = DBQuery<SCPRootInfo, 4>;
	RootQuery AllRootsOfType(SCPRootType Type);
	RootQuery AllRoots();
	using FileQuery = DBQuery<SCPCFileInfo, 9>;
	FileQuery AllFilesWhere(std::string WhereClause);

	FileQuery Files(class FileFilter& Filter)
	{
		return FileQuery(SQLite::Statement(InternalDB, Filter.str()));
	}
	RootQuery Roots(class RootFilter& Filter)
	{
		return RootQuery(SQLite::Statement(InternalDB, Filter.str()));
	}
};
