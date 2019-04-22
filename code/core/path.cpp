
#include <core/path.h>

#include <core/platform.h>

#include <regex>
#include <string>

namespace core {
namespace path {
	const char* clean_filename(const char* filename)
	{
		auto separator = strrchr(filename, fs::path::preferred_separator);
		if (separator != nullptr) {
			filename = separator + 1;
		}

		return filename;
	}
} // namespace path
namespace fs {
	std::vector<fs::path> FindFilesInDirectory(fs::path Directory, const char* Pattern, bool Recursive)
	{
		std::vector<core::fs::path> MatchingPaths;
		if (core::fs::exists(Directory) && core::fs::is_directory(Directory)) {
			if (Recursive) {
				core::fs::recursive_directory_iterator DirectoryEntries{Directory};
				std::copy_if(begin(DirectoryEntries), end(DirectoryEntries), std::back_inserter(MatchingPaths),
				             [=](const core::fs::directory_entry& Entry) {
					             return Entry.is_regular_file() &&
					                    std::regex_match(Entry.path().filename().string(), std::regex(Pattern));
				             });
			} else {
				core::fs::directory_iterator DirectoryEntries{Directory};
				std::copy_if(begin(DirectoryEntries), end(DirectoryEntries), std::back_inserter(MatchingPaths),
				             [=](const core::fs::directory_entry& Entry) {
					             return Entry.is_regular_file() &&
					                    std::regex_match(Entry.path().filename().string(), std::regex(Pattern));
				             });
			}
		}
		return MatchingPaths;
	}
} // namespace fs
} // namespace core
