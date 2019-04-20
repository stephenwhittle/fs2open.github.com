#pragma once
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
}
} // namespace core