/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell
 * or otherwise commercially exploit the source or things you created based on the
 * source.
 *
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <csetjmp>

#include <cctype>
//#include "globalincs/version.h"
//#include "mission/missionparse.h"
//#include "parse/encrypt.h"
#include "parse/parselo.h"
//#include "parse/sexp.h"
//#include "ship/ship.h"
//#include "weapon/weapon.h"
//#include "mod_table/mod_table.h"
#include "NOX.h"
#include "FSAssert.h"
#include "SCPCompiler.h"
#include "SCPLimits.h"
#include "FSStdTypes.h"
#include "FSMathTypes.h"
#include "SCPApplication.h"
#include "SCPBuiltinLanguages.h"
#include "SCPModTable.h"
#include "SCPFlags.h"
#include "SCPObjectLookup.h"
#include "utils/encoding.h"
#include "utils/unicode.h"
#include "memory/memory.h"
#include "memory/utils.h"
//#include "cfile/cfile.h"
#include <utf8.h>

using namespace parse;



#define SHARP_S			(char)-33

// to know that a modular table is currently being parsed
bool	Parsing_modular_table = false;

char		Current_filename[MAX_PATH_LEN];
char		Current_filename_sub[MAX_PATH_LEN];	//Last attempted file to load, don't know if ex or not.
char		Error_str[ERROR_LENGTH];
int		Warning_count, Error_count;
int		fred_parse_flag = 0;
int		Token_found_flag;

char 	*Parse_text = nullptr;
char	*Parse_text_raw = nullptr;
char	*Mp = NULL, *Mp_save = NULL;
const char	*token_found;

SCP_vector<Bookmark> Bookmarks;	// Stack of all our previously paused parsing

// text allocation stuff
void allocate_parse_text(size_t size);
static size_t Parse_text_size = 0;


//	Return true if this character is white space, else false.
int is_white_space(char ch)
{
	return ((ch == ' ') || (ch == '\t') || (ch == EOLN));
}
int is_white_space(unicode::codepoint_t cp)
{
	return ((cp == UNICODE_CHAR(' ')) || (cp == UNICODE_CHAR('\t')) || (cp == (unicode::codepoint_t)EOLN));
}

// Returns true if this character is gray space, else false (gray space is white space except for EOLN).
int is_gray_space(char ch)
{
	return ((ch == ' ') || (ch == '\t'));
}

bool is_gray_space(unicode::codepoint_t cp) {
	return cp == UNICODE_CHAR(' ') || cp == UNICODE_CHAR('\t');
}

int is_parenthesis(char ch)
{
	return ((ch == '(') || (ch == ')'));
}

//	Advance global Mp (mission pointer) past all current white space.
//	Leaves Mp pointing at first non white space character.
void ignore_white_space()
{
	while ((*Mp != '\0') && is_white_space(*Mp))
		Mp++;
}

void ignore_gray_space()
{
	while ((*Mp != '\0') && is_gray_space(*Mp))
		Mp++;
}

//	Truncate *str, eliminating all trailing white space.
//	Eg: "abc   "   becomes "abc"
//		 "abc abc " becomes "abc abc"
//		 "abc \t"   becomes "abc"
void drop_trailing_white_space(char *str)
{
	auto len = strlen(str);
	if (len == 0)
	{
		// Nothing to do here
		return;
	}
	auto i = len - 1;
	while (i != INVALID_SIZE && is_white_space(str[i]))
	{
		--i;
	}
	str[i + 1] = '\0';
}

//	Ditto for SCP_string
void drop_trailing_white_space(SCP_string &str)
{
	if (str.empty())
	{
		// Nothing to do here
		return;
	}
	auto i = str.size() - 1;
	while (i != INVALID_SIZE && is_white_space(str[i]))
	{
		--i;
	}
	str.resize(i + 1);
}

//	Eliminate any leading whitespace in str
void drop_leading_white_space(char *str)
{
	auto len = strlen(str);
	size_t first = 0;

	// find first non-whitespace
	while ((first < len) && is_white_space(str[first]))
		first++;

	// quick out
	if (first == 0)
		return;

	memmove(str, str+first, len-first);
	str[len-first] = 0;
}

//	Ditto for SCP_string
void drop_leading_white_space(SCP_string &str)
{
	auto len = str.length();
	size_t first = 0;

	// find first non-whitespace
	while ((first < len) && is_white_space(str[first]))
		first++;

	// quick out
	if (first == 0)
		return;

	// Assign the found substring to the string
	str = str.substr(first, len - first);
}

// eliminates all leading and trailing white space from a string.  Returns pointer passed in.
char *drop_white_space(char *str)
{
	drop_trailing_white_space(str);
	drop_leading_white_space(str);

	return str;
}

// ditto for SCP_string
void drop_white_space(SCP_string &str)
{
	drop_trailing_white_space(str);
	drop_leading_white_space(str);
}



//	Display a diagnostic message if Verbose is set.
//	(Verbose is set if -v command line switch is present.)
void diag_printf(const char *format, ...)
{
#ifndef NDEBUG
	va_list args;

	va_start(args, format);
	nprintf(("Parse", "%s", args));
	va_end(args);

#endif
}

//	Grab and return (a pointer to) a bunch of tokens, terminating at
// ERROR_LENGTH chars, or end of line.
char *next_tokens()
{
	int	count = 0;
	char	*pstr = Mp;
	char	ch;

	while (((ch = *pstr++) != EOLN) && (ch != '\0') && (count < ERROR_LENGTH-1))
		Error_str[count++] = ch;

	Error_str[count] = 0;
	return Error_str;
}

//	Return the line number given by the current mission pointer, ie Mp.
//	A very slow function (scans all processed text), but who cares how long
//	an error reporting function takes?
int get_line_num()
{
	int	count = 1;
	int	incomment = 0;
	int	multiline = 0;
	char	*stoploc;
	char	*p;

	p = Parse_text;
	stoploc = Mp;

	while (p < stoploc)
	{
		if (*p == '\0')
			GOutputDevice->Warning(LOCATION, "Unexpected end-of-file while looking for error line!");

		if ( !incomment && (*p == COMMENT_CHAR) )
			incomment = 1;

		if ( !incomment && (*p == '/') && (*(p+1) == '*') ) {
			multiline = 1;
			incomment = 1;
		}

		if ( incomment )
			stoploc++;

		if ( multiline && (*(p-1) == '*') && (*p == '/') ) {
			multiline = 0;
			incomment = 0;
		}

		if (*p++ == EOLN) {
			if ( !multiline && incomment )
				incomment = 0;
			count++;
		}
	}

	return count;
}

//	Call this function to display an error message.
//	error_level == 0 means this is just a warning.
//	!0 means it's an error message.
//	Prints line number and other useful information.
extern int Cmdline_noparseerrors;
void error_display(int error_level, const char *format, ...)
{
	char type[8];
	SCP_string error_text;
	va_list args;

	if (error_level == 0) {
		strcpy_s(type, "Warning");
		Warning_count++;
	} else {
		strcpy_s(type, "Error");
		Error_count++;
	}

	va_start(args, format);
	vsprintf(error_text, format, args);
	va_end(args);
	//TODO: @parselo fix Current_filename so diagnostics will work right
	nprintf((type, "%s(line %i): %s: %s\n", Current_filename, get_line_num(), type, error_text.c_str()));

	if(error_level == 0 || Cmdline_noparseerrors)
		GOutputDevice->Warning(LOCATION, "%s(line %i):\n%s: %s", Current_filename, get_line_num(), type, error_text.c_str());
	else
		GOutputDevice->Error(LOCATION, "%s(line %i):\n%s: %s", Current_filename, get_line_num(), type, error_text.c_str());
}


