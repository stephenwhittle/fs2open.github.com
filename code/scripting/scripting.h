#ifndef _SCRIPTING_H
#define _SCRIPTING_H

#include "globalincs/globals.h"
#include "globalincs/pstypes.h"
#include "scripting/lua/LuaFunction.h"

#include <cstdio>

//**********Scripting languages that are possible
#define SC_LUA			(1<<0)

//*************************Scripting structs*************************
#define SCRIPT_END_LIST		NULL

struct image_desc
{
	char fname[MAX_FILENAME_LEN];
	int handle;
};

struct script_function {
	int language = 0;
	luacpp::LuaFunction function;
};

//-WMC
struct script_hook
{
	//Override
	script_function override_function;

	//Actual hook
	script_function hook_function;
};

extern bool script_hook_valid(script_hook *hook);

//**********Main Conditional Hook stuff

#define MAX_HOOK_CONDITIONS	8

//Conditionals
#define CHC_NONE			-1
#define CHC_MISSION			0
#define CHC_SHIP			1
#define CHC_SHIPCLASS		2
#define CHC_SHIPTYPE		3
#define CHC_STATE			4
#define CHC_CAMPAIGN		5
#define CHC_WEAPONCLASS		6
#define CHC_OBJECTTYPE		7
#define CHC_KEYPRESS		8
#define CHC_ACTION			9
#define CHC_VERSION			10
#define CHC_APPLICATION		11

//Actions
enum ConditionalActions {
	CHA_NONE            = -1,
	CHA_WARPOUT         = 0,
	CHA_WARPIN          = 1,
	CHA_DEATH           = 2,
	CHA_ONFRAME         = 3,
	CHA_COLLIDESHIP     = 4,
	CHA_COLLIDEWEAPON   = 5,
	CHA_COLLIDEDEBRIS   = 6,
	CHA_COLLIDEASTEROID = 7,
	CHA_HUDDRAW         = 8,
	CHA_OBJECTRENDER    = 9,
	CHA_SPLASHSCREEN    = 10,
	CHA_GAMEINIT        = 11,
	CHA_MISSIONSTART    = 12,
	CHA_MISSIONEND      = 13,
	CHA_MOUSEMOVED      = 14,
	CHA_MOUSEPRESSED    = 15,
	CHA_MOUSERELEASED   = 16,
	CHA_KEYPRESSED      = 17,
	CHA_KEYRELEASED     = 18,
	CHA_ONSTATESTART    = 19,
	CHA_ONSTATEEND      = 20,
	CHA_ONWEAPONDELETE  = 21,
	CHA_ONWPEQUIPPED    = 22,
	CHA_ONWPFIRED       = 23,
	CHA_ONWPSELECTED    = 24,
	CHA_ONWPDESELECTED  = 25,
	CHA_GAMEPLAYSTART   = 26,
	CHA_ONTURRETFIRED   = 27,
	CHA_PRIMARYFIRE     = 28,
	CHA_SECONDARYFIRE   = 29,
	CHA_ONSHIPARRIVE    = 30,
	CHA_COLLIDEBEAM     = 31,
	CHA_ONACTION        = 32,
	CHA_ONACTIONSTOPPED = 33,
	CHA_MSGRECEIVED     = 34,
	CHA_HUDMSGRECEIVED  = 35,
	CHA_AFTERBURNSTART  = 36,
	CHA_AFTERBURNEND    = 37,
	CHA_BEAMFIRE        = 38,
	CHA_SIMULATION      = 39,
	CHA_LOADSCREEN      = 40,
	CHA_ONWEAPONCREATE  = 41,
};

// management stuff
void scripting_state_init();
void scripting_state_close();
void scripting_state_do_frame(float frametime);

class script_condition
{
public:
	int condition_type;
	union
	{
		char name[CONDITION_LENGTH];
	} data;

	script_condition()
		: condition_type(CHC_NONE)
	{
		memset(data.name, 0, sizeof(data.name));
	}
};

class script_action
{
public:
	int action_type;
	script_hook hook;

	script_action()
		: action_type(CHA_NONE)
	{
	}
};

class ConditionedHook
{
private:
	SCP_vector<script_action> Actions;
	script_condition Conditions[MAX_HOOK_CONDITIONS];
public:
	bool AddCondition(script_condition *sc);
	bool AddAction(script_action *sa);

	bool ConditionsValid(ConditionalActions action, class object *objp=NULL, int more_data = 0);
	bool IsOverride(class script_state *sys, ConditionalActions action);
	bool Run(class script_state *sys, ConditionalActions action, char format='\0', void *data=NULL);
};

//**********Main script_state function
class script_state
{
private:
	char StateName[32];

	int Langs;
	struct lua_State *LuaState;
	const struct script_lua_lib_list *LuaLibs;

	//Utility variables
	SCP_vector<image_desc> ScriptImages;
	SCP_vector<ConditionedHook> ConditionalHooks;

private:

	void ParseChunkSub(script_function& out_func, const char* debug_str=NULL);
	int RunBytecodeSub(script_function& func, char format='\0', void *data=NULL);

	void SetLuaSession(struct lua_State *L);

	void OutputLuaMeta(FILE *fp);
	
	//Lua private helper functions
	bool OpenHookVarTable();
	bool CloseHookVarTable();

	//Internal Lua helper functions
	void EndLuaFrame();

	//Destroy everything
	void Clear();

public:
	//***Init/Deinit
	script_state(const char *name);

	script_state(const script_state&) = delete;
	script_state& operator=(script_state &i) = delete;

	~script_state();

	//***Internal scripting stuff
	int LoadBm(char *name);
	void UnloadImages();

	lua_State *GetLuaSession(){return LuaState;}

	//***Init functions for langs
	int CreateLuaState();

	//***Get data
	int OutputMeta(const char *filename);

	//***Moves data
	//void MoveData(script_state &in);

	//***Variable handling functions
	bool GetGlobal(const char *name, char format='\0', void *data=NULL);
	void RemGlobal(const char *name);

	void SetHookVar(const char *name, char format, const void *data=NULL);
	void SetHookObject(const char *name, object *objp);
	void SetHookObjects(int num, ...);
	bool GetHookVar(const char *name, char format='\0', void *data=NULL);
	void RemHookVar(const char *name);
	void RemHookVars(unsigned int num, ...);

	//***Hook creation functions
	bool EvalString(const char *string, const char *format=NULL, void *rtn=NULL, const char *debug_str=NULL);
	void ParseChunk(script_hook *dest, const char* debug_str=NULL);
	void ParseGlobalChunk(int hookType, const char* debug_str=NULL);
	bool ParseCondition(const char *filename="<Unknown>");

	//***Hook running functions
	int RunBytecode(script_hook &hd, char format='\0', void *data=NULL);
	bool IsOverride(script_hook &hd);
	int RunCondition(ConditionalActions condition, char format = '\0', void* data = NULL, class object* objp = NULL,
	                 int more_data = 0);
	bool IsConditionOverride(ConditionalActions action, object *objp=NULL);

	//*****Other functions
	void EndFrame();
};


//**********Script registration functions
void script_init();

//**********Script globals
extern class script_state Script_system;
extern bool Output_scripting_meta;

//*************************Conditional scripting*************************

#endif //_SCRIPTING_H
