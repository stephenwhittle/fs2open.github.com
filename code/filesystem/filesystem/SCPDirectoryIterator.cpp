#include "SCPDirectoryIterator.h"
#include "filesystem/SCPFile.h"

SCPFilesystemView::SCPFilesystemView(SCPPath Directory, Options Opts /*= DefaultOptions*/) 
	:CurrentOptions(Opts)
{
	if (!SCPFile(Directory).Exists())
	{
		InternalIterator = ghc::filesystem::directory_iterator();
	}
	else 
	{
		if (Opts.HasFlag(Flags::Recursive)) {
			InternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
		} else {
			InternalIterator = ghc::filesystem::directory_iterator(Directory);
		}
	}
}

SCPFilesystemView::SCPFilesystemView(SCPPath Directory, std::set<SCP_string> Extensions, Options Opts)
	:SCPFilesystemView(Directory, Opts)
{
	this->Extensions = Extensions;
}

SCPFilesystemView::SCPFilesystemView(SCPPath Directory, std::regex FilterRegex, Options Opts /*= DefaultOptions*/)
	:SCPFilesystemView(Directory, Opts)
{
	this->FilterRegex = FilterRegex;
}


bool SCPFilesystemView::operator++() 
{
	return MoveNext();
}

SCPPath SCPFilesystemView::operator*() 
{
	return GetCurrentDirectoryEntry().path();
}

bool SCPFilesystemView::operator!=(const SCPFilesystemView& Other) 
{
	return InternalIterator != Other.InternalIterator;
}

bool SCPFilesystemView::operator==(const SCPFilesystemView& Other) 
{
	return InternalIterator == Other.InternalIterator;
}

SCPDirectoryIterator SCPFilesystemView::begin() 
{
	return SCPDirectoryIterator(this);
}

SCPDirectoryIterator SCPFilesystemView::end() 
{
	return SCPDirectoryIterator();
}

ghc::filesystem::directory_entry SCPFilesystemView::GetCurrentDirectoryEntry()
{
	return mpark::visit([this](auto&& Iterator) { return *Iterator; }, InternalIterator);
}

bool SCPFilesystemView::MoveNext()
{
	return mpark::visit([this](auto&& Iterator) { return AdvanceToNext(Iterator); }, InternalIterator);
}
bool SCPFilesystemView::HasMoreResults()
{
	return mpark::visit([this](auto&& Iterator) { return Iterator != ghc::filesystem::end(Iterator); }, InternalIterator);
}
 SCPDirectoryIterator::SCPDirectoryIterator(SCPFilesystemView* View) : ViewToIterate(View)
{
	 if (View->HasMoreResults())
	 {
		 if (!View->CurrentFileMatchesFilter())
		 {
			 HasMoreResults = View->MoveNext();
		 }
		 else
		 {
			 HasMoreResults = true; //No need to advance as the first file matches the filter
		 }
	 }
	 else
	 {
		 HasMoreResults = false;
	 }
}

 SCPDirectoryIterator::SCPDirectoryIterator() { HasMoreResults = false; }

SCPPath SCPDirectoryIterator::operator*() 
{
	if (ViewToIterate != nullptr && HasMoreResults)
	{
		return *(*ViewToIterate);
	}
	return SCPPath();
}

void SCPDirectoryIterator::operator++()
{
	if (ViewToIterate != nullptr && HasMoreResults) {
		HasMoreResults = ViewToIterate->MoveNext();
	}
}

bool SCPDirectoryIterator::operator!=(const SCPDirectoryIterator& Other)
{
	if (ViewToIterate == nullptr || Other.ViewToIterate == nullptr) {
		return HasMoreResults != Other.HasMoreResults;
	} else {
		return HasMoreResults != Other.HasMoreResults || ViewToIterate != Other.ViewToIterate;
	}
}
