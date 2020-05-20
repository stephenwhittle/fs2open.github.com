#include "FSOutwndOutputDevice.h"
#include "cmdline/cmdline.h"
#include "osapi/outwnd.h"
#include "osapi/dialogs.h"

void FSOutwndOutputDevice::Init()
{
	outwnd_init();
	if (Cmdline_debug_window)
	outwnd_debug_window_init(); 
}

void FSOutwndOutputDevice::Close()
{	
	if (Cmdline_debug_window)
	outwnd_debug_window_deinit();
	outwnd_close();
}

void FSOutwndOutputDevice::PrintImpl(const char* ID, const char* Message)
{
	outwnd_print(ID, Message);
}

void FSOutwndOutputDevice::AssertImpl(const char* Expression, const char* File, int Line, const char* Message)
{
	os::dialogs::AssertMessage(Expression, File, Line, Message);
}

void FSOutwndOutputDevice::WarningImpl(const char* File, int Line, const char* Message)
{
	os::dialogs::Warning(File, Line, Message);
}

void FSOutwndOutputDevice::ErrorImpl(const char* FileOrMessage, int Line /*= -1*/, const char* FormattedMessage /*= nullptr*/)
{
	if (FormattedMessage == nullptr)
	{
		os::dialogs::Error(FileOrMessage);
	}
	else
	{
		os::dialogs::Error(FileOrMessage, Line, FormattedMessage);
	}
}

FSOutputDeviceBase* GOutputDevice = new FSOutwndOutputDevice();