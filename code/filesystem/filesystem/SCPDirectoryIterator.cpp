#include "SCPDirectoryIterator.h"

SCPDirectoryIterator::SCPDirectoryIterator(SCPPath Directory, Options Opts /*= DefaultOptions*/) 
	:InternalIterator(Directory),
	CurrentOptions(Opts)
{
	if (Opts.HasFlag(Flags::Recursive))
	{
		RecursiveInternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
	}
}

SCPDirectoryIterator::SCPDirectoryIterator(SCPPath Directory, std::set<SCP_string> Extensions, Options Opts)
	:InternalIterator(Directory),
	Extensions(Extensions),
	CurrentOptions(Opts)
{
	if (Opts.HasFlag(Flags::Recursive)) {
		RecursiveInternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
	}
}

SCPDirectoryIterator::SCPDirectoryIterator(SCPPath Directory, std::regex FilterRegex, Options Opts /*= DefaultOptions*/)
	:InternalIterator(Directory),
	FilterRegex(FilterRegex),
	CurrentOptions(Opts)
{
	if (Opts.HasFlag(Flags::Recursive))
	{
		RecursiveInternalIterator = ghc::filesystem::recursive_directory_iterator(Directory);
	}
}

SCPDirectoryIterator::SCPDirectoryIterator(const SCPDirectoryIterator& Iterator)
	:InternalIterator(Iterator.InternalIterator),
	RecursiveInternalIterator(Iterator.RecursiveInternalIterator),
	FilterRegex(Iterator.FilterRegex),
	Extensions(Iterator.Extensions),
	CurrentOptions(Iterator.CurrentOptions)
{}

SCPDirectoryIterator::SCPDirectoryIterator(const SCPDirectoryIterator&& Iterator) 
	: InternalIterator(std::move(Iterator.InternalIterator)), 
	RecursiveInternalIterator(Iterator.RecursiveInternalIterator), 
	FilterRegex(std::move(Iterator.FilterRegex)),
	Extensions(std::move(Extensions)),
	CurrentOptions(std::move(CurrentOptions))
{}

SCPDirectoryIterator& SCPDirectoryIterator::operator++() 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		//loop the iterator, checking each file if it matches the regex (if set) or extensions 
		while (RecursiveInternalIterator != ghc::filesystem::end(RecursiveInternalIterator))
		{
			RecursiveInternalIterator++;
			if (RecursiveInternalIterator != ghc::filesystem::end(RecursiveInternalIterator))
			{
				if (FilterRegex.has_value())
				{
					if (std::regex_search(RecursiveInternalIterator->path().string(), *FilterRegex))
					{
						return *this;
					}
				}
				else
				{
					for (auto Extension : Extensions)
					{
						if (RecursiveInternalIterator->path().extension() == Extension)
						{
							return *this;
						}
					}
				}
			}
		}

	}
	else
	{
		//loop the iterator, checking each file if it matches the regex (if set) or extensions 
		while (InternalIterator != ghc::filesystem::end(InternalIterator))
		{
			InternalIterator++;
			if (InternalIterator != ghc::filesystem::end(InternalIterator))
			{
				if (FilterRegex.has_value())
				{
					if (std::regex_search(InternalIterator->path().string(), *FilterRegex))
					{
						return *this;
					}
				}
				else
				{
					for (auto Extension : Extensions)
					{
						if (InternalIterator->path().extension() == Extension)
						{
							return *this;
						}
					}
				}
			}
		}

	}
	return *this;
}

SCPPath SCPDirectoryIterator::operator*() 
{
	return SCPPath(CurrentOptions.HasFlag(Flags::Recursive) ? *RecursiveInternalIterator : *InternalIterator);
}

bool SCPDirectoryIterator::operator!=(const SCPDirectoryIterator& Other) 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		return RecursiveInternalIterator != Other.RecursiveInternalIterator;
	}
	return InternalIterator != Other.InternalIterator;
}

bool SCPDirectoryIterator::operator==(const SCPDirectoryIterator& Other) 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		return RecursiveInternalIterator == Other.RecursiveInternalIterator;
	}
	return InternalIterator == Other.InternalIterator;
}

SCPDirectoryIterator SCPDirectoryIterator::begin() 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		SCPDirectoryIterator BeginIterator = SCPDirectoryIterator(*this);
		BeginIterator.RecursiveInternalIterator     = ghc::filesystem::begin(RecursiveInternalIterator);
		return BeginIterator;
	}
	SCPDirectoryIterator BeginIterator = SCPDirectoryIterator(*this);
	BeginIterator.InternalIterator = ghc::filesystem::begin(InternalIterator);
	return BeginIterator;
}

SCPDirectoryIterator SCPDirectoryIterator::end() 
{
	if (CurrentOptions.HasFlag(Flags::Recursive))
	{
		SCPDirectoryIterator EndIterator = SCPDirectoryIterator(*this);
		EndIterator.RecursiveInternalIterator     = ghc::filesystem::end(RecursiveInternalIterator);
		return EndIterator;
	}
	SCPDirectoryIterator EndIterator = SCPDirectoryIterator(*this);
	EndIterator.InternalIterator     = ghc::filesystem::end(InternalIterator);
	return EndIterator;
}
