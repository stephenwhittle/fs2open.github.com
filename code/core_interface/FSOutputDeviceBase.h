#pragma once

#include "fmt/printf.h"

class FSOutputDeviceBase
{
protected:
	virtual void PrintImpl(const char* ID, const char* Message) = 0;
	virtual void AssertImpl(const char* Expression, const char* File, int Line, const char* Message) = 0;
	virtual void WarningImpl(const char* File, int Line, const char* Message) = 0;
	virtual void ErrorImpl(const char* FileOrMessage, int Line = -1, const char* FormattedMessage = nullptr) = 0;
	virtual void MessageImpl(const char* Message) = 0;
public:

	virtual void Init() = 0;
	virtual void Close() = 0;

	template<typename... Args>
		void Print(const char* ID, const char* Format, const Args& ... args)
	{
		std::string Message = fmt::sprintf(Format, args...);
		PrintImpl(ID, Message.c_str());
	}
	 
	template<typename... Args>
		void PrintGeneral(const char* Format, const Args& ... args)
	{
		std::string Message = fmt::sprintf(Format, args...);
		PrintImpl(nullptr, Message.c_str());
	}
	
	template<typename... Args>
	void AssertMessage(const char* Expression, const char* File, int Line, const char* Format = nullptr, const Args& ... args)
	{
		if (Format != nullptr)
		{
			std::string Message = fmt::sprintf(Format, args...);
			AssertImpl(Expression, File, Line, Message.c_str());
		}
		else
		{
			AssertImpl(Expression, File, Line, nullptr);
		}
	}

	template<typename... Args>
	void Warning(const char* File, int Line, const char* Format, const Args& ... args)
	{
		std::string Message = fmt::sprintf(Format, args...);
		WarningImpl(File, Line, Message.c_str());
	}

	template<typename... Args>
	void Error(const char* FileOrMessage, int Line = -1, const char* Format = nullptr, const Args& ... args)
	{
		if (Format == nullptr)
		{
			ErrorImpl(FileOrMessage);
		}
		else
		{

			std::string Message = fmt::sprintf(Format, args...);
			ErrorImpl(FileOrMessage, Line, Message.c_str());
		}
	}

	template<typename... Args>
	void Message(const char* Format, const Args& ... args)
	{
		std::string Message = fmt::sprintf(Format, args...);
		MessageImpl(Message.c_str());
	}
};

extern FSOutputDeviceBase* GOutputDevice;