// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CustomAssetManager.generated.h"

/**
 * Custom Asset Manager
 */
UCLASS()
class PROJECTNO6_API UCustomAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UCustomAssetManager& Get();

	/** Gets the pointed object of TSoftObjectPtr */
	template<class T>
	static FORCEINLINE T* GetObjectFromSoftPtr(TSoftObjectPtr<T> SoftPtr)
	{
		if (SoftPtr.IsValid() && SoftPtr.Get())
		{
			return SoftPtr.Get();
		}
		else
		{
			return SoftPtr.LoadSynchronous();
			// Or return LoadObject<T>(NULL, *SoftPtr.ToString(), NULL, LOAD_None, NULL);
		}
	}

	/** Gets the pointed class in TSubclassOf of TSoftClassPtr. Can change to return UClass* according to return type of TSoftClassPtr::LoadSynchronous */
	template<class T>
	static FORCEINLINE TSubclassOf<T> GetClassFromSoftPtr(TSoftClassPtr<T> SoftPtr)
	{
		if (SoftPtr.IsValid() && SoftPtr.Get())
		{
			return SoftPtr.Get();
		}
		else
		{
			return SoftPtr.LoadSynchronous();
			// Or return LoadClass<T>(NULL, *SoftPtr.ToString(), NULL, LOAD_None, NULL);
		}
	}
};
