#pragma once
#include "FSStdTypes.h"
#include "cfile/SCPCFileDatabase.h"
#include "cfile/cfilesystem.h"

class SCPCFileInfo {

	friend class SCPCFileDatabase::StatementType;
	friend class SCPCFileDatabase;
	uint32_t uid;
	SCP_string name_ext;               // Filename and extension
	uint32_t root_index;               // Where in Roots this is located
	SCPCFilePathTypeID pathtype_index; // Where in Paths this is located
	time_t write_time;                 // When it was last written
	std::uintmax_t size;               // How big it is in bytes
	std::uintmax_t pack_offset; // For pack files, where it is at.   0 if not in a pack file.  This can be used to tell
								// if in a pack file.
	SCPPath real_name; // For real files, the full path
	const void* data;  // For in-memory files, the data pointer

	// private constructor for database
	SCPCFileInfo(uint32_t uid,
				 std::string NameExt,
				 uint32_t RootIndex,
				 uint32_t PathType,
				 uint32_t write_time,
				 std::uintmax_t size,
				 std::uintmax_t PackOffset,
				 std::string FullPath,
				 std::uintptr_t DataPtr)
		: uid(uid), name_ext(NameExt), root_index(RootIndex), pathtype_index(static_cast<SCPCFilePathTypeID>(PathType)),
		  write_time(write_time), size(size), pack_offset(PackOffset), real_name(FullPath), data((void*)DataPtr){};

  public:
	SCPCFileInfo(SCPPath FullPath, uint32_t RootUID, SCPCFilePathTypeID PathType) // real file
		: uid(0), root_index(RootUID), pathtype_index(PathType), pack_offset(0), real_name(FullPath), data(nullptr)
	{
		name_ext  = real_name.filename();
		SCPFile F = SCPFile(FullPath);
		if (F.Exists()) {
			size       = F.FileSize();
			write_time = F.LastModifiedTime(FullPath);
		}
	}

	// SCPCFileInfo(SCPPath Filename, uint32_t RootUID, SCPCFilePathTypeID PathType, size_t PackOffset); //file in pack
	// file

	template<typename T>
	SCPCFileInfo(SCPPath Filename, uint32_t RootUID, SCPCFilePathTypeID PathType, uintmax_t Size, T* DataPointer) //in-memory file
		:uid(0), name_ext(Filename.filename()), root_index(RootUID), pathtype_index(PathType), write_time(0), Size(Size), pack_offset(0), data(DataPointer) {};
};