void copy_to_next_white(SCP_string& outstr, const char* instr)
{
	bool in_quotes = false;
	char ch;

	outstr = "";
	while (((ch = *instr++) > 0) && (ch != EOLN) && (ch != '\0')) {
		if (ch == '\"') {
			in_quotes = !in_quotes;
			continue;
		}

		if (!in_quotes && is_white_space(ch)) // not in quotes, white space terminates string
			break;

		if (!in_quotes &&
			is_parenthesis(ch)) // not in quotes, parentheses are important for parsing so we don't want to copy them
			break;

		outstr.append(1, ch);
	}
}

void copy_to_next_white(char* outstr, const char* instr, int max)
{
	int count      = 0;
	bool in_quotes = false;
	char ch;

	while (((ch = *instr++) > 0) && (ch != EOLN) && (ch != '\0') && (count < max)) {
		if (ch == '\"') {
			in_quotes = !in_quotes;
			continue;
		}

		if (!in_quotes && is_white_space(ch)) // not in quotes, white space terminates string
			break;

		if (!in_quotes &&
			is_parenthesis(ch)) // not in quotes, parentheses are important for parsing so we don't want to copy them
			break;

		*outstr++ = ch;
		count++;
	}

	if (count >= max)
		error_display(0,
					  "Token too long: [%s].  Length = " SIZE_T_ARG ".  Max is %i.\n",
					  next_tokens(),
					  strlen(next_tokens()),
					  max);

	*outstr = 0;
}

//	Copy characters from instr to outstr until eoln is found, or until max
//	characters have been copied (including terminator).
void copy_to_eoln(char *outstr, const char *more_terminators, const char *instr, int max)
{
	int	count = 0;
	char	ch;
	char	terminators[128];

	Assert((more_terminators == NULL) || (strlen(more_terminators) < 125));

	terminators[0] = EOLN;
	terminators[1] = 0;
	if (more_terminators != NULL)
		strcat_s(terminators, more_terminators);

	while (((ch = *instr++) != 0) && (strchr(terminators, ch) == NULL)  && (count < max)) {
		*outstr++ = ch;
		count++;
	}

	if (count >= max)
		error_display(0, "Token too long: [%s].  Length = " SIZE_T_ARG ".  Max is %i.\n", next_tokens(), strlen(next_tokens()), max);

	*outstr = 0;
}

//	Ditto for SCP_string.
void copy_to_eoln(SCP_string &outstr, const char *more_terminators, const char *instr)
{
	char	ch;
	char	terminators[128];

	Assert((more_terminators == NULL) || (strlen(more_terminators) < 125));

	terminators[0] = EOLN;
	terminators[1] = 0;
	if (more_terminators != NULL)
		strcat_s(terminators, more_terminators);

	outstr = "";
	while (((ch = *instr++) != 0) && (strchr(terminators, ch) == NULL)) {
		outstr.append(1, ch);
	}
}



//Returns a null-terminated character string allocated with vm_malloc() with the data
char* alloc_text_until(const char* instr, const char* endstr)
{
	Assert(instr && endstr);
	auto foundstr = stristr(instr, endstr);

	if(foundstr == NULL)
	{
        GOutputDevice->Error(LOCATION, "Missing [%s] in file", endstr);
        throw parse::ParseException("End string not found");
	}
	else
	{
		if ( (foundstr - instr) <= 0 ) {
			Int3();  // since this really shouldn't ever happen
			return NULL;
		}

		char* rstr = NULL;
		rstr = (char*) vm_malloc((foundstr - instr + 1)*sizeof(char));

		if(rstr != NULL) {
			strncpy(rstr, instr, foundstr-instr);
			rstr[foundstr-instr] = '\0';
		} else {
			GOutputDevice->Error(LOCATION, "Could not allocate enough memory in alloc_text_until");
		}

		return rstr;
	}
}

//	Copy text until a certain string is matched.
//	For example, this is used to copy mission notes, scanning until $END NOTES:
// is found.
void copy_text_until(char *outstr, const char *instr, const char *endstr, int max_chars)
{
	Assert(outstr && instr && endstr);

	auto foundstr = stristr(instr, endstr);

	if (foundstr == NULL) {
        nprintf(("Error", "Error.  Looking for [%s], but never found it.\n", endstr));
        throw parse::ParseException("End string not found");
	}

	if (foundstr - instr + strlen(endstr) < (uint) max_chars) {
		strncpy(outstr, instr, foundstr - instr);
		outstr[foundstr - instr] = 0;

	} else {
		nprintf(("Error", "Error.  Too much text (" SIZE_T_ARG " chars, %i allowed) before %s\n",
			foundstr - instr + strlen(endstr), max_chars, endstr));

        throw parse::ParseException("Too much text found");
	}

	diag_printf("Here's the partial wad of text:\n%.30s\n", outstr);
}

//	Ditto for SCP_string.
void copy_text_until(SCP_string &outstr, const char *instr, const char *endstr)
{
	Assert(instr && endstr);

	auto foundstr = stristr(instr, endstr);

	if (foundstr == NULL) {
        nprintf(("Error", "Error.  Looking for [%s], but never found it.\n", endstr));
        throw parse::ParseException("End string not found");
	}

	outstr.assign(instr, foundstr - instr);

	diag_printf("Here's the partial wad of text:\n%.30s\n", outstr.c_str());
}


//WMC
//Used for allocating large blocks, eg of Python code
//Returns a null-terminated string allocated with vm_malloc(),
//or NULL on failure
//Does depth checks for the start and end strings
//extra_chars indicates extra malloc space that should be allocated.
char* alloc_block(const char* startstr, const char* endstr, int extra_chars)
{
	Assert(startstr != NULL && endstr != NULL);
	Assert(stricmp(startstr, endstr));

	char* rval = NULL;
	auto elen = strlen(endstr);
	auto slen = strlen(startstr);
	size_t flen = 0;

	//Skip the opening thing and any extra stuff
	required_string(startstr);
	ignore_white_space();

	//Allocate it
	char* pos = Mp;

	//Depth checking
	int level = 1;
	while(*pos != '\0')
	{
		if(!strnicmp(pos, startstr, slen))
		{
			level++;
		}
		else if(!strnicmp(pos, endstr, elen))
		{
			level--;
		}

		if(level<=0)
		{
			break;
		}

		pos++;
	}

	//Check that we left the file
	if(level > 0)
	{
        GOutputDevice->Error(LOCATION, "Unclosed pair of \"%s\" and \"%s\" on line %d in file", startstr, endstr, get_line_num());
        throw parse::ParseException("End string not found");
	}
	else
	{
		//Set final length for faster calcs
		flen = pos-Mp;

		//Allocate the memory
		//WMC - Don't forget the null character that's added later on.
		rval = (char*) vm_malloc((flen + extra_chars + 1)*sizeof(char));

		//Copy the text (if memory was allocated)
		if(rval != NULL) {
			strncpy(rval, Mp, flen);
			rval[flen] = '\0';
		} else {
			return NULL;
		}
	}

	//Skip the copied stuff
	Mp += flen;
	required_string(endstr);
	return rval;
}

// After reading a multitext string, you can call this function to convert any newlines into
// spaces, so it's a one paragraph string (i.e. as in MS-Word).
void compact_multitext_string(char *str)
{
	auto p_dest = str;
	auto p_src = str;

	while (*p_src)
	{
		char ch = *p_src;
		p_src++;

		// skip CR
		// convert LF to space
		// copy characters backwards if any CRs previously encountered
		if (ch != '\r')
		{
			if (ch == '\n')
				*p_dest = ' ';
			else if (p_dest != p_src)
				*p_dest = *p_src;

			p_dest++;
		}
	}

	if (p_dest != p_src)
		*p_dest = 0;
}

