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

