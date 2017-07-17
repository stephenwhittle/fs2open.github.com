
#pragma once

#include "cfile/cfile.h"

namespace util {

enum class Encoding {
	ASCII,
	UTF8,
	UTF16LE,
	UTF16BE,
	UTF32LE,
	UTF32BE
};

/**
 * @brief Guesses the encoding of the given content by looking at the first few bytes
 * @param assume_utf8 Set the @true true if no BOM should be treated as UTF-8 instead of ASCII
 * @return The guessed encoding of the content
 */
Encoding guess_encoding(const SCP_string& content, bool assume_utf8 = true);

/**
 * @brief Determines if the given text has a Byte Order Mark (BOM) that has to be skipped to get to the real text
 * @param content The text to check
 * @return @c true if there is a BOM, @c false otherwise
 */
bool has_bom(const SCP_string& content);

/**
 * @brief Guesses if the specifies buffer contains Latin1 encoding
 * @param aBuf The buffer to guess
 * @param aLen The length of the buffer
 * @return @c true if the algorithm determined with high probability that the text is Latin1 encoded
 *
 * @note The code of this function was copied from uchardet.
 */
bool guessLatin1Encoding(const char* aBuf, size_t aLen);

int check_encoding_and_skip_bom(CFILE* file, const char* filename, int* start_offset = nullptr);
}
