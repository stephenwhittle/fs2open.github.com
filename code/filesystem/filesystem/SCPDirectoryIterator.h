#pragma once
//#include <ghc/fs_fwd.hpp>
#include "FSStdTypes.h"
#include <regex>
#include <tl/optional.hpp>
#include "filesystem/SCPPath.h"
#include "SCPFlags.h"
#include <set>
#include "mpark/variant.hpp"

class ghc::filesystem::directory_iterator;
class ghc::filesystem::recursive_directory_iterator;

class SCPDirectoryIterator
{
	class SCPFilesystemView* ViewToIterate = nullptr;
	bool HasMoreResults = false;
public:
	SCPDirectoryIterator(SCPFilesystemView* View)
		:ViewToIterate(View)
	{
		
	}
	SCPDirectoryIterator()
	{
		HasMoreResults = false;
	}
	SCPPath operator*();
	bool operator!= (const SCPDirectoryIterator& Other)
	{
		if (ViewToIterate == nullptr || Other.ViewToIterate== nullptr)
		{
			return HasMoreResults != Other.HasMoreResults;
		}
		else
		{
			return HasMoreResults == Other.HasMoreResults || ViewToIterate == Other.ViewToIterate;
		}
	}
	void operator++()
	{
	}
};

class SCPFilesystemView
{
public:
	enum class Flags
	{
		IncludeDirectories = 1,
		IncludeFiles = 2,
		Recursive = 4
	};
	using Options = SCPFlags<Flags>;
	constexpr static Options DefaultOptions = { Flags::IncludeFiles };
    
	SCPFilesystemView(SCPPath Directory, SCPFilesystemView::Options Opts = DefaultOptions);
    SCPFilesystemView(SCPPath Directory, std::set<SCP_string> Extensions, SCPFilesystemView::Options Opts = DefaultOptions);
    SCPFilesystemView(SCPPath Directory, std::regex FilterRegex, SCPFilesystemView::Options Opts = DefaultOptions);
	SCPFilesystemView(const SCPFilesystemView& Iterator);
	SCPFilesystemView(const SCPFilesystemView&& Iterator);
	SCPFilesystemView& operator++();
	SCPPath operator*();
	bool operator!=(const SCPFilesystemView& Other);
	bool operator==(const SCPFilesystemView& Other);
	SCPFilesystemView begin();
	SCPFilesystemView end();
	SCPFilesystemView& MoveNext()
	{

	}
	//Foreach function?
private:

	mpark::variant<ghc::filesystem::directory_iterator, ghc::filesystem::recursive_directory_iterator> InternalIterator;

	
	template<typename IteratorType>
	bool PassesFilter(IteratorType Iterator)
	{
		if (!CurrentOptions.HasFlag(Flags::IncludeDirectories) && Iterator->is_directory()) {
			continue;
		}
		if (!CurrentOptions.HasFlag(Flags::IncludeFiles) && Iterator->is_regular_file()) {
			continue;
		}
		if (FilterRegex.has_value()) {
			if (std::regex_search(Iterator->path().string(), *FilterRegex)) {
				return *this;
			}
		} else {
			for (auto Extension : Extensions) {
				if (Iterator->path().extension() == Extension) {
					return *this;
				}
			}
		}
	}

	template<typename IteratorType>
	SCPFilesystemView& AdvanceToNext(IteratorType Iterator)
	{
		Iterator++;
		while (Iterator != ghc::filesystem::end(Iterator) && !PassesFilter(Iterator))
		{
			Iterator++;
		}
		return *this;
	}
	std::set<SCP_string> Extensions;
	tl::optional<std::regex> FilterRegex;
	Options CurrentOptions;

};


//operator++
//operator*
//operator!=

//begin() free
//end() free