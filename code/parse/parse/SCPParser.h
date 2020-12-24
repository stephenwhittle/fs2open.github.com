#pragma once
#include "FSIntegerTypes.h"
#include "FSStdTypes.h"
#include "FSMathTypes.h"
#include <memory>
#include "parse/SCPTableFormatDescriptor.h"
#include "parse/SCPTable.h"
#include "memory/utils.h"
#include "utils/encoding.h"
//
//SCPParser takes ownership of the underlying data pointer
class SCPParser
{
    
public:
	
	template<typename TableType>
	static tl::optional<TableType> CreateBaseTable(SCPTableFormatDescriptor& TableFormat, SCP_buffer InRawData, SCP_string Filename)
	{
		std::shared_ptr<SCPParsedTableData> my_ast;
		
		SCP_buffer ProcessedData = util::ConvertFromLatinEncoding(InRawData, Filename);
		
		TableFormat.UseRuleToParseInput("Root", ProcessedData, my_ast);

		if (my_ast != nullptr) {
			return construct<TableType>(*my_ast);
		}
		else
		{
			return tl::optional<TableType>();
		}
	}
	template<typename TableType>
	static void LoadIntoTable( SCPTableBase<TableType>& Table, SCPTableFormatDescriptor& TableFormat, SCP_buffer InRawData, SCP_string Filename)
	{
		std::shared_ptr<SCPParsedTableData> my_ast;

		SCP_buffer ProcessedData = util::ConvertFromLatinEncoding(InRawData, Filename);

		TableFormat.UseRuleToParseInput("Root", ProcessedData, my_ast);

		if (my_ast != nullptr) {
			for (auto Node : my_ast->nodes)
			{
				Table.Deserialize(Node->name, *Node);
			}
		}
	}
	/*template <typename TableType>
	static tl::optional<TableType> LoadModularTable(const SCPTableFormatDescriptor& TableFormat, SCP_string FileFilter, int PathType, int SortOrder )
	{
		tl::optional<TableType> LoadedTable;

		SCP_vector<SCP_string> TableFileNames;
		cf_get_file_list(TableFileNames, PathType, FileFilter.c_str(), SortOrder);

		for (SCP_string FileName : TableFileNames)
		{
			SCP_buffer TableBuffer = read_file_text(FileName, CF_TYPE_TABLES);

		}
	}*/
		/*

	// Advance Mp to the next white space (ignoring white space inside of " marks)
	void advance_to_next_white();

	//	Advance Mp to the next eoln character.
	void advance_to_eoln(const char* more_terminators);

	//	Advance global Mp (mission pointer) past all current white space.
	//	Leaves Mp pointing at first non white space character.
	void ignore_white_space();

	// Search for specified string, skipping everything up to that point.  Returns 1 if found,
	// 0 if string wasn't found (and hit end of file), or -1 if not found, but end of checking
	// block was reached.
	int skip_to_string(const char* pstr, const char* end);

	//	Advances Mp past current token.
	void skip_token();

	// Goober5000
	// Advance to start of pstr.  Return 0 is successful, otherwise return !0
	int skip_to_start_of_string(const char* pstr, const char* end);

	// Advance to start of either pstr1 or pstr2.  Return 0 is successful, otherwise return !0
	int skip_to_start_of_string_either(const char* pstr1, const char* pstr2, const char* end);

	// Find a required string.
	// If not found, display an error message, but try up to RS_MAX_TRIES times
	// to find the string.  (This is the groundwork for ignoring non-understood
	// lines.
	//	If unable to find the required string after RS_MAX_TRIES tries, then
	//	abort using longjmp to parse_abort.
	int required_string(const char* pstr);

	int check_for_eof_raw();

	int check_for_eof();

	/ **
	Returns 1 if it finds a newline character precded by any amount of grayspace.
	* /
	int check_for_eoln();

	// similar to optional_string, but just checks if next token is a match.
	// It doesn't advance Mp except to skip past white space.
	int check_for_string(const char* pstr);

	// like check for string, but doesn't skip past any whitespace
	int check_for_string_raw(const char* pstr);

	// Find an optional string.
	//	If found, return 1, else return 0.
	//	If found, point past string, else don't update pointer.
	int optional_string(const char* pstr);

	int optional_string_either(const char* str1, const char* str2);

	// generic parallel to required_string_one_of
	int optional_string_one_of(int arg_count, ...);

	int required_string_fred(const char* pstr, const char* end);

	// attempt to find token in buffer.  It might not exist, however, in which case we don't need
	// to do anything.  If it is found, then we advance the pointer to just after the token.  To
	// further complicate things, we should only search to a certain point, since we don't want
	// a token that belongs to another section which might match the token we want.  Thus, we
	// also pass in an ending token, which marks the point we should stop looking at.
	int optional_string_fred(const char* pstr, const char* end, const char* end2);

	/ **
	 * @brief Checks for one of two required strings
	 *
	 * @retval 0 for str1 match
	 * @retval 1 for str2 match
	 * @throws parse::ParseException If neither strings were found
	 *
	 * @details Advances the Mp until a string is found or exceeds RS_MAX_TRIES. Once a string is found, Mp is located at
	 * the start of the found string.
	 * /
	int required_string_either(const char* str1, const char* str2);

	/ **
	 * @brief Checks for one of any of the given required strings.
	 *
	 * @returns The index number of the found string, if it was found
	 * @returns -1 if a string was not found
	 *
	 * @details By ngld, with some tweaks by MageKing17.
	 * /
	int required_string_one_of(int arg_count, ...);

	int required_string_either_fred(const char* str1, const char* str2);


	// stuffs a string into a buffer.  Can get a string between " marks and stops
	// when whitespace is encounted -- not to end of line
	void stuff_string_white(char* outstr, int len = 0);

	// ditto for SCP_string
	void stuff_string_white(SCP_string& outstr);

	// Goober5000
	void stuff_string_until(char* outstr, const char* endstr, int len = 0);

	// Goober5000
	void stuff_string_until(SCP_string& outstr, const char* endstr);



	// Karajorma - Stuffs the provided char array with either the contents of a quoted string or the name of a string
	// variable. Returns PARSING_FOUND_STRING if a string was found or PARSING_FOUND_VARIABLE if a variable was present.
	int get_string_or_variable(char* str);

	// ditto for SCP_string
	int get_string_or_variable(SCP_string& str);

	/ **
	 * Stuff a string (" chars ") into *str, return length.
	 * Accepts an optional max length parameter. If it is omitted or negative, then no max length is enforced.
	 * /
	int get_string(char* str, int max = -1);

	/ **
	 * Stuff a string (" chars ") into str.
	 * /
	void get_string(SCP_string& str);

	//	Stuff a string into a string buffer.
	//	Supports various FreeSpace primitive types.  If 'len' is supplied, it will override
	// the default string length if using the F_NAME case.
	void stuff_string(char* outstr, int type, int len, const char* terminators = nullptr);

	//	Stuff a string into a string buffer.
	//	Supports various FreeSpace primitive types.
	void stuff_string(SCP_string& outstr, int type, const char* terminators = nullptr);

	// stuff a string, but only until the end of a line. don't ignore leading whitespace. close analog of fgets()/cfgets()
	void stuff_string_line(char* outstr, int len);

	// ditto for SCP_string
	void stuff_string_line(SCP_string& outstr);

	// Exactly the same as stuff string only Malloc's the buffer.
	//	Supports various FreeSpace primitive types.  If 'len' is supplied, it will override
	// the default string length if using the F_NAME case.
	char* stuff_and_malloc_string(int type, const char* terminators);

	void stuff_malloc_string(char** dest, int type, const char* terminators);


	bool atof2(float* out);

	bool atoi2(int* out);

	bool atol2(long* out);

	//	Stuff a floating point value pointed at by Mp.
	//	Advances past float characters.
	void stuff_float(float* f);

	int stuff_float_optional(float* f, bool raw);

	//	Stuff an integer value pointed at by Mp.
	//	Advances past integer characters.
	void stuff_int(int* i);

	int stuff_int_optional(int* i, bool raw);


	// Stuffs an int value or the value of a number variable. Returns the index of the variable or
	// NOT_SET_BY_SEXP_VARIABLE.
	int stuff_int_or_variable(int& i, bool positive_value = false);

	// Stuff an integer value pointed at by Mp.If a variable is found instead stuff the value of that variable and
	// record the index of the variable in the following slot.
	int stuff_int_or_variable(int* ilp, int count, bool positive_value = false);

	// Stuffs boolean value.
	// Passes things off to stuff_boolean(bool)
	void stuff_boolean(int* i, bool a_to_eol = true);

	void stuff_boolean_flag(int* i, int flag, bool a_to_eol = true);

	// Stuffs a boolean value pointed at by Mp.
	// YES/NO (supporting 1/0 now as well)
	// Now supports localization :) -WMC

	void stuff_boolean(bool* b, bool a_to_eol = true);

	int stuff_bool_list(bool* blp, int max_bools);

	//	Stuff an integer value pointed at by Mp.
	//	Advances past integer characters.
	void stuff_ubyte(ubyte* i);

	int parse_string_flag_list(int* dest, flag_def_list defs[], int defs_size);

	int stuff_string_list(SCP_vector<SCP_string>& slp);

	// Stuffs a list of strings
	int stuff_string_list(char slp[][NAME_LENGTH], int max_strings);


	//	Stuffs an integer list.
	//	This is of the form ( i* )
	//	  where i is an integer.
	// For example, (1) () (1 2 3) ( 1 ) are legal integer lists.
	int stuff_int_list(int* ilp, int max_ints, int lookup_type);

	// helper for the next function. Removes a broken entry from ship or weapon lists and advances to the next one
	void clean_loadout_list_entry();

	// Karajorma - Stuffs an int list by parsing a list of ship or weapon choices.
	// Unlike stuff_int_list it can deal with variables and it also has better error reporting.
	int stuff_loadout_list(int* ilp, int max_ints, int lookup_type);

	// Stuffs an integer list like stuff_int_list.
	size_t stuff_float_list(float* flp, size_t max_floats);

	//	Marks an integer list.
	//	This is of the form ( i* )
	//	  where i is an integer.
	//	If a specified string is found in the lookup and its value is 7, then the 7th value
	//	in the array is set.
	void mark_int_list(int* ilp, int max_ints, int lookup_type);

	//	Stuff a vec3d struct, which is 3 floats.
	void stuff_vec3d(vec3d* vp);

	void stuff_parenthesized_vec3d(vec3d* vp);

	//	Stuffs vec3d list.  *vlp is an array of vec3ds.
	//	This is of the form ( (vec3d)* )
	//	  (where * is a kleene star, not a pointer indirection)
	// For example, ( (1 2 3) (2 3 4) (2 3 5) )
	//		 is a list of three vec3ds.
	int stuff_vec3d_list(vec3d* vlp, int max_vecs);

	// ditto the above, but a vector of vec3ds...
	int stuff_vec3d_list(SCP_vector<vec3d>& vec_list);

	//	Stuff a matrix, which is 3 vec3ds.
	void stuff_matrix(matrix* mp);



	//	Find a required string (*id), then stuff the text of type f_type that
	// follows it at *addr.  *strlist[] contains the strings it should try to
	// match.
	void
	find_and_stuff(const char* id, int* addr, int f_type, const char* strlist[], size_t max, const char* description);

	void find_and_stuff_optional(
		const char* id, int* addr, int f_type, const char* const* strlist, size_t max, const char* description);

	//	Mp points at a string.
	//	Find the string in the list of strings *strlist[].
	// Returns the index of the match, -1 if none.
	int match_and_stuff(int f_type, const char* const* strlist, int max, const char* description);

	void find_and_stuff_or_add(
		const char* id, int* addr, int f_type, char* strlist[], int* total, int max, const char* description);

*/

	~SCPParser() = default;
};