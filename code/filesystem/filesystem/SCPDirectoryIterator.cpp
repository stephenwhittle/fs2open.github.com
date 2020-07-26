#include "SCPDirectoryIterator.h"

SCPFilesystemView::SCPFilesystemView(SCPPath Directory, Options Opts /*= DefaultOptions*/) 
	:InternalIterator(Directory),
	CurrentOptions(Opts)
{
	if (Opts.HasFlag(Flags::Recursive))
	{
		RecursiveInternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
	}
}

SCPFilesystemView::SCPFilesystemView(SCPPath Directory, std::set<SCP_string> Extensions, Options Opts)
	:InternalIterator(Directory),
	Extensions(Extensions),
	CurrentOptions(Opts)
{
	if (Opts.HasFlag(Flags::Recursive)) {
		RecursiveInternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
		if (!PassesFilter(RecursiveInternalIterator))
		{
			AdvanceToNext(RecursiveInternalIterator);
		}
	}
}

SCPFilesystemView::SCPFilesystemView(SCPPath Directory, std::regex FilterRegex, Options Opts /*= DefaultOptions*/)
	:InternalIterator(Directory),
	FilterRegex(FilterRegex),
	CurrentOptions(Opts)
{
	if (Opts.HasFlag(Flags::Recursive))
	{
		RecursiveInternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
	}
}

SCPFilesystemView::SCPFilesystemView(const SCPFilesystemView& Iterator)
	:InternalIterator(Iterator.InternalIterator),
	RecursiveInternalIterator(Iterator.RecursiveInternalIterator),
	FilterRegex(Iterator.FilterRegex),
	Extensions(Iterator.Extensions),
	CurrentOptions(Iterator.CurrentOptions)
{}

SCPFilesystemView::SCPFilesystemView(const SCPFilesystemView&& Iterator) 
	: InternalIterator(std::move(Iterator.InternalIterator)), 
	RecursiveInternalIterator(Iterator.RecursiveInternalIterator), 
	FilterRegex(std::move(Iterator.FilterRegex)),
	Extensions(std::move(Extensions)),
	CurrentOptions(std::move(CurrentOptions))
{}

SCPFilesystemView& SCPFilesystemView::operator++() 
{
	//needs to respect the other flags
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		//loop the iterator, checking each file if it matches the regex (if set) or extensions 
		return AdvanceToNext(RecursiveInternalIterator);
	}
	else
	{
		return AdvanceToNext(InternalIterator);
	}
}

SCPPath SCPFilesystemView::operator*() 
{
	return SCPPath(CurrentOptions.HasFlag(Flags::Recursive) ? *RecursiveInternalIterator : *InternalIterator);
}

bool SCPFilesystemView::operator!=(const SCPFilesystemView& Other) 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		return RecursiveInternalIterator != Other.RecursiveInternalIterator;
	}
	return InternalIterator != Other.InternalIterator;
}

bool SCPFilesystemView::operator==(const SCPFilesystemView& Other) 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		return RecursiveInternalIterator == Other.RecursiveInternalIterator;
	}
	return InternalIterator == Other.InternalIterator;
}

SCPFilesystemView SCPFilesystemView::begin() 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		SCPFilesystemView BeginIterator = SCPFilesystemView(*this);
		BeginIterator.RecursiveInternalIterator     = ghc::filesystem::begin(RecursiveInternalIterator);
		return BeginIterator;
	}
	SCPFilesystemView BeginIterator = SCPFilesystemView(*this);
	BeginIterator.InternalIterator = ghc::filesystem::begin(InternalIterator);
	return BeginIterator;
}

SCPFilesystemView SCPFilesystemView::end() 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		SCPFilesystemView EndIterator = SCPFilesystemView(*this);
		EndIterator.RecursiveInternalIterator     = ghc::filesystem::end(RecursiveInternalIterator);
		return EndIterator;
	}
	SCPFilesystemView EndIterator = SCPFilesystemView(*this);
	EndIterator.InternalIterator     = ghc::filesystem::end(InternalIterator);
	return EndIterator;
}
