#pragma once
#include "module/SCPModuleBase.h"

#include "cmdline/SCPCmdlineModule.h"
#include "filesystem/SCPFilesystemModule.h"
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"

#include "SCPApplication.h"
#include <array>
#include "FSIntegerTypes.h"
#include "sqlite_orm/sqlite_orm.h"


class SCPCFileModule : public SCPModule<SCPCFileModule> 
{
	inline auto& CFileDatabase()
	{
		using namespace sqlite_orm;
		static auto& InternalCFileDatabase = make_storage(
			":memory:",
			make_table("FileInfo",
				make_column("ID", &SCPCFileInfo::uid, autoincrement(), primary_key(), unique()),
				make_column("NameAndExtension", &SCPCFileInfo::name_ext),
				make_column("RootID", &SCPCFileInfo::root_index),
				make_column("PathType", &SCPCFileInfo::pathtype_index),
				make_column("LastModified", &SCPCFileInfo::write_time),
				make_column("FileSize", &SCPCFileInfo::size),
				make_column("PackOffset", &SCPCFileInfo::pack_offset),
				make_column("FullPath", &SCPCFileInfo::real_name),
				make_column("Data", &SCPCFileInfo::data)),
			make_table("RootInfo",
				make_column("ID", &SCPRootInfo::uid, autoincrement(), primary_key(), unique()),
				make_column("Path", &SCPRootInfo::Path),
				make_column("Type", &SCPRootInfo::Type),
				make_column("LocationFlags", &SCPRootInfo::location_flags)
				));
		return InternalCFileDatabase;
	};


	template <unsigned int Index>
	constexpr static unsigned int CalculateCRCTableEntry()
	{
		unsigned int j   = 0;
		unsigned int crc = Index;
		for (j = 8; j > 0; j--) {
			if (crc & 1)
				crc = (crc >> 1) ^ 0xEDB88320;
			else
				crc >>= 1;
		}
		return crc;
	}
	template <typename T, T... Indices>
	constexpr std::array<unsigned int, sizeof...(Indices)>
	static GenerateCRCTableForSequence(std::integer_sequence<T, Indices...> int_seq)
	{
		return {CalculateCRCTableEntry<Indices>()...};
	}
	template <std::size_t size>
	constexpr static auto MakeCRCTable()
	{
		auto indices = std::make_integer_sequence<unsigned int, 256>{};
		return GenerateCRCTableForSequence(indices);
	}

	
	constexpr static std::array<uint, 256> CRCTable = MakeCRCTable<256>();

	int GetNextEmptyBlockIndex();

	tl::optional<CFILE&> GetNextEmptyBlock();

	CFILE* CFOpenFileFillBlock(const char* source, int line, FILE* fp, int type);
	CFILE* CFOpenInMemoryFileFillBlock(const char* source, int line, const void* data, size_t size, int dir_type);

	void BuildPackListForRoot(uint32_t RootID);
	void AddModRoots(const char* rootDirectory, SCPCFileLocationFlags basic_location);
public:
	static constexpr int MAX_CFILE_BLOCKS = 64;
	virtual bool StartupModule() override;
	virtual void ShutdownModule() override;
	
	class SCPPath UserDirectory;
	class SCPPath RootDirectory;
	//possibly make this static later
	constexpr uint cf_add_chksum_long(uint seed, ubyte* buffer, size_t size)
	{
		uint crc = seed;
		ubyte* p = buffer;

		while (size--)
			crc = (crc >> 8) ^ CRCTable[(crc ^ *p++) & 0xff];

		return crc;
	}

	void DumpOpenedFileList();
	//should be made private at best
	uint32_t AddRoot(class SCPRootInfo Root);
	//need to delete the copy constructor too
	static SCPCFileModule ConstructModule(SCPCmdlineModule& Dependency, SCPFilesystemModule& FSDependency)
	{
		SCPCFileModule ModuleInstance;
		ModuleInstance.RootDirectory = FSDependency.WorkingDirectory.GetCurrent();
		ModuleInstance.UserDirectory = SCPApplication::Get().GetConfigPath();
		return ModuleInstance;
	}
private:

	//may want to try to put this in a pointer or something so we don't need the full definition in the header
	//std::array<CFILE, MAX_CFILE_BLOCKS> Cfile_block_list{};
	//std::array<std::unique_ptr<class SCPCFileInfo>, 512 * 128>;
};
