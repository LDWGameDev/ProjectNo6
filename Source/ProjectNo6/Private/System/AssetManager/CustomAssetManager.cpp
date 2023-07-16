// Copy Right LDW. All Rights Reserved.


#include "System/AssetManager/CustomAssetManager.h"

UCustomAssetManager& UCustomAssetManager::Get()
{
	if (UCustomAssetManager* CustomSingletonInstance = Cast<UCustomAssetManager>(GEngine->AssetManager))
	{
		return *CustomSingletonInstance;
	}
	else
	{
		// Should never call this
		return *NewObject<UCustomAssetManager>();
	}
}
