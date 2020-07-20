#pragma once
#include <ghc/fs_fwd.hpp>
#include "FSStdTypes.h"
#include <regex>
#include <tl/optional.hpp>
#include "filesystem/SCPPath.h"
#include "SCPFlags.h"
#include <set>

class SCPDirectoryIterator
{
public:
	enum class Flags
	{
		IncludeDirectories,
		IncludeFiles,
		Recursive
	};
	using Options = SCPFlags<Flags>;
	constexpr static Options DefaultOptions = { Flags::IncludeFiles };
    
	SCPDirectoryIterator(SCPPath Directory, SCPDirectoryIterator::Options Opts = DefaultOptions);
    SCPDirectoryIterator(SCPPath Directory, std::set<SCP_string> Extensions, SCPDirectoryIterator::Options Opts = DefaultOptions);
    SCPDirectoryIterator(SCPPath Directory, std::regex FilterRegex, SCPDirectoryIterator::Options Opts = DefaultOptions);
	SCPDirectoryIterator(const SCPDirectoryIterator& Iterator);
	SCPDirectoryIterator(const SCPDirectoryIterator&& Iterator);
	SCPDirectoryIterator& operator++();
	SCPPath operator*();
	bool operator!=(const SCPDirectoryIterator& Other);
	bool operator==(const SCPDirectoryIterator& Other);
	SCPDirectoryIterator begin();
	SCPDirectoryIterator end();

	//Foreach function?
private:
	ghc::filesystem::directory_iterator InternalIterator;
	ghc::filesystem::recursive_directory_iterator RecursiveInternalIterator;

	std::set<SCP_string> Extensions;
	tl::optional<std::regex> FilterRegex;
	Options CurrentOptions;

};


//operator++
//operator*
//operator!=

//begin() free
//end() free