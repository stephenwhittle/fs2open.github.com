#pragma once
#include "module/SCPModuleBase.h"

#include "cmdline/SCPCmdlineModule.h"
#include "filesystem/SCPFilesystemModule.h"
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"
#include "cfile/SCPCFileDatabase.h"
#include "SCPApplication.h"
#include <array>
#include "FSIntegerTypes.h"
#include <memory>


class SCPCFileModule : public SCPModule<SCPCFileModule> 
{
	//cfiles themselves may need a table too
	//rootID, fileID, uintptr_t to a vector of unique/shared ptrs
	inline auto& CFileDatabase()
	{
		static SCPCFileDatabase Database;
		return Database;
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
	//this may be better off taking in an actual root or rootid
	void AddModRoots(const char* rootDirectory, SCPCFileLocationFlags basic_location);
	void BuildRootList(const char* cdrom_dir);
	void BuildCFileDatabase(const char* cdrom_dir);
	void BuildFileList();
	void AddFilesFromRoot(SCPRootInfo Root);
	int GetDefaultFilePath(char* path, uint path_max, int pathtype, const char* filename = nullptr, bool localize = false, SCPCFileLocationFlags location_flags = SCPCFileLocationALL, SCP_string LanguagePrefix = "");
	void PopulateFilesInMemoryRoot(uint32_t RootID); 
	void PopulateFilesInPackFile(uint32_t RootID);
	void PopulateLooseFilesInRoot(uint32_t RootID);
	
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
	static bool CheckLocationFlags(const SCPCFileLocationFlags FlagsToCheck, const SCPCFileLocationFlags DesiredFlags) ;

	//CFILE destructor should be responsible for calling fstream/close things
	std::unique_ptr<CFILE> CFileOpen(const class SCPCFileInfo FileInfo, SCPCFileModeFlags Mode);
	
private:

	//may want to try to put this in a pointer or something so we don't need the full definition in the header
	//std::array<CFILE, MAX_CFILE_BLOCKS> Cfile_block_list{};
	//std::array<std::unique_ptr<class SCPCFileInfo>, 512 * 128>;
};
