#pragma once
//TODO: @SCPApplication refactor FredRunning into some other kind of thing, preferably a global function or static member function for SCPApp?
extern int Fred_running;  // Is Fred running, or FreeSpace?
extern int Lcl_current_lang;
class SCPApplication
{
    private:
    
    public:
    int GetCurrentLanguage();
};
