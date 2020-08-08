#pragma once
//#include <ghc/fs_fwd.hpp>
#include "FSStdTypes.h"
#include <regex>
#include <tl/optional.hpp>
#include "filesystem/SCPPath.h"
#include "SCPFlags.h"
#include <set>
#include "mpark/variant.hpp"
#include "fmt/format.h"
class ghc::filesystem::directory_iterator;
class ghc::filesystem::recursive_directory_iterator;

class SCPDirectoryIterator
{
	class SCPFilesystemView* ViewToIterate = nullptr;
	bool HasMoreResults = false;
public:
	SCPDirectoryIterator(SCPFilesystemView* View);
	SCPDirectoryIterator();
	SCPPath operator*();
	bool operator!= (const SCPDirectoryIterator& Other);
	void operator++();
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
	
	bool operator++();
	SCPPath operator*();
	bool operator!=(const SCPFilesystemView& Other);
	bool operator==(const SCPFilesystemView& Other);
	SCPDirectoryIterator begin();
	SCPDirectoryIterator end();
	bool MoveNext();
	//Foreach function?
private:
	friend class SCPDirectoryIterator;
	mpark::variant<ghc::filesystem::directory_iterator, ghc::filesystem::recursive_directory_iterator> InternalIterator;

	ghc::filesystem::directory_entry GetCurrentDirectoryEntry();

	bool CurrentFileMatchesFilter()
	{
		return mpark::visit([this](auto&& Iterator) {return CheckIterator(Iterator); }, InternalIterator);
	}
	
	template<typename IteratorType>
	bool CheckIterator(IteratorType& Iterator)
	{
		if (!CurrentOptions.HasFlag(Flags::IncludeDirectories) && Iterator->is_directory()) {
			return false;
		}

		if (!CurrentOptions.HasFlag(Flags::IncludeFiles) && Iterator->is_regular_file()) {
			return false;
		}

		if (FilterRegex.has_value()) {
			if (std::regex_search(Iterator->path().string(), *FilterRegex)) {
				return true;
			}
			else
			{
				return false;
			}
		} 
		else if (Extensions.size() > 0)
		{
			for (auto Extension : Extensions) {
				if (Iterator->path().extension() == Extension) {
					return true;
				}
			}
			return false;
		}

		return true;
	}

	template<typename IteratorType>
	bool AdvanceToNext(IteratorType& Iterator)
	{
		if (Iterator == ghc::filesystem::end(Iterator))
		{
			return false;
		}

		Iterator++;
		while (Iterator != ghc::filesystem::end(Iterator) && !CurrentFileMatchesFilter())
		{
			Iterator++;
		}
		return Iterator != ghc::filesystem::end(Iterator);
	}
	//Helper function for Directory iterator constructor
	bool HasMoreResults();
	std::set<SCP_string> Extensions;
	tl::optional<std::regex> FilterRegex;
	Options CurrentOptions;

};


//operator++
//operator*
//operator!=

//begin() free
//end() free