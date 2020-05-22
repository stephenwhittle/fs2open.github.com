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
	void WarningImpl(const char* File, int Line, const char* Message) override;
	void ErrorImpl(const char* FileOrMessage, int Line = -1, const char* FormattedMessage = nullptr) override;
	void MessageImpl(const char* Message) override;

};
 