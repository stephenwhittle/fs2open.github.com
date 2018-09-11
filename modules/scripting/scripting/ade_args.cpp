
#include "scripting/ade_args.h"
#include "scripting/ade.h"

#include "scripting/lua/LuaTable.h"
#include "scripting/lua/LuaFunction.h"
#include "scripting/lua/LuaConvert.h"

#include "mod_table/mod_table.h"

#include <utf8.h>

namespace {
using namespace scripting;

ade_table_entry& getTableEntry(size_t idx) {
	return ade_manager::getInstance()->getEntry(idx);
}
}

namespace scripting {

//WMC - hack to skip X number of arguments on the stack
//Lets me use ade_get_args for global hook return values
int Ade_get_args_skip = 0;
bool Ade_get_args_lfunction = false;

//ade_get_args(state, arguments, variables)
//----------------------------------------------
//based on "Programming in Lua"
//
//Parses arguments from string to variables given
//a '|' divides required and optional arguments.
//Returns 0 if a required argument is invalid,
//or there are too few arguments actually passed
//
//NOTE: This function essentially takes objects
//from the stack in series, so it can easily be used
//to get the return values from a chunk of Lua code
//after it has been executed. See RunByteCode()
int ade_get_args(lua_State *L, const char *fmt, ...)
{
	//Check that we have all the arguments that we need
	//If we don't, return 0
	int needed_args = (int)strlen(fmt);
	int total_args = lua_gettop(L) - Ade_get_args_skip;

	if(strchr(fmt, '|') != NULL) {
		needed_args = (int)(strchr(fmt, '|') - fmt);
	}

	char funcname[128] = "\0";
#ifndef NDEBUG
	lua_Debug ar;
	memset(&ar, 0, sizeof(ar));
	if(lua_getstack(L, 0, &ar))
	{
		lua_getinfo(L, "nl", &ar);
		strcpy_s(funcname, "");
		if(ar.name != NULL) {
			strcat_s(funcname, ar.name);
		}
		if(ar.currentline > -1) {
			char buf[33];
			sprintf(buf, "%d", ar.currentline);
			strcat_s(funcname, " (Line ");
			strcat_s(funcname, buf);
			strcat_s(funcname, ")");
		}
	}
#endif
	if(!strlen(funcname)) {
		//WMC - This was causing crashes with user-defined functions.
		//WMC - Try and get at function name from upvalue
		if(!Ade_get_args_lfunction && !lua_isnone(L, lua_upvalueindex(ADE_FUNCNAME_UPVALUE_INDEX)))
		{
			if(lua_type(L, lua_upvalueindex(ADE_FUNCNAME_UPVALUE_INDEX)) == LUA_TSTRING)
				strcpy_s(funcname, lua_tostring(L, lua_upvalueindex(ADE_FUNCNAME_UPVALUE_INDEX)));
		}

		//WMC - Totally unknown function
		if(!strlen(funcname)) {
			strcpy_s(funcname, "<UNKNOWN>");
		}
	}
	if(total_args < needed_args) {
		LuaError(L, "Not enough arguments for '%s' - need %d, had %d. If you are using objects or handles, make sure that you are using \":\" to access member functions, rather than \".\"", funcname, needed_args, total_args);
		return 0;
	}

	//Start throught
	va_list vl;
	int nargs;
	int counted_args = 0;

	//Are we parsing optional args yet?
	bool optional_args = false;

	va_start(vl, fmt);
	nargs = 1 + Ade_get_args_skip;
	total_args += Ade_get_args_skip;
	while(*fmt && nargs <= total_args)
	{
		switch(*fmt++)
		{
			case 'b':
				if(lua_isboolean(L, nargs)) {
					*va_arg(vl, bool*) = lua_toboolean(L, nargs) > 0 ? true : false;
				} else {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; boolean expected", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			case 'd':
				if(lua_isnumber(L, nargs)) {
					*va_arg(vl, double*) = (double)lua_tonumber(L, nargs);
				} else {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; number expected", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			case 'f':
				if(lua_isnumber(L, nargs)) {
					*va_arg(vl, float*) = (float)lua_tonumber(L, nargs);
				} else {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; number expected", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			case 'i':
				if(lua_isnumber(L, nargs)) {
					*va_arg(vl, int*) = (int)lua_tonumber(L, nargs);
				} else {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; number expected", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			case 's':
				if(lua_isstring(L, nargs)) {
					auto value = lua_tostring(L, nargs);

					if (Unicode_text_mode) {
						// Validate the string when we are in unicode mode to ensure that FSO doesn't just crash when a script
						// passes an invalid UTF-8 sequence to the API
						auto end = value + strlen(value);
						auto invalid = utf8::find_invalid(value, end);

						if (invalid != end) {
							if (invalid == value) {
								// The first character is the invalid sequence
								LuaError(L, "An invalid UTF-8 encoding sequence was detected! The first invalid character was as the start of the string.");
							} else {
								// If the invalid sequence is inside the string then we try to give some context to make
								// finding the bug easier
								auto display_text_start = std::max(value, invalid - 32);
								SCP_string context_text(display_text_start, invalid);

								LuaError(L,
										 "An invalid UTF-8 encoding sequence was detected! The error was detected directly after this string \"%s\".",
										 context_text.c_str());
							}

							// Finally, assign a default value so that we can continue
							value = "Invalid UTF-8 sequence detected!";
						}
					}

					*va_arg(vl, const char **) = value;
				} else {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; string expected", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			case 'x':
				if(lua_isnumber(L, nargs)) {
					*va_arg(vl, fix*) = fl2f((float)lua_tonumber(L, nargs));
				} else {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; number expected", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			case 'o':
			{
				ade_odata od = va_arg(vl, ade_odata);
				if(lua_isuserdata(L, nargs))
				{
					// Use the helper function
					if (!luacpp::convert::popValue(L, od, nargs, false)) {
						if (!optional_args) {
							va_end(vl);
							return 0;
						}
					}
				}
				else if(lua_isnil(L, nargs) && optional_args)
				{
					//WMC - Modder has chosen to ignore this argument
				}
				else
				{
					LuaError(L, "%s: Argument %d is an invalid type '%s'; type '%s' expected", funcname, nargs, ade_get_type_string(L, nargs), getTableEntry(od.idx).GetName());
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
			}
				break;
			case 't':
			{
				// Get a table
				auto target = va_arg(vl, luacpp::LuaTable*);
				if (!luacpp::convert::popValue(L, *target, nargs, false)) {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; table expected.", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				break;
			}
			case 'u':
			{
				// Get a function
				auto target = va_arg(vl, luacpp::LuaFunction*);
				if (!luacpp::convert::popValue(L, *target, nargs, false)) {
					LuaError(L, "%s: Argument %d is an invalid type '%s'; function expected.", funcname, nargs, ade_get_type_string(L, nargs));
					if(!optional_args) {
						va_end(vl);
						return 0;
					}
				}
				// Automatically assign our error function to function values retrieved from the API
				target->setErrorFunction(luacpp::LuaFunction::createFromCFunction(L, ade_friendly_error));
				break;
			}
			case '|':
				nargs--;	//cancel out the nargs++ at the end
				counted_args--;
				optional_args = true;
				break;
			case '*':
				//WMC - Ignore one spot
				break;
			default:
				Error(LOCATION, "%s: Bad character passed to ade_get_args; (%c)", funcname, *(fmt-1));
				break;
		}
		nargs++;
		counted_args++;
	}
	va_end(vl);
	return counted_args;
}

//ade_set_args(state, arguments, variables)
//----------------------------------------------
//based on "Programming in Lua"
//
//Takes variables given and pushes them onto the
//Lua stack. Use it to return variables from a
//Lua scripting function.
//
//NOTE: You can also use this to push arguments
//on to the stack in series. See script_state::SetHookVar
int ade_set_args(lua_State *L, const char *fmt, ...)
{
	//Start throught
	va_list vl;
	int nargs;
	int setargs;	//args actually set

	va_start(vl, fmt);
	nargs = 0;
	setargs = 0;
	while(*fmt != '\0')
	{
		switch(*fmt++)
		{
			case '*':
				lua_pushnil(L);
				break;
			case 'b':	//WMC - Bool is actually int for GCC (Why...?)
				lua_pushboolean(L, va_arg(vl, int) ? 1 : 0);
				break;
			case 'd':
				lua_pushnumber(L, va_arg(vl, double));
				break;
			case 'f':
				lua_pushnumber(L, va_arg(vl, double));
				break;
			case 'i':
				lua_pushnumber(L, va_arg(vl, int));
				break;
			case 's':
			{
				//WMC - Isn't working with HookVar for some strange reason
				char *s = va_arg(vl, char*);
				lua_pushstring(L, s);
				break;
			}
			case 'x':
				lua_pushnumber(L, f2fl(va_arg(vl, fix)));
				break;
			case 'o':
			{
				//Copy over objectdata
				ade_odata od = (ade_odata) va_arg(vl, ade_odata);

				// Use the common helper method
				luacpp::convert::pushValue(L, od);
				break;
			}
			case 't':
			{
				// Set a table
				luacpp::LuaTable* table = va_arg(vl, luacpp::LuaTable*); // This must be a pointer since C++ classes can't be passed by vararg
				table->pushValue();
				break;
			}
			case 'u':
			{
				// Set a function
				luacpp::LuaFunction* func = va_arg(vl, luacpp::LuaFunction*); // This must be a pointer since C++ classes can't be passed by vararg
				func->pushValue();
				break;
			}
				//WMC -  Don't forget to update lua_set_arg
			default:
				Error(LOCATION, "Bad character passed to ade_set_args; (%c)", *(fmt-1));
				setargs--;
		}
		nargs++;
		setargs++;
	}
	va_end(vl);
	return setargs;
}

}
