#include "SCPParser.h"
#include "SCPCompiler.h"
#include "SCPObjectLookup.h"
#include "FSAssert.h"
#include "parse/parselo.h"
#include "memory/utils.h"

#define ERROR_LENGTH 64
#define RS_MAX_TRIES 5

 SCPParser::SCPParser(char* InRawData) 
	 : RawData(InRawData), 
	 Mp(InRawData) 
 {
	 // TODO: @parselo move this to cfile as well, split comment stripping so it can be left here then called by other
	 // modules
	 // process it (strip comments)
	 ConvertFromLatinEncoding(raw_text, file_len, filename);
	 process_raw_file_text(processed_text, raw_text);
 }

void SCPParser::advance_to_next_white()
{
	bool in_quotes = false;

	while ((*Mp != EOLN) && (*Mp != '\0')) {
		if (*Mp == '\"')
			in_quotes = !in_quotes;

		if (!in_quotes && is_white_space(*Mp))
			break;

		if (!in_quotes && is_parenthesis(*Mp))
			break;

		Mp++;
	}
}

void SCPParser::advance_to_eoln(const char* more_terminators)
{
	char terminators[128];

	Assert((more_terminators == NULL) || (strlen(more_terminators) < 125));

	terminators[0] = EOLN;
	terminators[1] = 0;
	if (more_terminators != NULL)
		strcat_s(terminators, more_terminators);

	while (strchr(terminators, *Mp) == NULL)
		Mp++;
}


void SCPParser::ignore_white_space()
{
	while ((*Mp != '\0') && is_white_space(*Mp))
		Mp++;
}


int SCPParser::skip_to_string(const char* pstr, const char* end)
{
	ignore_white_space();
	auto len    = strlen(pstr);
	size_t len2 = 0;

	if (end)
		len2 = strlen(end);

	while ((*Mp != '\0') && strnicmp(pstr, Mp, len) != 0) {
		if (end && *Mp == '#')
			return 0;

		if (end && !strnicmp(end, Mp, len2))
			return -1;

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || *Mp == '\0')
		return 0;

	Mp += strlen(pstr);
	return 1;
}

void SCPParser::skip_token()
{
	ignore_white_space();

	while ((*Mp != '\0') && !is_white_space(*Mp))
		Mp++;
}

int SCPParser::skip_to_start_of_string(const char* pstr, const char* end)
{
	ignore_white_space();
	auto len = strlen(pstr);
	size_t endlen;
	if (end)
		endlen = strlen(end);
	else
		endlen = 0;

	while ((*Mp != '\0') && strnicmp(pstr, Mp, len) != 0) {
		if (end && *Mp == '#')
			return 0;

		if (end && !strnicmp(end, Mp, endlen))
			return 0;

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || *Mp == '\0')
		return 0;

	return 1;
}

int SCPParser::skip_to_start_of_string_either(const char* pstr1, const char* pstr2, const char* end)
{
	size_t len1, len2, endlen;

	ignore_white_space();
	len1 = strlen(pstr1);
	len2 = strlen(pstr2);
	if (end)
		endlen = strlen(end);
	else
		endlen = 0;

	while ((*Mp != '\0') && strnicmp(pstr1, Mp, len1) != 0 && strnicmp(pstr2, Mp, len2) != 0) {
		if (end && *Mp == '#')
			return 0;

		if (end && !strnicmp(end, Mp, endlen))
			return 0;

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || *Mp == '\0')
		return 0;

	return 1;
}

int SCPParser::required_string(const char* pstr)
{
	int count = 0;

	ignore_white_space();

	while (strnicmp(pstr, Mp, strlen(pstr)) != 0 && (count < RS_MAX_TRIES)) {
		error_display(1, "Missing required token: [%s]. Found [%.32s] instead.\n", pstr, next_tokens());
		advance_to_eoln(NULL);
		ignore_white_space();
		count++;
	}

	if (count == RS_MAX_TRIES) {
		throw parse::ParseException("Required string not found");
	}

	Mp += strlen(pstr);
	diag_printf("Found required string [%s]\n", token_found = pstr);
	return 1;
}

int SCPParser::check_for_eof_raw()
{
	if (*Mp == '\0')
		return 1;

	return 0;
}

int SCPParser::check_for_eof()
{
	ignore_white_space();

	return check_for_eof_raw();
}

int SCPParser::check_for_eoln()
{
	ignore_gray_space();

	if (*Mp == EOLN)
		return 1;
	else
		return 0;
}

int SCPParser::check_for_string(const char* pstr)
{
	ignore_white_space();

	if (!strnicmp(pstr, Mp, strlen(pstr)))
		return 1;

	return 0;
}

int SCPParser::check_for_string_raw(const char* pstr)
{
	if (!strnicmp(pstr, Mp, strlen(pstr)))
		return 1;

	return 0;
}

int SCPParser::optional_string(const char* pstr)
{
	ignore_white_space();

	if (!strnicmp(pstr, Mp, strlen(pstr))) {
		Mp += strlen(pstr);
		return 1;
	}

	return 0;
}

