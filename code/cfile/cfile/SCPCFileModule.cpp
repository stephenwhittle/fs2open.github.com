#include "cfile/SCPCFileModule.h"
#include "SCPApplication.h"
#include "cfile/cfile.h"
#include "FSOutputDeviceBase.h"

#include "module/SCPModuleManager.h"
#include "filesystem/SCPFilesystemModule.h"
#include "FSIntegerTypes.h"
#include <utility>
#include "cfile/cfilesystem.h"
#include "cfile/SCPCFile.h"

int SCPCFileModule::GetNextEmptyBlockIndex() 
{
	int i;
	CFILE* cfile;

	for (i = 0; i < SCPCFileModule::MAX_CFILE_BLOCKS; i++) {
		cfile = &Cfile_block_list[i];
		if (cfile->type == CFILE_BLOCK_UNUSED) {
			cfile->data = nullptr;
			cfile->fp   = nullptr;
			cfile->type = CFILE_BLOCK_USED;
			return i;
		}
	}

	// If we've reached this point, a free Cfile_block could not be found
	nprintf(("Warning", "A free Cfile_block could not be found.\n"));

	// Dump a list of all opened files
	mprintf(("Out of cfile blocks! Currently opened files:\n"));
	DumpOpenedFileList();

	UNREACHABLE(
		"There are no more free cfile blocks. This means that there are too many files opened by FSO.\n"
		"This is probably caused by a programming or scripting error where a file does not get closed."); // out of free
																										  // cfile
																										  // blocks
	return -1;
}

//todo: @SCPCFileModule ascertain if we need to hold onto the pointer for a Cfile or not
//destructor can call cfclose if need be
tl::optional<CFILE&> SCPCFileModule::GetNextEmptyBlock() 
{
	for (CFILE& CurrentFile : Cfile_block_list)
	{
		if (CurrentFile.type == CFILE_BLOCK_UNUSED)
		{
			CurrentFile.data = nullptr;
			CurrentFile.fp = nullptr;
			CurrentFile.type = CFILE_BLOCK_UNUSED;
			return CurrentFile;
		}
	}
	nprintf(("Warning", "A free Cfile_block could not be found.\n"));

	// Dump a list of all opened files
	mprintf(("Out of cfile blocks! Currently opened files:\n"));
	DumpOpenedFileList();

	UNREACHABLE(
		"There are no more free cfile blocks. This means that there are too many files opened by FSO.\n"
		"This is probably caused by a programming or scripting error where a file does not get closed."); // out of free
																										  // cfile
																										  // blocks
	return {};
}

bool SCPCFileModule::StartupModule() 
{
	static bool InitializationGuard = false;
	if (InitializationGuard)
	{
		return false;
	}

	Cfile_block_list.fill({});

	cf_build_secondary_filelist(nullptr);
	
	InitializationGuard = true;
	return true;
}

void SCPCFileModule::ShutdownModule() 
{
	mprintf(("Still opened files:\n"));
	DumpOpenedFileList();

	//cf_free_secondary_filelist();

	//cfile_inited = 0;
}

void SCPCFileModule::DumpOpenedFileList() 
{
	for (auto& CFileBlock : Cfile_block_list)
	{
		if (CFileBlock.type != CFILE_BLOCK_UNUSED)
		{
			mprintf(("    %s:%d\n", CFileBlock.source_file, CFileBlock.line_num));
		}
	}
}


CFILE* SCPCFileModule::CFOpenFileFillBlock(const char* source, int line, FILE* fp, int type)
{

	tl::optional<CFILE&> File = GetNextEmptyBlock();
	if (!File) {
		fclose(fp);
		return nullptr;
	} else {
		File->data         = nullptr;
		File->mem_mapped   = false;
		File->fp           = fp;
		File->dir_type     = type;
		File->max_read_len = 0;

		File->source_file = source;
		File->line_num    = line;

		int pos = ftell(fp);
		if (pos == -1L)
			pos = 0;
		cf_init_lowlevel_read_code(&File.value(), 0, filelength(fileno(fp)), 0);

		return &File.value();
	}
}

CFILE*
SCPCFileModule::CFOpenInMemoryFileFillBlock(const char* source, int line, const void* data, size_t size, int dir_type)
{
	int cfile_block_index;

	cfile_block_index = GetNextEmptyBlockIndex();
	if (cfile_block_index == -1) {
		return NULL;
	} else {
		CFILE* cfp = &Cfile_block_list[cfile_block_index];

		cfp->max_read_len = 0;
		cfp->fp           = nullptr;
		cfp->mem_mapped   = false;
		cfp->dir_type     = dir_type;

		cfp->source_file = source;
		cfp->line_num    = line;

		cf_init_lowlevel_read_code(cfp, 0, size, 0);
		cfp->data = data;

		return cfp;
	}
}