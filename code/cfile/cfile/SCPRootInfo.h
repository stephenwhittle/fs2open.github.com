#pragma once
#include "filesystem/SCPPath.h"
#include "cfile/SCPCFileDatabase.h"

class SCPRootInfo {

  public:
	enum class RootType { Path, PackFile, InMemory };

	SCPRootInfo(SCPPath RootPath, RootType Type, SCPCFileLocationFlags LocationFlags)
		: uid(0), Path(RootPath), Type(Type), location_flags(LocationFlags){};
	// Temporary getters, may make the class immutable instead
	SCPPath GetPath() { return Path; }
	SCPCFileLocationFlags GetLocationFlags() { return location_flags; }
	uint32_t GetUID() { return uid; }
	RootType GetType() { return Type; }
	friend class SCPCFileDatabase::StatementType;
	friend class SCPCFileDatabase;

  private:
	uint32_t uid;
	SCPPath Path;  // Contains something like c:\projects\freespace or
				   // c:\projects\freespace\freespace.vp
	RootType Type; // CF_ROOTTYPE_PATH  = Path, CF_ROOTTYPE_PACK =Pack file, CF_ROOTTYPE_MEMORY=In memory
	SCPCFileLocationFlags location_flags;

	// private constructor to allow database to return instances
	SCPRootInfo(uint32_t uid, std::string Path, uint32_t Type, uint32_t LocationFlags)
		: uid(uid), Path(Path), Type(static_cast<RootType>(Type)),
		  location_flags(static_cast<SCPCFileLocation>(LocationFlags))
	{
	}
};