// ditto for SCP_string
void compact_multitext_string(SCP_string &str)
{
	auto p_dest = str.begin();
	auto p_src = str.begin();

	while (p_src != str.end())
	{
		char ch = *p_src;
		p_src++;

		// skip CR
		// convert LF to space
		// copy characters backwards if any CRs previously encountered
		if (ch != '\r')
		{
			if (ch == '\n')
				*p_dest = ' ';
			else if (p_dest != p_src)
				*p_dest = *p_src;

			p_dest++;
		}
	}

	if (p_dest != p_src)
		str.erase(p_dest);
}

// Converts a character from Windows-1252 to CP437.
int maybe_convert_foreign_character(int ch)
{
	// time to do some special foreign character conversion
	switch (ch) {
		case -57:
			ch = 128;
			break;

		case -4:
			ch = 129;
			break;

		case -23:
			ch = 130;
			break;

		case -30:
			ch = 131;
			break;

		case -28:
			ch = 132;
			break;

		case -32:
			ch = 133;
			break;

		case -27:
			ch = 134;
			break;

		case -25:
			ch = 135;
			break;

		case -22:
			ch = 136;
			break;

		case -21:
			ch = 137;
			break;

		case -24:
			ch = 138;
			break;

		case -17:
			ch = 139;
			break;

		case -18:
			ch = 140;
			break;

		case -20:
			ch = 141;
			break;

		case -60:
			ch = 142;
			break;

		case -59:
			ch = 143;
			break;

		case -55:
			ch = 144;
			break;

		case -26:
			ch = 145;
			break;

		case -58:
			ch = 146;
			break;

		case -12:
			ch = 147;
			break;

		case -10:
			ch = 148;
			break;

		case -14:
			ch = 149;
			break;

		case -5:
			ch = 150;
			break;

		case -7:
			ch = 151;
			break;

		case -1:
			ch = 152;
			break;

		case -42:
			ch = 153;
			break;

		case -36:
			ch = 154;
			break;

		case -94:
			ch = 155;
			break;

		case -93:
			ch = 156;
			break;

		case -91:
			ch = 157;
			break;

		case -125:
			ch = 159;
			break;

		case -31:
			ch = 160;
			break;

		case -19:
			ch = 161;
			break;

		case -13:
			ch = 162;
			break;

		case -6:
			ch = 163;
			break;

		case -15:
			ch = 164;
			break;

		case -47:
			ch = 165;
			break;

		case -86:
			ch = 166;
			break;

		case -70:
			ch = 167;
			break;

		case -65:
			ch = 168;
			break;

		case -84:
			ch = 170;
			break;

		case -67:
			ch = 171;
			break;

		case -68:
			ch = 172;
			break;

		case -95:
			ch = 173;
			break;

		case -85:
			ch = 174;
			break;

		case -69:
			ch = 175;
			break;

		case -33:
			ch = 225;
			break;

		case -75:
			ch = 230;
			break;

		case -79:
			ch = 241;
			break;

		case -9:
			ch = 246;
			break;

		case -80:
			ch = 248;
			break;

		case -73:
			ch = 250;
			break;

		case -78:
			ch = 253;
			break;

		case -96:
			ch = 255;
			break;
	}

	return ch;
}

// Goober5000
// Yarn - The capacity of out must be at least the value returned by
// get_converted_string_length(in) (plus one if add_null is true).
// Returns the number of characters written to out.
size_t maybe_convert_foreign_characters(const char *in, char *out, bool add_null)
{
	if (Fred_running) {
		size_t len = strlen(in);

		if (add_null) {
			strcpy(out, in);
			return len + 1;
		} else {
			strncpy(out, in, len);
			return len;
		}
	} else {
		auto inp = in;
		auto outp = out;

		while (*inp != '\0') {
			if (*inp == SHARP_S) {
				*outp++ = 's';
				*outp++ = 's';
			} else if (Lcl_pl) {
				*outp++ = *inp;
			} else {
				*outp++ = (char) maybe_convert_foreign_character(*inp);
			}
			inp++;
		}

		if (add_null) {
			*outp++ = '\0';
		}

		return outp - out;
	}
}

// Goober5000
void maybe_convert_foreign_characters(SCP_string &text)
{
	if (!Fred_running) {
		for (SCP_string::iterator ii = text.begin(); ii != text.end(); ++ii) {
			text.reserve(get_converted_string_length(text));

			if (*ii == SHARP_S) {
				text.replace(ii, ii + 1, "ss");
				++ii;
			} else if (!Lcl_pl) {
				*ii = (char) maybe_convert_foreign_character(*ii);
			}
		}
	}
}

// Yarn - Returns what the length of the text will be after it's processed by
// maybe_convert_foreign_characters, not including the null terminator.
size_t get_converted_string_length(const char *text)
{
	if (Fred_running) {
		return strlen(text);
	} else {
		size_t count = 0;
		auto s = strchr(text, SHARP_S);
		while (s != nullptr) {
			count++;
			s = strchr(s + 1, SHARP_S);
		}
		return strlen(text) + count;
	}
}

// Yarn - Returns what the length of the text will be after it's processed by
// maybe_convert_foreign_characters.
size_t get_converted_string_length(const SCP_string &text)
{
	if (Fred_running) {
		return text.size();
	} else {
		size_t count = 0;
		for (auto ii = text.begin(); ii != text.end(); ++ii) {
			if (*ii == SHARP_S) {
				count++;
			}
		}
		return text.size() + count;
	}
}

// Goober5000
bool get_number_before_separator(int &number, int &number_chars, const char *text, char separator)
{
	char buf[8];
	const char *ch = text;
	int len = 0;

	while (true)
	{
		// didn't find separator
		if (*ch == '\0' || len == 8)
			return false;

		// found separator
		if (*ch == separator)
			break;

		// found nondigit
		if (!isdigit(*ch))
			return false;

		// copying in progress
		buf[len] = *ch;
		len++;
		ch++;
	}

	// got an integer
	buf[len] = '\0';
	number = atoi(buf);
	number_chars = len;
	return true;
}

// Goober5000
bool get_number_before_separator(int &number, int &number_chars, const SCP_string &text, SCP_string::iterator text_pos, char separator)
{
	char buf[8];
	SCP_string::iterator ch = text_pos;
	int len = 0;

	while (true)
	{
		// didn't find separator
		if (ch == text.end() || len == 8)
			return false;

		// found separator
		if (*ch == separator)
			break;

		// found nondigit
		if (!isdigit(*ch))
			return false;

		// copying in progress
		buf[len] = *ch;
		len++;
		++ch;
	}

	// got an integer
	buf[len] = '\0';
	number = atoi(buf);
	number_chars = len;
	return true;
}

