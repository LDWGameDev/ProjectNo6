// Copy Right LDW. All Rights Reserved.


#include "System/Settings/CustomGameUserSettings.h"

UCustomGameUserSettings::UCustomGameUserSettings()
{

}

UCustomGameUserSettings* UCustomGameUserSettings::Get()
{
	if (GEngine)
	{
		return Cast<UCustomGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
}
