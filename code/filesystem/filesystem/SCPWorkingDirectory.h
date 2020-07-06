#pragma once
#include "filesystem/SCPPath.h"
#include <ghc/fs_fwd.hpp>
#include <system_error>
#include <stack>
class SCPWorkingDirectory
{
	std::stack<SCPPath> WorkingDirectoryStack;

public:
    SCPPath GetCurrent()
    {
		return ghc::filesystem::current_path();
    }
	
	bool Set(SCPPath NewDirectory)
	{
		std::error_code ec;
		ghc::filesystem::current_path(NewDirectory, ec);
		return ec ? false: true;
	}
	bool Push(SCPPath NewDirectory)
	{
		std::error_code ec;
		
		SCPPath OldPath = GetCurrent();

		ghc::filesystem::current_path(NewDirectory, ec);
		if (!ec)
		{
			WorkingDirectoryStack.push(OldPath);
		}
		return ec ? false : true;
	}
	SCPPath Pop()
	{
		if (WorkingDirectoryStack.size() > 0)
		{
			SCPPath PrevPath = WorkingDirectoryStack.top(); 
			WorkingDirectoryStack.pop();
			Set(PrevPath);
		}
		return GetCurrent();
	}
};