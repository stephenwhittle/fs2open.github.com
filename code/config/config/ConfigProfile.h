#pragma once
#include "FSOutputDeviceBase.h"
#include "FSStdTypes.h"
#include "filesystem/SCPPath.h"
#include "iniparser.hpp"
#include "tl/optional.hpp"
namespace SCP
{
	class ConfigProfile
	{
	public:
		ConfigProfile(SCPPath ConfigProfilePath) : FilePath(ConfigProfilePath)
		{
			if (!UnderlyingFile.Load(ConfigProfilePath.string()))
			{
				if (UnderlyingFile.LastResult().error_code == INI_ERR_INVALID_FILENAME)
				{
					GOutputDevice->Message("Config profile at %s missing. Will use defaults\r\n",
										   ConfigProfilePath.string());
				}
			}
		}

		template<typename ValueType>
		void WriteConfigValue(SCP_string SectionName, SCP_string KeyName, ValueType KeyValue)
		{
			UnderlyingFile.GetSection(SectionName)->SetValue(KeyName, KeyValue);
			Commit();
		}

		template<typename ValueType>
		tl::optional<ValueType> ReadConfigValue(SCP_string SectionName, SCP_string KeyName)
		{
			INI::Section* CurrentSection = UnderlyingFile.GetSection(SectionName);
			if (CurrentSection)
			{
				for (auto CurrentKey : CurrentSection->GetSectionKeys())
				{
					if (CurrentKey == KeyName)
					{
						return CurrentSection->GetValue(KeyName).AsT<ValueType>();
					}
				}
			}
			return {};
		}
		void Commit()
		{
			UnderlyingFile.Save(FilePath);
		}
	private:
		INI::File UnderlyingFile;
		SCPPath FilePath;
	};
} // namespace SCP