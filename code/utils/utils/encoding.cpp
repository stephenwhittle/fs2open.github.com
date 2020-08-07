//
//

#include "utils/encoding.h"
#include "FSAssert.h"
#include "SCPModTable.h"
#include "SCPCompiler.h"
#include "utf8.h"
#include "SDL_stdinc.h"
namespace util {

Encoding guess_encoding(const SCP_string& content, bool assume_utf8) {
	if (content.size()>= 3 && !strncmp(content.c_str(), "\xEF\xBB\xBF", 3)) {        // UTF-8
		return Encoding::UTF8;
	}

	if (content.size()>= 4 && !strncmp(content.c_str(), "\x00\x00\xFE\xFF", 4)) {    // UTF-32 big-endian
		return Encoding::UTF32BE;
	}

	if (content.size()>= 4 && !strncmp(content.c_str(), "\xFF\xFE\x00\x00", 4)) {    // UTF-32 little-endian
		return Encoding::UTF32LE;
	}

	if (content.size()>= 2 && !strncmp(content.c_str(), "\xFE\xFF", 2)) {            // UTF-16 big-endian
		return Encoding::UTF16BE;
	}

	if (content.size()>= 2 && !strncmp(content.c_str(), "\xFF\xFE", 2)) {            // UTF-16 little-endian
		return Encoding::UTF16LE;
	}

	return assume_utf8 ? Encoding::UTF8 : Encoding::ASCII;
}
bool has_bom(const SCP_string& content) {
	if (content.size()>= 3 && !strncmp(content.c_str(), "\xEF\xBB\xBF", 3)) {        // UTF-8
		return true;
	}

	if (content.size()>= 4 && !strncmp(content.c_str(), "\x00\x00\xFE\xFF", 4)) {    // UTF-32 big-endian
		return true;
	}

	if (content.size()>= 4 && !strncmp(content.c_str(), "\xFF\xFE\x00\x00", 4)) {    // UTF-32 little-endian
		return true;
	}

	if (content.size()>= 2 && !strncmp(content.c_str(), "\xFE\xFF", 2)) {            // UTF-16 big-endian
		return true;
	}

	if (content.size()>= 2 && !strncmp(content.c_str(), "\xFF\xFE", 2)) {            // UTF-16 little-endian
		return true;
	}

	return false;
}


SCP_buffer ConvertFromLatinEncoding(SCP_buffer const& raw_text, SCP_string FileName)
{
	if (Unicode_text_mode) {
		// Validate the UTF-8 encoding
		auto invalid = utf8::find_invalid(raw_text.begin(), raw_text.end());
		if (invalid != raw_text.end()) {
			auto isLatin1 = util::guessLatin1Encoding(raw_text.Data(), (size_t)raw_text.Size);

			// We do the additional can_reallocate check here since we need control over raw_text to reencode the file
			if (isLatin1) {
				// Latin1 is the encoding of retail data and for legacy reasons we convert that to UTF-8.
				// We still output a warning though...
				GOutputDevice->Warning(
					LOCATION,
					"Found Latin-1 encoded file %s. This file will be automatically converted to UTF-8 but "
					"it may cause parsing issues with retail FS2 files since those contained invalid data.\n"
					"To silence this warning you must convert the files to UTF-8, e.g. by using a program like iconv.",
					FileName);

				// SDL2 has iconv functionality so we use that to convert from Latin1 to UTF-8

				// We need the raw_text as the output buffer so we first need to copy the current
				SCP_string input_str = raw_text.Data();
				SCP_buffer out_str = SCP_buffer(raw_text.Size * 2);
				do {
					auto in_str   = input_str.c_str();
					auto in_size  = input_str.size();
					auto out_size = out_str.Size;
					auto out_data = out_str.Data();

					auto iconv = SDL_iconv_open("UTF-8", "ISO-8859-1");
					auto err   = SDL_iconv(iconv, &in_str, &in_size, &out_data, &out_size);
					SDL_iconv_close(iconv);

					if (err == 0) {
						break;
					} else if (err == SDL_ICONV_E2BIG) {
						//Buffer wasn't big enough, increment by extra bytes
						out_size += 100;
						out_str = SCP_buffer(out_size);
					} else {
						GOutputDevice->Warning(LOCATION,
								"File reencoding failed (error code " SIZE_T_ARG ")!\n"
								"You will probably encounter encoding issues.",
								err);

						// Copy the original data back to the mission text pointer so that we don't loose any data here
						
						break;
					}
				} while (true);
				return out_str;
			} else {
				GOutputDevice->Warning(LOCATION,
									   "Found invalid UTF-8 encoding in file %s at position " PTRDIFF_T_ARG "!\n"
									   "This may cause parsing errors and should be fixed!",
									   FileName,
									   invalid - raw_text.Data());
				
			}
		}
	}
	return raw_text.Clone();
}


// The following code is adapted from the uchardet library, the original licence of the file has been kept

/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Universal charset detector code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *          Shy Shalom <shooshX@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include <cstdio>

#define UDF    0        // undefined
#define OTH    1        //other
#define ASC    2        // ascii capital letter
#define ASS    3        // ascii small letter
#define ACV    4        // accent capital vowel
#define ACO    5        // accent capital other
#define ASV    6        // accent small vowel
#define ASO    7        // accent small other
#define CLASS_NUM   8    // total classes

#define FREQ_CAT_NUM    4

static const unsigned char Latin1_CharToClass[] = { OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 00 - 07
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 08 - 0F
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 10 - 17
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 18 - 1F
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 20 - 27
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 28 - 2F
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 30 - 37
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 38 - 3F
													OTH, ASC, ASC, ASC, ASC, ASC, ASC, ASC,   // 40 - 47
													ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC,   // 48 - 4F
													ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC,   // 50 - 57
													ASC, ASC, ASC, OTH, OTH, OTH, OTH, OTH,   // 58 - 5F
													OTH, ASS, ASS, ASS, ASS, ASS, ASS, ASS,   // 60 - 67
													ASS, ASS, ASS, ASS, ASS, ASS, ASS, ASS,   // 68 - 6F
													ASS, ASS, ASS, ASS, ASS, ASS, ASS, ASS,   // 70 - 77
													ASS, ASS, ASS, OTH, OTH, OTH, OTH, OTH,   // 78 - 7F
													OTH, UDF, OTH, ASO, OTH, OTH, OTH, OTH,   // 80 - 87
													OTH, OTH, ACO, OTH, ACO, UDF, ACO, UDF,   // 88 - 8F
													UDF, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // 90 - 97
													OTH, OTH, ASO, OTH, ASO, UDF, ASO, ACO,   // 98 - 9F
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // A0 - A7
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // A8 - AF
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // B0 - B7
													OTH, OTH, OTH, OTH, OTH, OTH, OTH, OTH,   // B8 - BF
													ACV, ACV, ACV, ACV, ACV, ACV, ACO, ACO,   // C0 - C7
													ACV, ACV, ACV, ACV, ACV, ACV, ACV, ACV,   // C8 - CF
													ACO, ACO, ACV, ACV, ACV, ACV, ACV, OTH,   // D0 - D7
													ACV, ACV, ACV, ACV, ACV, ACO, ACO, ACO,   // D8 - DF
													ASV, ASV, ASV, ASV, ASV, ASV, ASO, ASO,   // E0 - E7
													ASV, ASV, ASV, ASV, ASV, ASV, ASV, ASV,   // E8 - EF
													ASO, ASO, ASV, ASV, ASV, ASV, ASV, OTH,   // F0 - F7
													ASV, ASV, ASV, ASV, ASV, ASO, ASO, ASO,   // F8 - FF
};


/* 0 : illegal
   1 : very unlikely
   2 : normal
   3 : very likely
*/
static const unsigned char Latin1ClassModel[] = {
/*      UDF OTH ASC ASS ACV ACO ASV ASO  */
/*UDF*/  0, 0, 0, 0, 0, 0, 0, 0,
/*OTH*/  0, 3, 3, 3, 3, 3, 3, 3,
/*ASC*/  0, 3, 3, 3, 3, 3, 3, 3,
/*ASS*/  0, 3, 3, 3, 1, 1, 3, 3,
/*ACV*/  0, 3, 3, 3, 1, 2, 1, 2,
/*ACO*/  0, 3, 3, 3, 3, 3, 3, 3,
/*ASV*/  0, 3, 1, 3, 1, 1, 1, 3,
/*ASO*/  0, 3, 1, 3, 1, 1, 3, 3, };

bool guessLatin1Encoding(const char* aBuf, size_t aLen) {
	char mLastCharClass = OTH;
	uint32_t mFreqCounter[FREQ_CAT_NUM];
	for (int i = 0; i < FREQ_CAT_NUM; i++) {
		mFreqCounter[i] = 0;
	}

	unsigned char charClass;
	unsigned char freq;
	for (size_t i = 0; i < aLen; i++) {
		charClass = Latin1_CharToClass[(unsigned char) aBuf[i]];
		freq = Latin1ClassModel[mLastCharClass * CLASS_NUM + charClass];
		if (freq == 0) {
			return false;
		}
		mFreqCounter[freq]++;
		mLastCharClass = charClass;
	}

	float confidence;
	uint32_t total = 0;
	for (int32_t i = 0; i < FREQ_CAT_NUM; i++) {
		total += mFreqCounter[i];
	}

	if (!total) {
		confidence = 0.0f;
	} else {
		confidence = mFreqCounter[3] * 1.0f / total;
		confidence -= mFreqCounter[1] * 20.0f / total;
	}

	if (confidence < 0.0f) {
		confidence = 0.0f;
	}

	return confidence > .5f;
}
}

