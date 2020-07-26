#pragma once
#include "FSStdTypes.h"
#include "cfile/SCPCFileDatabase.h"
#include "cfile/cfilesystem.h"
#include "filesystem/SCPFile.h"

class SCPCFileInfo {

	
	uint32_t uid;
	SCP_string name_ext;               // Filename and extension
	uint32_t root_index;               // Where in Roots this is located
	SCPCFilePathTypeID pathtype_index; // Where in Paths this is located
	time_t write_time;                 // When it was last written
	std::uintmax_t size;               // How big it is in bytes
	std::uintmax_t pack_offset; // For pack files, where it is at.   0 if not in a pack file.  This can be used to tell
								// if in a pack file.
	SCPPath real_name; // For real files, the full path
	void* data;  // For in-memory files, the data pointer

	// private constructor for database
	SCPCFileInfo(uint32_t uid,
				 std::string NameExt,
				 uint32_t RootIndex,
				 uint32_t PathType,
				 uint32_t write_time,
				 std::intmax_t size,
				 std::intmax_t PackOffset,
				 std::string FullPath,
				 std::intptr_t DataPtr)
		: uid(uid), name_ext(NameExt), root_index(RootIndex), pathtype_index(static_cast<SCPCFilePathTypeID>(PathType)),
		  write_time(write_time), size(std::uintmax_t(size)), pack_offset(std::uintmax_t(PackOffset)), real_name(FullPath), data((void*)DataPtr){};

  public:
	friend class SCPCFileDatabase::StatementType;
	friend class SCPCFileDatabase;
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
	
	SCPCFileInfo(SCPPath Filename, uint32_t RootUID, SCPCFilePathTypeID PathType, time_t WriteTime, std::uintmax_t Size, std::uintmax_t PackOffset) //file in pack
		:uid(0), name_ext(Filename), root_index(RootUID), pathtype_index(PathType), write_time(WriteTime), size(Size), pack_offset(PackOffset), data(nullptr) 
	{
		real_name = PathTypes.at(PathType).Path / Filename;
	};

	template<typename T>
	SCPCFileInfo(SCPPath Filename, uint32_t RootUID, SCPCFilePathTypeID PathType, uintmax_t Size, T* DataPointer) //in-memory file
		:uid(0), name_ext(Filename.filename()), root_index(RootUID), pathtype_index(PathType), write_time(0), size(Size), pack_offset(0), data((void*)DataPointer) {};

	uint32_t GetAssociatedRootID() const { return uid; }
	SCPPath GetFullPath() const { return real_name; }
	uintmax_t GetFileSize() const { return size; }
	uintmax_t GetPackFileOffset() const { return pack_offset; }
	void* const GetDataPointer() const { return data; }
};