bool matches_version_specific_tag(const char *line_start, bool &compatible_version, int &tag_len)
{
	// special version-specific comment
	// formatted like e.g. ;;FSO 3.7.0;;
	// Should now support anything from ;;FSO 3;; to ;;FSO 3.7.3.20151106;; -MageKing17
	if (strnicmp(line_start, ";;FSO ", 6) != 0)
		return false;

	int major, minor, build, revis;
	int s_num = scan_fso_version_string(line_start, &major, &minor, &build, &revis);

	if (s_num == 0)
		return false;

	// hack for releases
	if (s_num == 4 && FS_VERSION_REVIS < 1000) {
		s_num = 3;
	}

	const char *ch = line_start + 6;
	while ((*ch) != ';') {
		Assertion((*ch) != '\0', "String that was already guaranteed to end with semicolons did not end with semicolons; it's possible we have fallen into an alternate universe. Failing string: [%s]\n", line_start);
		ch++;
	}
	ch++;
	Assertion((*ch) == ';', "String that was guaranteed to have double semicolons did not; it's possible we have fallen into an alternate universe. Failing string: [%s]\n", line_start);
	ch++;

	tag_len = (int)(ch - line_start);
	compatible_version = true;

	// check whether major, minor, and build line up with this version
	if (major > FS_VERSION_MAJOR)
	{
		compatible_version = false;
	}
	else if (major == FS_VERSION_MAJOR && s_num > 1)
	{
		if (minor > FS_VERSION_MINOR)
		{
			compatible_version = false;
		}
		else if (minor == FS_VERSION_MINOR && s_num > 2)
		{
			if (build > FS_VERSION_BUILD)
			{
				compatible_version = false;
			}
			else if (build == FS_VERSION_BUILD && s_num > 3)
			{
				if (revis > FS_VERSION_REVIS)
				{
					compatible_version = false;
				}
			}
		}
	}

	// true for tag match
	return true;
}

// Strip comments from a line of input.
// Goober5000 - rewritten for the second time
void strip_comments(char *line, bool &in_quote, bool &in_multiline_comment_a, bool &in_multiline_comment_b)
{
	char *writep = line;
	char *readp = line;

	// copy all characters from read to write, unless they're commented
	while (*readp != '\r' && *readp != '\n' && *readp != '\0')
	{
		// only check for comments if not quoting
		if (!in_quote)
		{
			bool compatible_version;
			int tag_len;

			// see what sort of comment characters we recognize
			if (!strncmp(readp, "/*", 2))
			{
				// comment styles are mutually exclusive
				if (!in_multiline_comment_b)
					in_multiline_comment_a = true;
			}
			else if (!strncmp(readp, "!*", 2))
			{
				// comment styles are mutually exclusive
				if (!in_multiline_comment_a)
					in_multiline_comment_b = true;
			}
			else if (!strncmp(readp, "*/", 2))
			{
				if (in_multiline_comment_a)
				{
					in_multiline_comment_a = false;
					readp += 2;
					continue;
				}
			}
			else if (!strncmp(readp, "*!", 2))
			{
				if (in_multiline_comment_b)
				{
					in_multiline_comment_b = false;
					readp += 2;
					continue;
				}
			}
			// special version-specific comment
			// formatted like e.g. ;;FSO 3.7.0;;
			else if (matches_version_specific_tag(readp, compatible_version, tag_len))
			{
				// comment passes, so advance pass the tag and keep reading
				if (compatible_version)
				{
					readp += tag_len;
					continue;
				}
				// comment does not pass, so ignore the line
				else
				{
					break;
				}
			}
			// standard comment
			else if (*readp == ';')
			{
				break;
			}
		}

		// maybe toggle quoting
		if (*readp == '\"')
			in_quote = !in_quote;

		// if not inside a comment, copy the characters
		if (!in_multiline_comment_a && !in_multiline_comment_b)
		{
			if (writep != readp)
				*writep = *readp;

			writep++;
		}

		// read the next character
		readp++;
	}

	// if we moved any characters, or if we haven't reached the end of the string, then mark end-of-line and terminate string
	if (writep != readp || *readp != '\0')
	{
		writep[0] = EOLN;
		writep[1] = '\0';
	}
}

int parse_get_line(char *lineout, int max_line_len, const char *start, int max_size, const char *cur)
{
	char * t = lineout;
	int i, num_chars_read=0;
	char c;

	for ( i = 0; i < max_line_len-1; i++ ) {
		do {
			if ( (cur - start) >= max_size ) {
				*lineout = 0;
				if ( lineout > t ) {
					return num_chars_read;
				} else {
					return 0;
				}
			}
			c = *cur++;
			num_chars_read++;
		} while ( c == 13 );

		*lineout++ = c;
		if ( c=='\n' ) break;
	}

	*lineout++ = 0;
	return  num_chars_read;
}





// Goober5000
void read_file_text_from_default(const default_file& file, char *processed_text, char *raw_text)
{
	// we have no filename, so copy a substitute
	strcpy_s(Current_filename_sub, "internal default file");

	// if we are paused then processed_text and raw_text must not be NULL!!
	if ( !Bookmarks.empty() && ((processed_text == NULL) || (raw_text == NULL)) ) {
		GOutputDevice->Error(LOCATION, "ERROR: Neither \"processed_text\" nor \"raw_text\" may be NULL when parsing is paused!!\n");
	}

	// make sure to do this before anything else
	allocate_parse_text(file.size + 1);

	// if we have no raw buffer, set it as the default raw text area
	if (raw_text == NULL)
		raw_text = Parse_text_raw;

	auto text = reinterpret_cast<const char*>(file.data);

	// copy text in the array (but only if the raw text and the array are not the same)
	if (raw_text != file.data)
	{
		// Copy the file contents into the array and null-terminate it
		// We have to make sure to adjust the size if the size of a char is more than 1
		strncpy(raw_text, text, file.size / sizeof(char));
		raw_text[file.size / sizeof(char)] = '\0';
	}

	if (processed_text == NULL)
		processed_text = Parse_text;

	// process the text
	process_raw_file_text(processed_text, raw_text);
}

void stop_parse()
{
	Assert( Bookmarks.empty() );

	if (Parse_text != nullptr) {
		vm_free(Parse_text);
		Parse_text = nullptr;
	}

	if (Parse_text_raw != nullptr) {
		vm_free(Parse_text_raw);
		Parse_text_raw = nullptr;
	}

	Parse_text_size = 0;
}

void allocate_parse_text(size_t size)
{
	Assert( size > 0 );

	// Make sure that there is space for the terminating null character
	size += 1;

	if (size <= Parse_text_size) {
		// Make sure that a new parsing session does not use uninitialized data.
		memset( Parse_text, 0, sizeof(char) * Parse_text_size );
		memset( Parse_text_raw, 0, sizeof(char) * Parse_text_size);
		return;
	}

	static ubyte parse_atexit = 0;

	if (!parse_atexit) {
		atexit(stop_parse);
		parse_atexit = 1;
	}

	if (Parse_text != nullptr) {
		vm_free(Parse_text);
		Parse_text = nullptr;
	}

	if (Parse_text_raw != nullptr) {
		vm_free(Parse_text_raw);
		Parse_text_raw = nullptr;
	}

	Parse_text = (char *) vm_malloc(sizeof(char) * size, memory::quiet_alloc);
	Parse_text_raw = (char *) vm_malloc(sizeof(char) * size, memory::quiet_alloc);

	if ( (Parse_text == nullptr) || (Parse_text_raw == nullptr) ) {
		GOutputDevice->Error(LOCATION, "Unable to allocate enough memory for Parse_text!  Aborting...\n");
	}

	memset( Parse_text, 0, sizeof(char) * size );
	memset( Parse_text_raw, 0, sizeof(char) * size);

	Parse_text_size = size;
}



