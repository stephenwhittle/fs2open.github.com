#pragma once

#include "FSOutputDeviceBase.h"

class FSOutwndOutputDevice : public FSOutputDeviceBase
{
public:
    virtual void Init() override;
	virtual void Close() override;
protected:
	void PrintImpl(const char* ID, const char* Message) override;
	void AssertImpl(const char* Expression, const char* File, int Line, const char* Message) override;

};
 