int SCPParser::optional_string_either(const char* str1, const char* str2)
{
	ignore_white_space();

	if (!strnicmp(str1, Mp, strlen(str1))) {
		Mp += strlen(str1);
		return 0;
	} else if (!strnicmp(str2, Mp, strlen(str2))) {
		Mp += strlen(str2);
		return 1;
	}

	return -1;
}

int SCPParser::optional_string_one_of(int arg_count, ...)
{
	Assertion(arg_count > 0, "optional_string_one_of() called with arg_count of %d; get a coder!\n", arg_count);
	int idx, found = -1;
	char* pstr;
	va_list vl;

	ignore_white_space();

	va_start(vl, arg_count);
	for (idx = 0; idx < arg_count; idx++) {
		pstr = va_arg(vl, char*);

		if (!strnicmp(pstr, Mp, strlen(pstr))) {
			Mp += strlen(pstr);
			found = idx;
			break;
		}
	}
	va_end(vl);

	return found;
}

int SCPParser::required_string_fred(const char* pstr, const char* end)
{
	char* backup = Mp;

	token_found = pstr;
	if (fred_parse_flag)
		return 0;

	ignore_white_space();
	while (*Mp != '\0' && strnicmp(pstr, Mp, strlen(pstr)) != 0) {
		if ((*Mp == '#') || (end && !strnicmp(end, Mp, strlen(end)))) {
			Mp = NULL;
			break;
		}

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || *Mp == '\0') {
		diag_printf("Required string [%s] not found\n", pstr);
		Mp               = backup;
		Token_found_flag = 0;
		return 0;
	}

	Mp += strlen(pstr);
	diag_printf("Found required string [%s]\n", pstr);
	Token_found_flag = 1;
	return 1;
}

