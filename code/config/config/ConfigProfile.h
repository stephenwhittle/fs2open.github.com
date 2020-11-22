#pragma once
#include "FSOutputDeviceBase.h"
#include "iniparser.hpp"
#include "filesystem/SCPPath.h"

namespace SCP
{
	class ConfigProfile
	{
	public:
		ConfigProfile(SCPPath ConfigProfilePath)
		{
			if (!UnderlyingFile.Load(ConfigProfilePath.string()))
			{
				switch (UnderlyingFile.LastResult().error_code)
				{
					case INI_ERR_PARSING_ERROR:
						GOutputDevice->Message("Parse error when loading config profile at {}. Will use safe defaults",
											   ConfigProfilePath.string());
						break;
					case INI_ERR_INVALID_FILENAME:
						GOutputDevice->Message("Config profile at {} missing. Will use safe defaults",
											   ConfigProfilePath.string());
						break;
					case 0: //No error
						break;
					default:
						GOutputDevice->Message("Unknown error when loading config profile at {}. Will use safe defaults",
											   ConfigProfilePath.string());
				}
			}
		}

		template<typename ValueType>
		void WriteConfigValue(class SCP_string SectionName, SCP_string KeyName, ValueType KeyValue)
		{
			UnderlyingFile.GetSection(SectionName)->SetValue(KeyName, KeyValue);
		}

		template<typename ValueType>
		tl::optional<ValueType> ReadConfigValue(SCP_string SectionName, SCP_string KeyName)
		{
			UnderlyingFile.GetSection(SectionName).GetValue(KeyName).AsT<ValueType>();
		}

	private:
		INI::File UnderlyingFile;
	};
} // namespace SCP