//0x80-0x9f =  windows 1252
//otherwise if there are no single-byte cahracters between 80 and bf it is utf8
/*
std::string iso_8859_1_to_utf8(std::string &str)
{
	string strOut;
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		uint8_t ch = *it;
		if (ch < 0x80) {
			strOut.push_back(ch);
		}
		else {
			strOut.push_back(0xc0 | ch >> 6);
			strOut.push_back(0x80 | (ch & 0x3f));
		}
	}
	return strOut;
}
*/

/*

 In real ISO Latin-1, character codes in the range 127-159 are undefined

I've been working on a similar "guess the encoding" problem. The best solution involves knowing the encoding. Barring
that, you can make educated guesses to distinguish between UTF-8 and ISO-8859-1.

To answer the general question of how to detect if a string is properly encoded UTF-8, you can verify the following
things:

	No byte is 0x00, 0xC0, 0xC1, or in the range 0xF5-0xFF.
	Tail bytes (0x80-0xBF) are always preceded by a head byte 0xC2-0xF4 or another tail byte.
	Head bytes should correctly predict the number of tail bytes (eg, any byte in 0xC2-0xDF should be followed by
exactly one byte in the range 0x80-0xBF).

If a string passes all those tests, then it's interpretable as valid UTF-8. That doesn't guarantee that it is UTF-8, but
it's a good predictor.

Legal input in ISO-8859-1 will likely have no control characters (0x00-0x1F and 0x80-0x9F) other than line separators.
Looks like 0x7F isn't defined in ISO-8859-1 either.

(I'm basing this off of Wikipedia pages for UTF-8 and ISO-8859-1.)



ASCII

If your data contains no bytes above 0x7F, then it's ASCII. (Or a 7-bit ISO646 encoding, but those are very obsolete.)
UTF-8

If your data validates as UTF-8, then you can safely assume it is UTF-8. Due to UTF-8's strict validation rules, false
positives are extremely rare. ISO-8859-1 vs. windows-1252

The only difference between these two encodings is that ISO-8859-1 has the C1 control characters where windows-1252 has
the printable characters €‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ. I've seen plenty of files that use curly quotes or dashes, but
none that use C1 control characters. So don't even bother with them, or ISO-8859-1, just detect windows-1252 instead.


US-ASCII

No BOM, but you don't need one. ASCII can be easily identified by the lack of bytes in the 80-FF range.
UTF-8

BOM is EF BB BF. But you can't rely on this. Lots of UTF-8 files don't have a BOM, especially if they originated on
non-Windows systems.

But you can safely assume that if a file validates as UTF-8, it is UTF-8. False positives are rare.

Specifically, given that the data is not ASCII, the false positive rate for a 2-byte sequence is only 3.9% (1920/49152).
For a 7-byte sequence, it's less than 1%. For a 12-byte sequence, it's less than 0.1%. For a 24-byte sequence, it's less
than 1 in a million. UTF-16

BOM is FE FF (for BE) or FF FE (for LE). Note that the UTF-16LE BOM is found at the start of the UTF-32LE BOM, so check
UTF-32 first.

If you happen to have a file that consists mainly of ISO-8859-1 characters, having half of the file's bytes be 00 would
also be a strong indicator of UTF-16.

Otherwise, the only reliable way to recognize UTF-16 without a BOM is to look for surrogate pairs (D[8-B]xx D[C-F]xx),
but non-BMP characters are too rarely-used to make this approach practical. XML

If your file starts with the bytes 3C 3F 78 6D 6C (i.e., the ASCII characters "<?xml"), then look for an encoding=
declaration. If present, then use that encoding. If absent, then assume UTF-8, which is the default XML encoding.

If you need to support EBCDIC, also look for the equivalent sequence 4C 6F A7 94 93.

In general, if you have a file format that contains an encoding declaration, then look for that declaration rather than
trying to guess the encoding. None of the above

There are hundreds of other encodings, which require more effort to detect. I recommend trying Mozilla's charset
detector or a .NET port of it. A reasonable default

If you've ruled out the UTF encodings, and don't have an encoding declaration or statistical detection that points to a
different encoding, assume ISO-8859-1 or the closely related Windows-1252. (Note that the latest HTML standard requires
a “ISO-8859-1” declaration to be interpreted as Windows-1252.)



*/