int SCPParser::optional_string_fred(const char* pstr, const char* end, const char* end2)
{
	char* mp_save = Mp;

	token_found = pstr;
	if (fred_parse_flag)
		return 0;

	ignore_white_space();
	while ((*Mp != '\0') && strnicmp(pstr, Mp, strlen(pstr)) != 0) {
		if ((*Mp == '#') || (end && !strnicmp(end, Mp, strlen(end))) || (end2 && !strnicmp(end2, Mp, strlen(end2)))) {
			Mp = NULL;
			break;
		}

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (!Mp || *Mp == '\0') {
		diag_printf("Optional string [%s] not found\n", pstr);
		Mp               = mp_save;
		Token_found_flag = 0;
		return 0;
	}

	Mp += strlen(pstr);
	diag_printf("Found optional string [%s]\n", pstr);
	Token_found_flag = 1;
	return 1;
}

int SCPParser::required_string_either(const char* str1, const char* str2)
{
	ignore_white_space();

	for (int count = 0; count < RS_MAX_TRIES; ++count) {
		if (strnicmp(str1, Mp, strlen(str1)) == 0) {
			// Mp += strlen(str1);
			diag_printf("Found required string [%s]\n", token_found = str1);
			return 0;
		} else if (strnicmp(str2, Mp, strlen(str2)) == 0) {
			// Mp += strlen(str2);
			diag_printf("Found required string [%s]\n", token_found = str2);
			return 1;
		}

		error_display(1, "Required token = [%s] or [%s], found [%.32s].\n", str1, str2, next_tokens());

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	throw parse::ParseException("Required string not found");
}

int SCPParser::required_string_one_of(int arg_count, ...)
{
	Assertion(arg_count > 0, "required_string_one_of() called with arg_count of %d; get a coder!\n", arg_count);
	int count = 0;
	int idx;
	char* expected;
	SCP_string message = "";
	va_list vl;

	ignore_white_space();

	while (count < RS_MAX_TRIES) {
		va_start(vl, arg_count);
		for (idx = 0; idx < arg_count; idx++) {
			expected = va_arg(vl, char*);
			if (strnicmp(expected, Mp, strlen(expected)) == 0) {
				diag_printf("Found required string [%s]", token_found = expected);
				va_end(vl);
				return idx;
			}
		}
		va_end(vl);

		if (message.empty()) {
			va_start(vl, arg_count);
			message = "Required token = ";
			for (idx = 0; idx < arg_count; idx++) {
				message += "[";
				message += va_arg(vl, char*);
				message += "]";
				if (arg_count == 2 && idx == 0) {
					message += " or ";
				} else if (idx == arg_count - 2) {
					message += ", or ";
				} else if (idx < arg_count - 2) {
					message += ", ";
				}
			}
			va_end(vl);
		}

		error_display(1, "%s, found [%.32s]\n", message.c_str(), next_tokens());
		advance_to_eoln(NULL);
		ignore_white_space();
		count++;
	}

	return -1;
}

int SCPParser::required_string_either_fred(const char* str1, const char* str2)
{
	ignore_white_space();

	while (*Mp != '\0') {
		if (!strnicmp(str1, Mp, strlen(str1))) {
			// Mp += strlen(str1);
			diag_printf("Found required string [%s]\n", token_found = str1);
			return fred_parse_flag = 0;

		} else if (!strnicmp(str2, Mp, strlen(str2))) {
			// Mp += strlen(str2);
			diag_printf("Found required string [%s]\n", token_found = str2);
			return fred_parse_flag = 1;
		}

		advance_to_eoln(NULL);
		ignore_white_space();
	}

	if (*Mp == '\0')
		diag_printf("Unable to find either required token [%s] or [%s]\n", str1, str2);

	return -1;
}

void SCPParser::stuff_string_white(char* outstr, int len)
{
	if (!len)
		len = NAME_LENGTH - 1;

	ignore_white_space();
	copy_to_next_white(outstr, Mp, len);
	advance_to_next_white();
}

void SCPParser::stuff_string_white(SCP_string& outstr)
{
	ignore_white_space();
	copy_to_next_white(outstr, Mp);
	advance_to_next_white();
}

void SCPParser::stuff_string_until(char* outstr, const char* endstr, int len)
{
	if (!len)
		len = NAME_LENGTH - 1;

	ignore_gray_space();
	copy_text_until(outstr, Mp, endstr, len);
	Mp += strlen(outstr);
	drop_trailing_white_space(outstr);
}

void SCPParser::stuff_string_until(SCP_string& outstr, const char* endstr)
{
	ignore_gray_space();
	copy_text_until(outstr, Mp, endstr);
	Mp += outstr.length();
	drop_trailing_white_space(outstr);
}

int SCPParser::get_string_or_variable(char* str)
{
	int result = -1;

	ignore_white_space();

	// Variable
	if (*Mp == '@') {
		Mp++;
		stuff_string_white(str);
		int sexp_variable_index = get_index_sexp_variable_name(str);

		// We only want String variables
		Assertion(sexp_variable_index != -1, "Didn't find variable name \"%s\"", str);
		Assert(Sexp_variables[sexp_variable_index].type & SEXP_VARIABLE_STRING);

		result = PARSING_FOUND_VARIABLE;
	}
	// Quoted string
	else if (*Mp == '"') {
		get_string(str);
		result = PARSING_FOUND_STRING;
	} else {
		get_string(str);
		GOutputDevice->Error(
			LOCATION,
			"Invalid entry \"%s\"  found in get_string_or_variable. Must be a quoted string or a string variable name.",
			str);
	}

	return result;
}

int SCPParser::get_string_or_variable(SCP_string& str)
{
	int result = -1;

	ignore_white_space();

	// Variable
	if (*Mp == '@') {
		Mp++;
		stuff_string_white(str);
		int sexp_variable_index = get_index_sexp_variable_name(str);

		// We only want String variables
		Assertion(sexp_variable_index != -1, "Didn't find variable name \"%s\"", str.c_str());
		Assert(Sexp_variables[sexp_variable_index].type & SEXP_VARIABLE_STRING);

		result = PARSING_FOUND_VARIABLE;
	}
	// Quoted string
	else if (*Mp == '"') {
		get_string(str);
		result = PARSING_FOUND_STRING;
	} else {
		get_string(str);
		GOutputDevice->Error(
			LOCATION,
			"Invalid entry \"%s\"  found in get_string_or_variable. Must be a quoted string or a string variable name.",
			str.c_str());
	}

	return result;
}

int SCPParser::get_string(char* str, int max)
{
	auto len = strcspn(Mp + 1, "\"");

	if (max >= 0 && len >= (size_t)max)
		error_display(0, "String too long.  Length = " SIZE_T_ARG ".  Max is %i.\n", len, max);

	strncpy(str, Mp + 1, len);
	str[len] = 0;

	Mp += len + 2;
	return (int)len;
}

void SCPParser::get_string(SCP_string& str)
{
	auto len = strcspn(Mp + 1, "\"");
	str.assign(Mp + 1, len);

	Mp += len + 2;
}

void SCPParser::stuff_string(char* outstr, int type, int len, const char* terminators)
{
	char read_str[PARSE_BUF_SIZE] = "";
	int read_len                  = PARSE_BUF_SIZE;
	int final_len                 = len - 1;
	int tag_id;

	// make sure we have enough room
	Assert(final_len > 0);

	// make sure it's zero'd out
	memset(outstr, 0, len);

	switch (type) {
	case F_RAW:
	case F_LNAME:
	case F_NAME:
	case F_DATE:
	case F_FILESPEC:
	case F_PATHNAME:
	case F_MESSAGE:
		ignore_gray_space();
		copy_to_eoln(read_str, terminators, Mp, read_len);
		drop_trailing_white_space(read_str);
		advance_to_eoln(terminators);
		break;

	case F_NOTES:
		ignore_white_space();
		copy_text_until(read_str, Mp, "$End Notes:", read_len);
		Mp += strlen(read_str);
		required_string("$End Notes:");
		break;

		// F_MULTITEXTOLD keeping for backwards compatability with old missions
		// can be deleted once all missions are using new briefing format

	case F_MULTITEXTOLD:
		ignore_white_space();
		copy_text_until(read_str, Mp, "$End Briefing Text:", read_len);
		Mp += strlen(read_str);
		required_string("$End Briefing Text:");
		break;

	case F_MULTITEXT:
		ignore_white_space();
		copy_text_until(read_str, Mp, "$end_multi_text", read_len);
		Mp += strlen(read_str);
		drop_trailing_white_space(read_str);
		required_string("$end_multi_text");
		break;

	default:
		GOutputDevice->Error(LOCATION, "Unhandled string type %d in stuff_string!", type);
	}

	if (type == F_FILESPEC) {
		// Make sure that the passed string looks like a good filename
		if (strlen(read_str) == 0) {
			// Empty file name is probably not valid!
			error_display(0, "A file name was expected but no name was supplied! This is probably a mistake.");
		}
	}

	strncpy(outstr, read_str, strlen(read_str));
	diag_printf("Stuffed string = [%.30s]\n", outstr);
}

void SCPParser::stuff_string(SCP_string& outstr, int type, const char* terminators)
{
	SCP_string read_str;
	int tag_id;

	// make sure it's zero'd out
	outstr = "";

	switch (type) {
	case F_RAW:
	case F_LNAME:
	case F_NAME:
	case F_DATE:
	case F_FILESPEC:
	case F_PATHNAME:
	case F_MESSAGE:
		ignore_gray_space();
		copy_to_eoln(read_str, terminators, Mp);
		drop_trailing_white_space(read_str);
		advance_to_eoln(terminators);
		break;

	case F_NOTES:
		ignore_white_space();
		copy_text_until(read_str, Mp, "$End Notes:");
		Mp += read_str.length();
		required_string("$End Notes:");
		break;

		// F_MULTITEXTOLD keeping for backwards compatability with old missions
		// can be deleted once all missions are using new briefing format

	case F_MULTITEXTOLD:
		ignore_white_space();
		copy_text_until(read_str, Mp, "$End Briefing Text:");
		Mp += read_str.length();
		required_string("$End Briefing Text:");
		break;

	case F_MULTITEXT:
		ignore_white_space();
		copy_text_until(read_str, Mp, "$end_multi_text");
		Mp += read_str.length();
		drop_trailing_white_space(read_str);
		required_string("$end_multi_text");
		break;

	default:
		GOutputDevice->Error(LOCATION, "Unhandled string type %d in stuff_string!", type);
	}

	if (type == F_FILESPEC) {
		// Make sure that the passed string looks like a good filename
		if (read_str.empty()) {
			// Empty file name is not valid!
			error_display(1, "A file name was expected but no name was supplied!\n");
		}
	}
	outstr = read_str;

	diag_printf("Stuffed string = [%.30s]\n", outstr.c_str());
}

void SCPParser::stuff_string_line(char* outstr, int len)
{
	char read_str[PARSE_BUF_SIZE] = "";
	int read_len                  = PARSE_BUF_SIZE;
	int final_len                 = len - 1;
	int tag_id;

	Assert(final_len > 0);

	// read in a line
	copy_to_eoln(read_str, "\n", Mp, read_len);
	drop_trailing_white_space(read_str);
	advance_to_eoln("");
	Mp++;

	diag_printf("Stuffed string = [%.30s]\n", outstr);
}

void SCPParser::stuff_string_line(SCP_string& outstr)
{
	SCP_string read_str;
	int tag_id;

	// read in a line
	copy_to_eoln(read_str, "\n", Mp);
	drop_trailing_white_space(read_str);
	advance_to_eoln("");
	Mp++;

	diag_printf("Stuffed string = [%.30s]\n", outstr.c_str());
}

char* SCPParser::stuff_and_malloc_string(int type, const char* terminators)
{
	SCP_string tmp_result;

	stuff_string(tmp_result, type, terminators);
	drop_white_space(tmp_result);

	if (tmp_result.empty())
		return NULL;

	return vm_strdup(tmp_result.c_str());
}

void SCPParser::stuff_malloc_string(char** dest, int type, const char* terminators)
{
	Assert(dest != NULL); // wtf?

	char* new_val = stuff_and_malloc_string(type, terminators);

	if (new_val != NULL) {
		if ((*dest) != NULL) {
			vm_free(*dest);
		}

		(*dest) = new_val;
	}
}

bool SCPParser::atof2(float* out)
{
	ignore_white_space();
	char ch = *Mp;

	if ((ch != '.') && (ch != '-') && (ch != '+') && ((ch < '0') || (ch > '9'))) {
		error_display(1, "Expecting float, found [%.32s].\n", next_tokens());
		*out = 0.0f;
		return false;
	}

	*out = (float)atof(Mp);
	return true;
}

bool SCPParser::atoi2(int* out)
{
	ignore_white_space();
	char ch = *Mp;

	if ((ch != '-') && (ch != '+') && ((ch < '0') || (ch > '9'))) {
		error_display(1, "Expecting int, found [%.32s].\n", next_tokens());
		*out = 0;
		return false;
	}

	*out = atoi(Mp);
	return true;
}

bool SCPParser::atol2(long* out)
{
	ignore_white_space();
	char ch = *Mp;

	if ((ch != '-') && (ch != '+') && ((ch < '0') || (ch > '9'))) {
		error_display(1, "Expecting long, found [%.32s].\n", next_tokens());
		*out = 0;
		return false;
	}

	*out = atol(Mp);
	return true;
}

void SCPParser::stuff_float(float* f)
{
	bool success = atof2(f);

	if (!success)
		skip_token();
	else
		Mp += strspn(Mp, "+-0123456789.");

	if (*Mp == ',')
		Mp++;

	diag_printf("Stuffed float: %f\n", *f);
}

int SCPParser::stuff_float_optional(float* f, bool raw)
{
	bool comma = false;

	if (!raw)
		ignore_white_space();

	auto skip_len = strspn(Mp, "+-0123456789.");
	if (*(Mp + skip_len) == ',') {
		comma = true;
	}

	if (skip_len == 0) {
		if (comma) {
			Mp++;
			return 1;
		} else {
			return 0;
		}
	}

	stuff_float(f);
	return 2;
}

void SCPParser::stuff_int(int* i)
{
	bool success = atoi2(i);

	if (!success)
		skip_token();
	else
		Mp += strspn(Mp, "+-0123456789");

	if (*Mp == ',')
		Mp++;

	diag_printf("Stuffed int: %i\n", *i);
}

int SCPParser::stuff_int_optional(int* i, bool raw)
{
	bool comma = false;

	if (!raw)
		ignore_white_space();

	auto skip_len = strspn(Mp, "+-0123456789");
	if (*(Mp + skip_len) == ',') {
		comma = true;
	}

	if (skip_len == 0) {
		if (comma) {
			Mp++;
			return 1;
		} else {
			return 0;
		}
	}

	stuff_int(i);
	return 2;
}

int SCPParser::stuff_int_or_variable(int& i, bool positive_value)
{
	int index = NOT_SET_BY_SEXP_VARIABLE;

	if (*Mp == '@') {
		Mp++;
		int value = -1;
		char str[128];
		stuff_string(str, F_NAME, sizeof(str));

		index = get_index_sexp_variable_name(str);

		if (index > -1 && index < MAX_SEXP_VARIABLES) {
			if (Sexp_variables[index].type & SEXP_VARIABLE_NUMBER) {
				value = atoi(Sexp_variables[index].text);
			} else {
				GOutputDevice->Error(
					LOCATION,
					"Invalid variable type \"%s\" found in mission. Variable must be a number variable!",
					str);
			}
		} else {

			GOutputDevice->Error(LOCATION, "Invalid variable name \"%s\" found.", str);
		}

		// zero negative values if requested
		if (positive_value && value < 0) {
			value = 0;
		}

		// Record the value of the index for FreeSpace
		i = value;
	} else {
		stuff_int(&i);
	}
	return index;
}

int SCPParser::stuff_int_or_variable(int* ilp, int count, bool positive_value)
{
	if (*Mp == '@') {
		Mp++;
		int value = -1;
		char str[128];
		stuff_string(str, F_NAME, sizeof(str));

		int index = get_index_sexp_variable_name(str);

		if (index > -1 && index < MAX_SEXP_VARIABLES) {
			if (Sexp_variables[index].type & SEXP_VARIABLE_NUMBER) {
				value = atoi(Sexp_variables[index].text);
			} else {
				GOutputDevice->Error(
					LOCATION,
					"Invalid variable type \"%s\" found in mission. Variable must be a number variable!",
					str);
			}
		} else {

			GOutputDevice->Error(LOCATION, "Invalid variable name \"%s\" found.", str);
		}

		// zero negative values if requested
		if (positive_value && value < 0) {
			value = 0;
		}

		// Record the value of the index for FreeSpace
		ilp[count++] = value;
		// Record the index itself because we may need it later.
		ilp[count++] = index;
	} else {
		stuff_int(&ilp[count++]);
		// Since we have a numerical value we don't have a SEXP variable index to add for next slot.
		ilp[count++] = NOT_SET_BY_SEXP_VARIABLE;
	}
	return count;
}

void SCPParser::stuff_boolean(int* i, bool a_to_eol)
{
	bool tempb;
	stuff_boolean(&tempb, a_to_eol);
	if (tempb)
		*i = 1;
	else
		*i = 0;
}

void SCPParser::stuff_boolean(bool* b, bool a_to_eol)
{
	char token[32];
	stuff_string_white(token, sizeof(token) / sizeof(char));
	if (a_to_eol)
		advance_to_eoln(NULL);

	if (isdigit(token[0])) {
		if (token[0] != '0')
			*b = true;
		else
			*b = false;
	} else {
		if (!stricmp(token, "yes") || !stricmp(token, "true") || !stricmp(token, "ja") // German
			|| !stricmp(token, "Oui")                                                  // French
			|| !stricmp(token, "si")                                                   // Spanish
			|| !stricmp(token, "ita vero")                                             // Latin
			|| !stricmp(token, "HIja'") || !stricmp(token, "HISlaH"))                  // Klingon
		{
			*b = true;
		} else if (!stricmp(token, "no") || !stricmp(token, "false") || !stricmp(token, "nein") // German
				   || !stricmp(token, "Non")                                                    // French
				   // I don't know spanish for "no"
				   // But according to altavista, spanish for "No" is "no"
				   // Go figure.
				   || !stricmp(token, "minime")  // Latin
				   || !stricmp(token, "ghobe'")) // Klingon
		{
			*b = false;
		} else {
			*b = false;
			error_display(0, "Boolean '%s' type unknown; assuming 'no/false'", token);
		}
	}

	diag_printf("Stuffed bool: %s\n", (b) ? NOX("true") : NOX("false"));
}

int SCPParser::stuff_bool_list(bool* blp, int max_bools)
{
	int count      = 0;
	bool trash_buf = false;

	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading boolean list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;

	ignore_white_space();

	while (*Mp != ')') {
		if (count < max_bools) {
			stuff_boolean(&blp[count++], false);
			ignore_white_space();

			// Since Bobb has set a precedent, allow commas for bool lists -WMC
			if (*Mp == ',') {
				Mp++;
				ignore_white_space();
			}
		} else {
			trash_buf = true;
			break;
		}
	}

	if (trash_buf) {
		error_display(0,
					  "Boolean list has more than allowed arguments; max is %d. Arguments over max will be ignored.",
					  max_bools);
		while (*Mp != ')') {
			stuff_boolean(&trash_buf, false);
			ignore_white_space();
		}
	}

	Mp++;

	return count;
}

void SCPParser::stuff_ubyte(ubyte* i)
{
	int temp;
	bool success = atoi2(&temp);

	*i = (ubyte)temp;

	if (!success)
		skip_token();
	else
		Mp += strspn(Mp, "+-0123456789");

	if (*Mp == ',')
		Mp++;

	diag_printf("Stuffed byte: %i\n", *i);
}

int SCPParser::parse_string_flag_list(int* dest, flag_def_list defs[], int defs_size)
{
	Assert(dest != NULL); // wtf?

	char(*slp)[NAME_LENGTH] = (char(*)[32]) new char[defs_size * NAME_LENGTH];
	int num_strings         = stuff_string_list(slp, defs_size);
	int i, j;

	for (i = 0; i < num_strings; i++) {
		for (j = 0; j < defs_size; j++) {
			if (!stricmp(slp[i], defs[j].name)) {
				(*dest) |= defs[j].def;
			}
		}
	}

	delete[] slp; //>_>
	// nobody saw that right

	return num_strings;
}

int SCPParser::stuff_string_list(SCP_vector<SCP_string>& slp)
{
	//_asm int 3;
	slp.clear();

	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading string list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;

	ignore_white_space();

	SCP_string buf;

	while (*Mp != ')') {
		if (*Mp != '\"') {
			error_display(0, "Missing quotation marks in string list.");
		}
		// Assert ( *Mp == '\"' );					// should always be enclosed in quotes

		buf = "";
		get_string(buf);
		slp.push_back(buf);
		ignore_white_space();
	}

	Mp++;

	return (int)(slp.size());
}

int SCPParser::stuff_string_list(char slp[][NAME_LENGTH], int max_strings)
{
	int count = 0;
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading string list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;

	ignore_white_space();

	while (*Mp != ')') {
		Assert(count < max_strings);
		if (*Mp != '\"') {
			error_display(0, "Missing quotation marks in string list.");
		}
		// Assert ( *Mp == '\"' );					// should always be enclosed in quotes

		if (count < max_strings) {
			get_string(slp[count++]);
		} else {
			char trash[NAME_LENGTH];
			get_string(trash);
		}
		ignore_white_space();
	}

	Mp++;

	return count;
}

int SCPParser::stuff_int_list(int* ilp, int max_ints, int lookup_type)
{
	Assertion(max_ints > 0, "Requested to parse an integer list with a maximum of 0 entries!");

	int count = 0, ok_flag = 1, dummy;
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading integer list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;
	ignore_white_space();

	while (*Mp != ')') {
		if (*Mp == '"') {
			int num = 0;
			char str[128];

			get_string(str);
			switch (lookup_type) {
			case SHIP_TYPE:
				num = ship_name_lookup(str); // returns index of Ship[] entry with name
				break;

			case SHIP_INFO_TYPE:
				ok_flag = 1;
				num     = ship_info_lookup(str); // returns index of Ship_info[] entry with name
				if (num < 0)
					ok_flag = 0;
				break;

			case WEAPON_POOL_TYPE:
				ok_flag = 1;
				num     = weapon_info_lookup(str);
				if (num < 0)
					ok_flag = 0;
				break;

			case WEAPON_LIST_TYPE:
				num = weapon_info_lookup(str);
				if (num < 0)
					num = -2;
				break;

			case RAW_INTEGER_TYPE:
				num = atoi(str);
				break;

			default:
				GOutputDevice->Error(LOCATION, "Unknown lookup_type %d in stuff_int_list", lookup_type);
				break;
			}

			if (ok_flag) {
				if (num == -1) {
					error_display(1,
								  "Unable to find string \"%s\" in stuff_int_list\n\nMany possible sources for "
								  "this error.  Get a programmer!\n",
								  str);
				} else if (num == -2) {
					if (str[0] != '\0') {
						error_display(0,
									  "Unable to find WEAPON_LIST_TYPE string \"%s\" in stuff_int_list\n\nMany "
									  "possible sources for this error.  Get a programmer!\n",
									  str);
					}
				}

				if (num < 0) // other negatives used to bypass the above error trap, but should be -1
					num = -1;

				if (count < max_ints) {
					ilp[count] = num;
				}
				++count;
			}

		} else {
			if (ok_flag && (count < max_ints))
				stuff_int(&ilp[count]);
			else
				stuff_int(&dummy);
			++count;
		}

		ignore_white_space();
	}

	Mp++;

	if (count > max_ints) {
		error_display(0,
					  "Too many entries in integer list. Expected %d, found %d.\nList type was %s",
					  max_ints,
					  count,
					  get_lookup_type_name(lookup_type));
	}

	return count;
}

void SCPParser::clean_loadout_list_entry()
{
	int dummy;

	// clean out the broken entry
	ignore_white_space();
	stuff_int_or_variable(dummy);
	ignore_white_space();
}

int SCPParser::stuff_loadout_list(int* ilp, int max_ints, int lookup_type)
{
	int count = 0;
	int index, sexp_variable_index, variable_found;
	char str[128];

	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading loadout list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;
	ignore_white_space();

	while (*Mp != ')') {
		if (count >= max_ints) {
			GOutputDevice->Error(LOCATION, "Loadout contains too many entries.\n");
		}

		index               = -1;
		sexp_variable_index = NOT_SET_BY_SEXP_VARIABLE;
		variable_found      = get_string_or_variable(str);

		// if we've got a variable get the variable index and copy it's value into str so that regardless of whether
		// we found a variable or not it now holds the name of the ship or weapon we're interested in.
		if (variable_found) {
			Assert(lookup_type != CAMPAIGN_LOADOUT_SHIP_LIST);
			sexp_variable_index = get_index_sexp_variable_name(str);

			if (sexp_variable_index < 0) {
				GOutputDevice->Error(LOCATION, "Invalid SEXP variable name \"%s\" found in stuff_loadout_list.", str);
			}

			strcpy_s(str, Sexp_variables[sexp_variable_index].text);
		}

		switch (lookup_type) {
		case MISSION_LOADOUT_SHIP_LIST:
		case CAMPAIGN_LOADOUT_SHIP_LIST:
			index = ship_info_lookup(str);
			break;

		case MISSION_LOADOUT_WEAPON_LIST:
		case CAMPAIGN_LOADOUT_WEAPON_LIST:
			index = weapon_info_lookup(str);
			break;

		default:
			Int3();
		}

		// Complain if this isn't a valid ship or weapon and we are loading a mission. Campaign files can be loading
		// containing no ships from the current tables (when swapping mods) so don't report that as an error.
		if (index < 0 && (lookup_type == MISSION_LOADOUT_SHIP_LIST || lookup_type == MISSION_LOADOUT_WEAPON_LIST)) {
			// print a warning in debug mode
			error_display(0, "Invalid type \"%s\" found in loadout of mission file...skipping", str);
			// increment counter for release FRED builds.
			Num_unknown_loadout_classes++;

			clean_loadout_list_entry();
			continue;
		}

		// similarly, complain if this is a valid ship or weapon class that the player can't use
		if ((lookup_type == MISSION_LOADOUT_SHIP_LIST) && (!(Ship_info[index].flags[Ship::Info_Flags::Player_ship]))) {
			clean_loadout_list_entry();
			error_display(0,
						  "Ship type \"%s\" found in loadout of mission file. This class is not marked as a player "
						  "ship...skipping",
						  str);
			continue;
		} else if ((lookup_type == MISSION_LOADOUT_WEAPON_LIST) &&
				   (!(Weapon_info[index].wi_flags[Weapon::Info_Flags::Player_allowed]))) {
			clean_loadout_list_entry();
			nprintf(("Warning",
					 "Warning: Weapon type %s found in loadout of mission file. This class is not marked as a "
					 "player allowed weapon...skipping\n",
					 str));
			if (!Is_standalone)
				error_display(0,
							  "Weapon type \"%s\" found in loadout of mission file. This class is not marked as a "
							  "player allowed weapon...skipping",
							  str);
			continue;
		}

		// we've found a real item. Add its index to the list.
		if (count < max_ints) {
			ilp[count++] = index;
		}

		ignore_white_space();

		// Campaign lists need go no further
		if (lookup_type == CAMPAIGN_LOADOUT_SHIP_LIST || lookup_type == CAMPAIGN_LOADOUT_WEAPON_LIST) {
			continue;
		}

		// record the index of the variable that gave us this item if any
		if (count < max_ints) {
			ilp[count++] = sexp_variable_index;
		}

		// Now read in the number of this type available. The number must be positive
		count = stuff_int_or_variable(ilp, count, true);

		ignore_white_space();
	}

	Mp++;
	return count;
}

size_t SCPParser::stuff_float_list(float* flp, size_t max_floats)
{
	size_t count = 0;
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading float list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;
	ignore_white_space();
	while (*Mp != ')') {
		Assert(count < max_floats);
		if (count < max_floats) {
			stuff_float(&flp[count++]);
		} else {
			float dummy;
			stuff_float(&dummy);
		}
		ignore_white_space();
	}

	Mp++;

	return count;
}

void SCPParser::mark_int_list(int* ilp, int max_ints, int lookup_type)
{
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Marking integer list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;
	ignore_white_space();

	while (*Mp != ')') {
		if (*Mp == '"') {
			int num = 0;
			char str[128];

			get_string(str);
			switch (lookup_type) {
			case SHIP_TYPE:
				//TODO: @SCPParser this may not work
				num = ObjectLookup<SHIP_TYPE>(str);
				num = ship_name_lookup(str); // returns index of Ship[] entry with name
				break;

			case SHIP_INFO_TYPE:
				num = ship_info_lookup(str); // returns index of Ship_info[] entry with name
				break;

			case WEAPON_LIST_TYPE:
				num = weapon_info_lookup(str);
				break;

			default:
				GOutputDevice->Error(LOCATION, "Unknown lookup_type %d in mark_int_list", lookup_type);
				break;
			}

			if ((num < 0) || (num >= max_ints))
				GOutputDevice->Error(LOCATION, "Unable to find string \"%s\" in mark_int_list.\n", str);

			//			ilp[num] = 1;

		} else {
			int tval;

			stuff_int(&tval);
			Assert((tval >= 0) && (tval < max_ints));
			if (tval >= 0 && tval < max_ints) {
				ilp[tval] = 1;
			}
		}

		ignore_white_space();
	}

	Mp++;
}

void SCPParser::stuff_vec3d(vec3d* vp)
{
	stuff_float(&vp->xyz.x);
	stuff_float(&vp->xyz.y);
	stuff_float(&vp->xyz.z);
}

void SCPParser::stuff_parenthesized_vec3d(vec3d* vp)
{
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading parenthesized vec3d.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	} else {
		Mp++;
		stuff_vec3d(vp);
		ignore_white_space();
		if (*Mp != ')') {
			error_display(1, "Reading parenthesized vec3d.  Found [%c].  Expecting ')'.\n", *Mp);
			throw parse::ParseException("Syntax error");
		}
		Mp++;
	}
}

int SCPParser::stuff_vec3d_list(vec3d* vlp, int max_vecs)
{
	int count = 0;

	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading vec3d list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;

	ignore_white_space();

	while (*Mp != ')') {
		Assert(count < max_vecs);
		if (count < max_vecs) {
			stuff_parenthesized_vec3d(&vlp[count++]);
		} else {
			vec3d temp;
			stuff_parenthesized_vec3d(&temp);
		}

		ignore_white_space();
	}

	Mp++;

	return count;
}

int SCPParser::stuff_vec3d_list(SCP_vector<vec3d>& vec_list)
{
	ignore_white_space();

	if (*Mp != '(') {
		error_display(1, "Reading vec3d list.  Found [%c].  Expecting '('.\n", *Mp);
		throw parse::ParseException("Syntax error");
	}

	Mp++;

	ignore_white_space();

	while (*Mp != ')') {
		vec3d temp;
		stuff_parenthesized_vec3d(&temp);
		vec_list.push_back(temp);

		ignore_white_space();
	}

	Mp++;

	return (int)vec_list.size();
}

void SCPParser::stuff_matrix(matrix* mp)
{
	stuff_vec3d(&mp->vec.rvec);
	stuff_vec3d(&mp->vec.uvec);
	stuff_vec3d(&mp->vec.fvec);
}

void SCPParser::find_and_stuff(
	const char* id, int* addr, int f_type, const char* strlist[], size_t max, const char* description)
{
	char token[128];
	int checking_ship_classes = (stricmp(id, "$class:") == 0);

	// Goober5000 - don't say errors when we're checking classes because 1) we have more checking to do; and 2) we
	// will say a redundant error later
	required_string(id);
	stuff_string(token, f_type, sizeof(token));
	*addr = string_lookup(token, strlist, max, description, !checking_ship_classes);

	// Goober5000 - handle certain FSPort idiosyncracies with ship classes
	if (*addr < 0 && checking_ship_classes) {
		int idx = ship_info_lookup(token);

		if (idx >= 0)
			*addr = string_lookup(Ship_info[idx].name, strlist, max, description, 0);
		else
			*addr = -1;
	}
}

void SCPParser::find_and_stuff_optional(
	const char* id, int* addr, int f_type, const char* const* strlist, size_t max, const char* description)
{
	char token[128];

	if (optional_string(id)) {
		stuff_string(token, f_type, sizeof(token));
		*addr = string_lookup(token, strlist, max, description, 1);
	}
}

int SCPParser::match_and_stuff(int f_type, const char* const* strlist, int max, const char* description)
{
	char token[128];

	stuff_string(token, f_type, sizeof(token));
	return string_lookup(token, strlist, max, description, 0);
}

void SCPParser::find_and_stuff_or_add(
	const char* id, int* addr, int f_type, char* strlist[], int* total, int max, const char* description)
{
	char token[128];

	*addr = -1;
	required_string(id);
	stuff_string(token, f_type, sizeof(token));
	if (*total)
		*addr = string_lookup(token, strlist, *total, description, 0);

	if (*addr == -1) // not in list, so lets try and add it.
	{
		Assert(*total < max);
		strcpy(strlist[*total], token);
		*addr = (*total)++;
	}
}

void SCPParser::stuff_boolean_flag(int* i, int flag, bool a_to_eol)
{
	bool temp;
	stuff_boolean(&temp, a_to_eol);
	if (temp)
		*i |= flag;
	else
		*i &= ~(flag);
}