// Goober5000
void process_raw_file_text(char *processed_text, char *raw_text)
{
	char	*mp;
	char	*mp_raw;
	char outbuf[PARSE_BUF_SIZE];
	bool in_quote = false;
	bool in_multiline_comment_a = false;
	bool in_multiline_comment_b = false;
	int raw_text_len = (int)strlen(raw_text);

	if (processed_text == NULL)
		processed_text = Parse_text;

	if (raw_text == NULL)
		raw_text = Parse_text_raw;

	Assert( processed_text != NULL );
	Assert( raw_text != NULL );

	mp = processed_text;
	mp_raw = raw_text;

	// strip comments from raw text, reading into file_text
	int num_chars_read = 0;
	while ( (num_chars_read = parse_get_line(outbuf, PARSE_BUF_SIZE, raw_text, raw_text_len, mp_raw)) != 0 ) {
		mp_raw += num_chars_read;

		// stupid hacks to make retail data work with fixed parser, per Mantis #3072
		if (!strcmp(outbuf, "1402, \"Sie haben IPX-Protokoll als Protokoll ausgew\xE4hlt, aber dieses Protokoll ist auf Ihrer Maschine nicht installiert.\".\"\n")) {
			outbuf[121] = ' ';
			outbuf[122] = ' ';
		} else if (!strcmp(outbuf, "1117, \"\\r\\n\"Aucun web browser trouva. Del\xE0 isn't on emm\xE9nagea ou if \\r\\non est emm\xE9nagea, ca isn't set pour soient la default browser.\\r\\n\\r\\n\"\n")) {
			char *ch = &outbuf[11];
			do {
				*ch = *(ch+1);
				++ch;
			} while (*ch);
		} else if (!strcmp(outbuf, "1337, \"(fr)Loading\"\n")) {
			outbuf[3] = '6';
		} else if (!strcmp(outbuf, "3966, \"Es sieht so aus, als habe Staffel Kappa Zugriff auf die GTVA-Zugangscodes f\xFCr das System gehabt. Das ist ein ernstes Sicherheitsleck. Ihre IFF-Kennung erschien als \"verb\xFCndet\", so da\xDF sie sich dem Konvoi ungehindert n\xE4hern konnten. Zum Gl\xFC\x63k flogen Sie und  Alpha 2 Geleitschutz und lie\xDF\x65n den Schwindel auffliegen, bevor Kappa ihren Befehl ausf\xFChren konnte.\"\n")) {
			outbuf[171] = '\'';
			outbuf[181] = '\'';
		}

		strip_comments(outbuf, in_quote, in_multiline_comment_a, in_multiline_comment_b);

		if (Unicode_text_mode) {
			// In unicode mode we simply assume that the text is already properly encoded in UTF-8
			// Also, since we don't know how big mp actually is since we get the pointer from the outside we can't use one of
			// the "safe" strcpy variants here...
			strcpy(mp, outbuf);
			mp += strlen(outbuf);
		} else {
			mp += maybe_convert_foreign_characters(outbuf, mp, false);
		}
	}

	// Make sure the string is terminated properly
	*mp = *mp_raw = '\0';
/*
	while (cfgets(outbuf, PARSE_BUF_SIZE, mf) != NULL) {
		if (strlen(outbuf) >= PARSE_BUF_SIZE-1)
			error_display(0, "Input string too long.  Max is %i characters.\n%.256s\n", PARSE_BUF_SIZE, outbuf);

		//	If you hit this assert, it is probably telling you the obvious.  The file
		//	you are trying to read is truly too large.  Look at *filename to see the file name.
		Assert(mp_raw - file_text_raw + strlen(outbuf) < PARSE_TEXT_SIZE);
		strcpy_s(mp_raw, outbuf);
		mp_raw += strlen(outbuf);

		in_comment = strip_comments(outbuf, in_comment);
		strcpy_s(mp, outbuf);
		mp += strlen(outbuf);
	}

	*mp = *mp_raw = EOF_CHAR;
*/

}

void debug_show_mission_text()
{
	char	*mp = Parse_text;
	char	ch;

	while ((ch = *mp++) != '\0')
		printf("%c", ch);
}


int stuff_int_or_variable (int &i, bool positive_value = false);
int stuff_int_or_variable (int *ilp, int count, bool positive_value = false);


const char* get_lookup_type_name(int lookup_type)
{
	switch (lookup_type) {
		case SHIP_TYPE:
			return "Ships";
		case SHIP_INFO_TYPE:
			return "Ship Classes";
		case WEAPON_POOL_TYPE:
			return "Weapon Pool";
		case WEAPON_LIST_TYPE:
			return "Weapon Types";
		case RAW_INTEGER_TYPE:
			return "Untyped integer list";
	}

	return "Unknown lookup type, tell a coder!";
}


/**
 * @brief Given a string, find it in a string array.
 *
 * @param str1 is the string to be found.
 * @param strlist is the list of strings to search.
 * @param max is the number of entries in *strlist to scan.
 * @param description is only used for diagnostics in case it can't be found.
 * @param say_errors @c true if errors should be reported
 * @return
 */
int string_lookup(const char *str1, const char* const *strlist, size_t max, const char *description, bool say_errors) {
	for (size_t i=0; i<max; i++) {
		Assert(strlen(strlist[i]) != 0); //-V805

		if (!stricmp(str1, strlist[i]))
			return (int)i;
	}

	if (say_errors)
		error_display(0, "Unable to find [%s] in %s list.\n", str1, description);

	return -1;
}

// pause current parsing so that some else can be parsed without interfering
// with the currently parsing file
void pause_parse()
{
	Bookmark Mark;

	Mark.filename = Current_filename;
	Mark.Mp = Mp;
	Mark.Warning_count = Warning_count;
	Mark.Error_count = Error_count;

	Bookmarks.push_back(Mark);
}

// unpause parsing to continue with previously parsing file
void unpause_parse()
{
	Assert( !Bookmarks.empty() );
	if (Bookmarks.empty())
		return;

	Bookmark Mark = Bookmarks.back();

	Mp = Mark.Mp;
	Warning_count = Mark.Warning_count;
	Error_count = Mark.Error_count;

	strcpy_s(Current_filename, Mark.filename.c_str());

	Bookmarks.pop_back();
}

void reset_parse(char *text)
{
	if (text != NULL) {
		Mp = text;
	} else {
		Mp = Parse_text;
	}

	Warning_count = 0;
	Error_count = 0;

	strcpy_s(Current_filename, Current_filename_sub);
}

// Display number of warnings and errors at the end of a parse.
void display_parse_diagnostics()
{
	nprintf(("Parse", "\nParse complete.\n"));
	nprintf(("Parse", "%i errors.  %i warnings.\n", Error_count, Warning_count));
}


//TODO: @parselo move this to graphics api
// Splits a string into 2 lines if the string is wider than max_pixel_w pixels.  A null
// terminator is placed where required to make the first line <= max_pixel_w.  The remaining
// text is returned (leading whitespace removed).  If the line doesn't need to be split,
// NULL is returned.
char *split_str_once(char *src, int max_pixel_w)
{
	char *brk = NULL;
	int i, w, len, last_was_white = 0;

	Assert(src);
	Assert(max_pixel_w > 0);

	gr_get_string_size(&w, NULL, src);
	if ( (w <= max_pixel_w) && !strstr(src, "\n") ) {
		return NULL;  // string doesn't require a cut
	}

	len = (int)strlen(src);
	for (i=0; i<len; i++) {
		gr_get_string_size(&w, NULL, src, i);
		if ( w > max_pixel_w )
			break;

		if (src[i] == '\n') {  // reached natural end of line
			src[i] = 0;
			return src + i + 1;
		}

		if (is_white_space(src[i])) {
			if (!last_was_white)
				brk = src + i;

			last_was_white = 1;

		} else {
			last_was_white = 0;
		}
	}

	// if we are over max pixel width and weren't able to come up with a good non-word
	// split then just return the original src text and the calling function should
	// have to handle the result
	if ( (w > max_pixel_w) && ((i == 0) || !brk) ) {
		return src;
	}

	if (!brk) {
		brk = src + i;
	}

	*brk = 0;
	src = brk + 1;
	while (is_white_space(*src))
		src++;

	if (!*src)
		return NULL;  // end of the string anyway

	if (*src == '\n')
		src++;

	return src;
}

