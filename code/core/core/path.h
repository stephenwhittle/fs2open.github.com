#pragma once
#pragma warning(disable : 4244)
#include <ghc/filesystem.hpp>

#include <vector>
namespace core {
namespace path {
	const char* clean_filename(const char* filename);
}

namespace fs {
	using namespace ghc::filesystem;
	using ifstream = ghc::filesystem::ifstream;
	using ofstream = ghc::filesystem::ofstream;
	using fstream  = ghc::filesystem::fstream;
} // namespace fs

namespace fs {
	std::vector<fs::path> FindFilesInDirectory(fs::path Directory, const char* Pattern, bool Recursive);
	static const char preferred_separator_string[2] = {core::fs::path::preferred_separator, '\0'};

	
// check to see that a passed sting is valid, ie:
	//  - has >0 length
	//  - is not "none"
	//  - is not "<none>"
	bool VALID_FNAME(const char* x);
	/**
	 * @brief Checks if the specified string may be a valid file name
	 *
	 * @warning This only does a quick check against an empty string and a few known invalid names. It does not check if
	 * the file actually exists.
	 *
	 * @param x The file name to check
	 * @return @c true if the name is valid, @c false otherwise
	 */
	bool VALID_FNAME(const std::string& x);

} // namespace fs
} // namespace core

#pragma warning(default : 4244)