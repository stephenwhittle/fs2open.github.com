#include "cfile/SCPCFileDatabase.h"

#include "cfile/cfilesystem.h"
#include "tl/optional.hpp"

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

SCPCFileDatabase::RootQuery SCPCFileDatabase::AllRootsOfType(SCPRootInfo::RootType Type)
{
	SQLite::Statement QueryRootsByType(InternalDB, R"(SELECT * from roots WHERE Type = ?)");
	QueryRootsByType.bind(static_cast<uint32_t>(Type));
	return RootQuery(std::move(QueryRootsByType));
}

SCPCFileDatabase::RootQuery SCPCFileDatabase::AllRoots()
{
	return RootQuery(std::move(SQLite::Statement(InternalDB, R"(SELECT * from roots)")));
}