#define SPLIT_STR_BUFFER_SIZE	512

// --------------------------------------------------------------------------------------
// split_str()
//
// A general function that will split a string into several lines.  Lines are allowed up
// to max_pixel_w pixels.  Breaks are found in white space.
//
// Supports \n's in the strings!
//
// parameters:		src			=>		source string to be broken up
//						max_pixel_w	=>		max width of line in pixels
//						n_chars		=>		output array that will hold number of characters in each line
//						p_str			=>		output array of pointers to start of lines within src
//						max_lines	=>		limit of number of lines to break src up into
//						ignore_char	=>		OPTIONAL parameter (default val -1).  Ignore words starting with this character
//												This is useful when you want to ignore embedded control information that starts
//												with a specific character, like $ or #
//
//	returns:			number of lines src is broken into
//						-1 is returned when an error occurs
//
int split_str(const char *src, int max_pixel_w, int *n_chars, const char **p_str, int max_lines, unicode::codepoint_t ignore_char, bool strip_leading_whitespace)
{
	char buffer[SPLIT_STR_BUFFER_SIZE];
	const char *breakpoint = NULL;
	int sw, new_line = 1, line_num = 0, last_was_white = 0;
	int ignore_until_whitespace, buf_index;

	// check our assumptions..
	Assert(src != NULL);
	Assert(n_chars != NULL);
	Assert(p_str != NULL);
	Assert(max_lines > 0);
	Assert(max_pixel_w > 0);

	memset(buffer, 0, sizeof(buffer));
	buf_index = 0;
	ignore_until_whitespace = 0;

	// get rid of any leading whitespace
	while (strip_leading_whitespace && is_white_space(*src))
		src++;

	new_line = 1;
	p_str[0] = NULL;

	// iterate through chars in line, keeping track of most recent "white space" location that can be used
	// as a line splitting point if necessary
	unicode::codepoint_range range(src);
	auto end_iter = std::end(range);
	auto iter = std::begin(range);
	for (; iter != end_iter; ++iter) {
		auto cp = *iter;

		if (line_num >= max_lines)
			return line_num;  // time to bail out

		// starting a new line of text, init stuff for that
		if (new_line) {
			p_str[line_num] = NULL;
			if (strip_leading_whitespace && is_gray_space(cp))
				continue;

			p_str[line_num] = iter.pos();
			breakpoint = NULL;
			new_line = 0;
		}

		// maybe skip leading whitespace
		if (ignore_until_whitespace) {
			if ( is_white_space(cp) )
				ignore_until_whitespace = 0;

			continue;
		}

		// if we have a newline, split the line here
		if (cp == UNICODE_CHAR('\n')) {
			n_chars[line_num] = (int)(iter.pos() - p_str[line_num]);  // track length of line
			line_num++;
			if (line_num < max_lines) {
				p_str[line_num] = NULL;
			}
			new_line = 1;

			memset(buffer, 0, SPLIT_STR_BUFFER_SIZE);
			buf_index = 0;
			continue;
		}

		if (cp == ignore_char) {
			ignore_until_whitespace = 1;
			continue;
		}

		if (is_gray_space(cp)) {
			if (!last_was_white)  // track at first whitespace in a series of whitespace
				breakpoint = iter.pos();

			last_was_white = 1;

		} else {
			// indicate next time around that this wasn't a whitespace character
			last_was_white = 0;
		}

		auto encoded_width = unicode::encoded_size(cp);
		Assertion(buf_index + encoded_width < SPLIT_STR_BUFFER_SIZE,
				  "buffer overflow in split_str: screen width causes this text to be longer than %d characters!",
				  SPLIT_STR_BUFFER_SIZE - 1);

		// throw it in our buffer
		unicode::encode(cp, &buffer[buf_index]);
		buf_index += (int)encoded_width;
		buffer[buf_index] = 0;  // null terminate it

		gr_get_string_size(&sw, NULL, buffer);
		if (sw >= max_pixel_w) {
			const char *end;

			if (breakpoint) {
				end = breakpoint;
				iter = unicode::text_iterator(breakpoint, src, src + strlen(src));

			} else {
				end = iter.pos();  // force a split here since to whitespace
				--iter;  // reuse this character in next line
			}

			n_chars[line_num] = (int)(end - p_str[line_num]);  // track length of line
			Assert(n_chars[line_num]);
			line_num++;
			if (line_num < max_lines) {
				p_str[line_num] = NULL;
			}
			new_line = 1;

			memset(buffer, 0, sizeof(buffer));
			buf_index = 0;
			continue;
		}
	}	// end for

	if (!new_line && p_str[line_num]) {
		n_chars[line_num] = (int)(iter.pos() - p_str[line_num]);  // track length of line
		Assert(n_chars[line_num]);
		line_num++;
	}

	return line_num;
}

int split_str(const char *src, int max_pixel_w, SCP_vector<int> &n_chars, SCP_vector<const char*> &p_str, unicode::codepoint_t ignore_char, bool strip_leading_whitespace)
{
	char buffer[SPLIT_STR_BUFFER_SIZE];
	const char *breakpoint = NULL;
	int sw, new_line = 1, line_num = 0, last_was_white = 0;
	int ignore_until_whitespace = 0, buf_index = 0;

	// check our assumptions..
	Assert(src != NULL);
	Assert(max_pixel_w > 0);

	memset(buffer, 0, sizeof(buffer));

	// get rid of any leading whitespace
	while (strip_leading_whitespace && is_white_space(*src))
		src++;

	p_str.clear();

	// iterate through chars in line, keeping track of most recent "white space" location that can be used
	// as a line splitting point if necessary
	unicode::codepoint_range range(src);
	auto end_iter = std::end(range);
	auto iter = std::begin(range);
	for (; iter != end_iter; ++iter) {
		auto cp = *iter;

		// starting a new line of text, init stuff for that
		if (new_line) {
			if (strip_leading_whitespace && is_gray_space(cp))
				continue;

			p_str.push_back(iter.pos());
			breakpoint = NULL;
			new_line = 0;
		}

		// maybe skip leading whitespace
		if (ignore_until_whitespace) {
			if ( is_white_space(cp) ) {
				ignore_until_whitespace = 0;

				// don't eat the newline
				if (cp == EOLN)
					--iter;
			}

			continue;
		}

		// if we have a newline, split the line here
		if (cp == UNICODE_CHAR('\n')) {
			n_chars.push_back((int)(iter.pos() - p_str[line_num]));  // track length of line
			line_num++;
			new_line = 1;

			memset(buffer, 0, SPLIT_STR_BUFFER_SIZE);
			buf_index = 0;
			continue;
		}

		if (cp == ignore_char) {
			ignore_until_whitespace = 1;
			continue;
		}

		if (is_gray_space(cp)) {
			if (!last_was_white)  // track at first whitespace in a series of whitespace
				breakpoint = iter.pos();

			last_was_white = 1;

		} else {
			// indicate next time around that this wasn't a whitespace character
			last_was_white = 0;
		}

		auto encoded_width = unicode::encoded_size(cp);
		Assertion(buf_index + encoded_width < SPLIT_STR_BUFFER_SIZE,
				  "buffer overflow in split_str: screen width causes this text to be longer than %d characters!",
				  SPLIT_STR_BUFFER_SIZE - 1);

		// throw it in our buffer
		unicode::encode(cp, &buffer[buf_index]);
		buf_index += (int)encoded_width;
		buffer[buf_index] = 0;  // null terminate it

		gr_get_string_size(&sw, NULL, buffer);
		if (sw >= max_pixel_w) {
			const char *end;

			if (breakpoint) {
				end = breakpoint;
				iter = unicode::text_iterator(breakpoint, src, src + strlen(src));

			} else {
				end = iter.pos();  // force a split here since to whitespace
				--iter;  // reuse this character in next line
			}

			n_chars.push_back((int)(end - p_str[line_num]));  // track length of line
			Assert(n_chars[line_num]);
			line_num++;
			new_line = 1;

			memset(buffer, 0, sizeof(buffer));
			buf_index = 0;
			continue;
		}
	}	// end for

	if (!new_line && p_str[line_num]) {
		n_chars.push_back((int)(iter.pos() - p_str[line_num]));  // track length of line
		Assert(n_chars[line_num]);
		line_num++;
	}

	return line_num;
}

