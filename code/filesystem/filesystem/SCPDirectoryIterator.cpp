#include "SCPDirectoryIterator.h"

SCPDirectoryIterator::SCPDirectoryIterator(SCPPath Directory, Options Opts /*= DefaultOptions*/) 
	:InternalIterator(Directory),
	CurrentOptions(Opts)
{
}

SCPDirectoryIterator::SCPDirectoryIterator(SCPPath Directory, std::vector<SCP_string> Extensions, Options Opts)
	:InternalIterator(Directory),
	Extensions(Extensions),
	CurrentOptions(Opts)
{
}

SCPDirectoryIterator::SCPDirectoryIterator(SCPPath Directory, std::regex FilterRegex, Options Opts /*= DefaultOptions*/)
	:InternalIterator(Directory),
	FilterRegex(FilterRegex),
	CurrentOptions(Opts)
{
}

SCPDirectoryIterator::SCPDirectoryIterator(const SCPDirectoryIterator& Iterator) 
	:InternalIterator(Iterator.InternalIterator),
	FilterRegex(Iterator.FilterRegex),
	Extensions(Extensions),
	CurrentOptions(CurrentOptions)
{}

SCPDirectoryIterator::SCPDirectoryIterator(const SCPDirectoryIterator&& Iterator) 
	: InternalIterator(std::move(Iterator.InternalIterator)), 
	FilterRegex(std::move(Iterator.FilterRegex)), 
	Extensions(std::move(Extensions)),
	CurrentOptions(std::move(CurrentOptions))
{}

SCPDirectoryIterator& SCPDirectoryIterator::operator++() 
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
	return *this;
}

SCPPath& SCPDirectoryIterator::operator*() 
{
	return SCPPath(*InternalIterator);
}

bool SCPDirectoryIterator::operator!=(const SCPDirectoryIterator& Other) 
{
	return InternalIterator != Other.InternalIterator;
}

bool SCPDirectoryIterator::operator==(const SCPDirectoryIterator& Other) 
{
	return InternalIterator == Other.InternalIterator;
}

SCPDirectoryIterator SCPDirectoryIterator::begin() 
{
	SCPDirectoryIterator BeginIterator = SCPDirectoryIterator(*this);
	BeginIterator.InternalIterator = ghc::filesystem::begin(InternalIterator);
	return BeginIterator;
}

SCPDirectoryIterator SCPDirectoryIterator::end() 
{
	SCPDirectoryIterator EndIterator = SCPDirectoryIterator(*this);
	EndIterator.InternalIterator     = ghc::filesystem::end(InternalIterator);
	return EndIterator;
}
