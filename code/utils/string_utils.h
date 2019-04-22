#pragma once

#include <core/pstypes.h>

namespace util {

template <typename Out>
void split_string(const std::string& s, char delim, Out result)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) {
			*(result++) = item;
		}
	}
}

std::vector<std::string> split_string(const std::string& s, char delim);

} // namespace util