// Goober5000
// accounts for the dumb communications != communication, etc.
int subsystem_stricmp(const char *str1, const char *str2)
{
	Assert(str1 && str2);

	// ensure len-1 will be valid
	if (!*str1 || !*str2)
		return stricmp(str1, str2);

	// calc lengths
	auto len1 = (int)strlen(str1);
	auto len2 = (int)strlen(str2);

	// get rid of trailing s on s1?
	if (tolower(*(str1+len1-1) == 's'))
		len1--;

	// get rid of trailing s on s2?
	if (tolower(*(str2+len2-1) == 's'))
		len2--;

	// once we remove the trailing s on both names, they should be the same length
	if (len1 > len2)
		return 1;
	if (len1 < len2)
		return -1;

	// now do the comparison
	return strnicmp(str1, str2, len1);
}

// Goober5000
// current algorithm adapted from http://www.codeproject.com/string/stringsearch.asp
const char *stristr(const char *str, const char *substr)
{
	// check for null and insanity
	Assert(str);
	Assert(substr);
	if (str == NULL || substr == NULL || *substr == '\0')
		return NULL;

	// save both a lowercase and an uppercase version of the first character of substr
	char substr_ch_lower = (char)tolower(*substr);
	char substr_ch_upper = (char)toupper(*substr);

	// find the maximum distance to search
	const char *upper_bound = str + strlen(str) - strlen(substr);

	// loop through every character of str
	for (const char *start = str; start <= upper_bound; start++)
	{
		// check first character of substr
		if ((*start == substr_ch_upper) || (*start == substr_ch_lower))
		{
			// first character matched, so check the rest
			for (const char *str_ch = start+1, *substr_ch = substr+1; *substr_ch != '\0'; str_ch++, substr_ch++)
			{
				// character match?
				if (*str_ch == *substr_ch)
					continue;

				// converted character match?
				if (tolower(*str_ch) == tolower(*substr_ch))
					continue;

				// mismatch
				goto stristr_continue_outer_loop;
			}

			// finished inner loop with success!
			return start;
		}

stristr_continue_outer_loop:
		/* NO-OP */ ;
	}

	// no match
	return NULL;
}

// non-const version
char *stristr(char *str, const char *substr)
{
	// check for null and insanity
	Assert(str);
	Assert(substr);
	if (str == NULL || substr == NULL || *substr == '\0')
		return NULL;

	// save both a lowercase and an uppercase version of the first character of substr
	char substr_ch_lower = (char)tolower(*substr);
	char substr_ch_upper = (char)toupper(*substr);

	// find the maximum distance to search
	const char *upper_bound = str + strlen(str) - strlen(substr);

	// loop through every character of str
	for (char *start = str; start <= upper_bound; start++)
	{
		// check first character of substr
		if ((*start == substr_ch_upper) || (*start == substr_ch_lower))
		{
			// first character matched, so check the rest
			for (const char *str_ch = start+1, *substr_ch = substr+1; *substr_ch != '\0'; str_ch++, substr_ch++)
			{
				// character match?
				if (*str_ch == *substr_ch)
					continue;

				// converted character match?
				if (tolower(*str_ch) == tolower(*substr_ch))
					continue;

				// mismatch
				goto stristr_continue_outer_loop;
			}

			// finished inner loop with success!
			return start;
		}

stristr_continue_outer_loop:
		/* NO-OP */ ;
	}

	// no match
	return NULL;
}

// Goober5000
bool can_construe_as_integer(const char *text)
{
	// trivial case; evaluates to 0
	if (*text == '\0')
		return true;

	// number sign or digit for first char
	if ((*text != '+') && (*text != '-') && !isdigit(*text))
		return false;

	// check digits for rest
	for (const char *p = text + 1; *p != '\0'; p++)
	{
		if (!isdigit(*p))
			return false;
	}

	return true;
}

// Goober5000
// yoinked gratefully from dbugfile.cpp
void vsprintf(SCP_string &dest, const char *format, va_list ap)
{
	va_list copy;

#if defined(_MSC_VER) && _MSC_VER < 1800
	// Only Visual Studio >= 2013 supports va_copy
	// This isn't portable but should work for Visual Studio
	copy = ap;
#else
	va_copy(copy, ap);
#endif

	int needed_length = vsnprintf(nullptr, 0, format, copy);
	va_end(copy);

	if (needed_length < 0) {
		// Error
		return;
	}

	dest.resize(static_cast<size_t>(needed_length));
	vsnprintf(&dest[0], dest.size() + 1, format, ap);
}

void sprintf(SCP_string &dest, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf(dest, format, args);
	va_end(args);
}

// Goober5000
bool end_string_at_first_hash_symbol(char *src)
{
	char *p;
	Assert(src);

	p = get_pointer_to_first_hash_symbol(src);
	if (p)
	{
		while ((p != src) && (*(p-1) == ' '))
			p--;

		*p = '\0';
		return true;
	}

	return false;
}

// Goober5000
bool end_string_at_first_hash_symbol(SCP_string &src)
{
	int index = get_index_of_first_hash_symbol(src);
	if (index >= 0)
	{
		while (index > 0 && src[index-1] == ' ')
			index--;

		src.resize(index);
		return true;
	}

	return false;
}

// Goober5000
char *get_pointer_to_first_hash_symbol(char *src)
{
	Assert(src);
	return strchr(src, '#');
}

// Goober5000
const char *get_pointer_to_first_hash_symbol(const char *src)
{
	Assert(src);
	return strchr(src, '#');
}

// Goober5000
int get_index_of_first_hash_symbol(SCP_string &src)
{
	size_t pos = src.find('#');
	return (pos == SCP_string::npos) ? -1 : (int)pos;
}

// Goober5000
ptrdiff_t replace_one(char *str, const char *oldstr, const char *newstr, size_t max_len, ptrdiff_t range)
{
	Assert(str && oldstr && newstr);

	// search
	char *ch = stristr(str, oldstr);

	// found?
	if (ch)
	{
		// not found within bounds?
		if ((range > 0) && ((ch - str) > range))
		{
			return 0;
		}

		// determine if replacement will exceed max len
		if (strlen(str) + strlen(newstr) - strlen(oldstr) > max_len)
		{
			return -1;
		}

		// allocate temp string to hold extra stuff
		char *temp = (char *) vm_malloc(sizeof(char) * max_len);

		// ensure allocation was successful
		if (temp)
		{
			// save remainder of string
			strcpy_s(temp, sizeof(char)*max_len, ch + strlen(oldstr));

			// replace
			strcpy(ch, newstr);

			// append rest of string
			strcpy(ch + strlen(newstr), temp);
		}

		// free temp string
		vm_free(temp);
	}
	// not found
	else
	{
		return 0;
	}

	// return pos of replacement
	return (ch - str);
}

// Goober5000
ptrdiff_t replace_all(char *str, const char *oldstr, const char *newstr, size_t max_len, ptrdiff_t range)
{
	ptrdiff_t val, tally = 0;

	while ((val = replace_one(str, oldstr, newstr, max_len, range)) > 0)
	{
		tally++;

		// adjust range (if we have one), because the text length might have changed
		if (range) {
			range += strlen(newstr) - strlen(oldstr);
		}
	}

	return (val < 0) ? val : tally;
}

SCP_string& replace_one(SCP_string& context, const SCP_string& from, const SCP_string& to)
{
	size_t foundHere;
	if ((foundHere = context.find(from, 0)) != SCP_string::npos)
	{
		context.replace(foundHere, from.length(), to);
	}
	return context;
}

SCP_string& replace_one(SCP_string& context, const char* from, const char* to)
{
	size_t foundHere;
	if ((foundHere = context.find(from, 0)) != SCP_string::npos)
	{
		context.replace(foundHere, strlen(from), to);
	}
	return context;
}

// http://www.cppreference.com/wiki/string/replace
SCP_string& replace_all(SCP_string& context, const SCP_string& from, const SCP_string& to)
{
	size_t from_len = from.length();
	size_t to_len = to.length();

	size_t lookHere = 0;
	size_t foundHere;
	while ((foundHere = context.find(from, lookHere)) != SCP_string::npos)
	{
		context.replace(foundHere, from_len, to);
		lookHere = foundHere + to_len;
	}
	return context;
}

// http://www.cppreference.com/wiki/string/replace
SCP_string& replace_all(SCP_string& context, const char* from, const char* to)
{
	size_t from_len = strlen(from);
	size_t to_len = strlen(to);

	size_t lookHere = 0;
	size_t foundHere;
	while ((foundHere = context.find(from, lookHere)) != SCP_string::npos)
	{
		context.replace(foundHere, from_len, to);
		lookHere = foundHere + to_len;
	}
	return context;
}

// WMC
// Compares two strings, ignoring (last) extension
// Returns 0 if equal, nonzero if not
int strextcmp(const char *s1, const char *s2)
{
	// sanity check
	Assert( (s1 != NULL) && (s2 != NULL) );

	// find last '.' in both strings
	char *s1_end = (char *)strrchr(s1, '.');
	char *s2_end = (char *)strrchr(s2, '.');

	// get length
	size_t s1_len, s2_len;

	if (s1_end != NULL)
		s1_len = (s1_end - s1);
	else
		s1_len = strlen(s1);

	if (s2_end != NULL)
		s2_len = (s2_end - s2);
	else
		s2_len = strlen(s2);

	// if the lengths aren't the same then it's deffinitely not the same name
	if (s2_len != s1_len)
		return 1;

	return strnicmp(s1, s2, s1_len);
}

// Goober5000
bool drop_extension(char *str)
{
	char *p = strrchr(str, '.');
	if (p != NULL)
	{
		*p = 0;
		return true;
	}

	return false;
}

// Goober5000
bool drop_extension(SCP_string &str)
{
	size_t pos = str.rfind('.');
	if (pos != SCP_string::npos)
	{
		str.resize(pos);
		return true;
	}

	return false;
}

//WMC
void backspace(char* src)
{
	Assert(src!= NULL);		//this would be bad

	char *dest = src;
	src++;

	while(*src != '\0') {
		*dest++ = *src++;
	}

	*dest = '\0';
}

// Goober5000
void format_integer_with_commas(char *buf, int integer, bool use_comma_with_four_digits)
{
	int old_pos, new_pos, triad_count;
	char backward_buf[32];

	// print an initial string of just the digits
	sprintf(buf, "%d", integer);

	// no commas needed?
	if ((integer < 1000) || (integer < 10000 && !use_comma_with_four_digits))
		return;

	// scan the string backwards, writing commas after every third digit
	new_pos = 0;
	triad_count = 0;
	for (old_pos = (int)strlen(buf) - 1; old_pos >= 0; old_pos--)
	{
		backward_buf[new_pos] = buf[old_pos];
		new_pos++;
		triad_count++;

		if (triad_count == 3 && old_pos > 0)
		{
			backward_buf[new_pos] = ',';
			new_pos++;
			triad_count = 0;
		}
	}
	backward_buf[new_pos] = '\0';

	// now reverse the string
	new_pos = 0;
	for (old_pos = (int)strlen(backward_buf) - 1; old_pos >= 0; old_pos--)
	{
		buf[new_pos] = backward_buf[old_pos];
		new_pos++;
	}
	buf[new_pos] = '\0';
}

// Goober5000
// there's probably a better way to do this, but this way works and is clear and short
int scan_fso_version_string(const char *text, int *major, int *minor, int *build, int *revis)
{
	int val;

	val = sscanf(text, ";;FSO %i.%i.%i.%i;;", major, minor, build, revis);
	if (val == 4)
		return val;

	*revis = 0;
	val = sscanf(text, ";;FSO %i.%i.%i;;", major, minor, build);
	if (val == 3)
		return val;

	*build = *revis = 0;
	val = sscanf(text, ";;FSO %i.%i;;", major, minor);
	if (val == 2)
		return val;

	*minor = *build = *revis = 0;
	val = sscanf(text, ";;FSO %i;;", major);
	if (val == 1)
		return val;

	*major = *minor = *build = *revis = 0;
	return 0;
}

// Goober5000 - used for long Warnings, Errors, and FRED error messages with SEXPs
void truncate_message_lines(SCP_string &text, int num_allowed_lines)
{
	Assert(num_allowed_lines > 0);
	size_t find_from = 0;

	while (find_from < text.size())
	{
		if (num_allowed_lines <= 0)
		{
			text.resize(find_from);
			text.append("[...]");
			break;
		}

		size_t pos = text.find('\n', find_from);
		if (pos == SCP_string::npos)
			break;

		num_allowed_lines--;
		find_from = pos + 1;
	}
}

// Goober5000 - ugh, I can't see why they didn't just use stuff_*_list for these;
// the only differece is the lack of parentheses

// from aicode.cpp
// Stuff a list of floats at *plist.
void parse_float_list(float *plist, int size)
{
	int i;

	for (i=0; i<size; i++)
	{
		stuff_float(&plist[i]);
	}
}

// from aicode.cpp
// Stuff a list of ints at *plist.
void parse_int_list(int *ilist, int size)
{
	int i;

	for (i=0; i<size; i++)
	{
		stuff_int(&ilist[i]);
	}
}

// parse a modular table of type "name_check" and parse it using the specified function callback
int parse_modular_table(const char *name_check, void (*parse_callback)(const char *filename), int path_type, int sort_type)
{
	SCP_vector<SCP_string> tbl_file_names;
	int i, num_files = 0;

	if ( (name_check == NULL) || (parse_callback == NULL) || ((*name_check) != '*') ) {
		UNREACHABLE("parse_modular_table() called with invalid arguments; get a coder!\n");
		return 0;
	}

	num_files = cf_get_file_list(tbl_file_names, path_type, name_check, sort_type);

	Parsing_modular_table = true;

	const auto ext = strrchr(name_check, '.');

	for (i = 0; i < num_files; i++){
		if (ext != nullptr) {
			tbl_file_names[i] += ext;
		}
		mprintf(("TBM  =>  Starting parse of '%s' ...\n", tbl_file_names[i].c_str()));
		(*parse_callback)(tbl_file_names[i].c_str());
	}

	Parsing_modular_table = false;

	return num_files;
}
