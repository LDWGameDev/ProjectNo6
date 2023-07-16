// Fill out your copyright notice in the Description page of Project Settings.


#include "LDWSaveLoadGISS.h"
#include "Engine/GameInstance.h"
#include "HAL/FileManager.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "GameFramework/SaveGame.h"
#include "Serialization/BufferArchive.h"
#include "SaveGameSystem.h"
#include "PlatformFeatures.h"

DEFINE_LOG_CATEGORY(LogLDWSaveLoad);

ULDWSaveLoadGISS::ULDWSaveLoadGISS()
{

}

void ULDWSaveLoadGISS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentSaveSlotName = cDefaultSaveSlotName;
}

ULDWSaveLoadGISS* ULDWSaveLoadGISS::Get(UObject* WorldContextObject)
{
	if (GEngine != nullptr)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				return GameInstance->GetSubsystem<ULDWSaveLoadGISS>();
			}
		}
	}
	return nullptr;
}

FString ULDWSaveLoadGISS::GetSaveGameFullPath(const FString& SaveSlotName, const FString& SaveGameName)
{
	return (FPaths::ProjectSavedDir() + TEXT("SaveGames/") + SaveSlotName + TEXT("/") + SaveGameName + TEXT(".sav"));
}

bool ULDWSaveLoadGISS::CheckSaveSlotExist(const FString SaveSlotName)
{
	FString SaveSlotFullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/") + SaveSlotName;
	return (IFileManager::Get().DirectoryExists(*SaveSlotFullPath));
}

bool ULDWSaveLoadGISS::CheckOrCreateSaveSlot(const FString& SaveSlotName)
{
	FString SaveSlotDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/") + SaveSlotName;
	if (IFileManager::Get().DirectoryExists(*SaveSlotDirectory))
	{
		UE_LOG(LogLDWSaveLoad, Log, TEXT("Save slot existed at %s"), *SaveSlotDirectory);
		return true;
	}
	else
	{
		bool CreateDirectoryResult = IFileManager::Get().MakeDirectory(*SaveSlotDirectory, true);
		if (CreateDirectoryResult)
		{
			UE_LOG(LogLDWSaveLoad, Log, TEXT("Save slot created at %s"), *SaveSlotDirectory);
			return true;
		}
	}
	UE_LOG(LogLDWSaveLoad, Log, TEXT("Cannot check or create save slot at %s"), *SaveSlotDirectory);
	return false;
}

bool ULDWSaveLoadGISS::CheckSaveGameExist(const FString& SaveSlotName, const FString& SaveGameName)
{
	return (IFileManager::Get().FileSize(*GetSaveGameFullPath(SaveSlotName, SaveGameName)) > 0);
}

bool ULDWSaveLoadGISS::SaveSaveGameToSlot(USaveGame* SaveGameToSave, const FString& SaveSlotName, const FString& SaveGameName)
{
	bool Result = false;
	FString FullSaveGamePath = GetSaveGameFullPath(SaveSlotName, SaveGameName);

	// Check for SaveGame and SaveSlot folder
	if (SaveGameToSave != nullptr && CheckOrCreateSaveSlot(SaveSlotName))
	{
		TArray<uint8> ObjectBytes;

		// Create archive object to store data
		FMemoryWriter MemoryWriter(ObjectBytes,true);

		// Create and write save game header 
		FLDWSaveGameHeader SaveGameHeader(SaveGameToSave->GetClass());
		SaveGameHeader.Write(MemoryWriter);

		// Create string proxy archive, save SaveGame, replacing UObjects and FNames with strings.
		FObjectAndNameAsStringProxyArchive StringProxyArchive(MemoryWriter, false);
		SaveGameToSave->Serialize(StringProxyArchive);

		Result = FFileHelper::SaveArrayToFile(ObjectBytes, *FullSaveGamePath);
	}

	if (Result)
	{
		UE_LOG(LogLDWSaveLoad, Log, TEXT("Succeeded save save game at %s"), *FullSaveGamePath);
	}
	else
	{
		UE_LOG(LogLDWSaveLoad, Log, TEXT("Failed save save game at %s"), *FullSaveGamePath);
	}
	return Result;
} 

USaveGame* ULDWSaveLoadGISS::LoadSaveGameFromSlot(const FString& SaveSlotName, const FString& SaveGameName)
{
	if (!CheckSaveGameExist(SaveSlotName, SaveGameName)) return nullptr;
	FString SaveGamePath = GetSaveGameFullPath(SaveSlotName, SaveGameName);
	TArray<uint8> ObjectBytes;
	bool Result = FFileHelper::LoadFileToArray(ObjectBytes, *SaveGamePath);
	
	if (!Result || ObjectBytes.Num() == 0) return nullptr;
	USaveGame* OutSaveGame = nullptr;
	// Create archive object to read data
	FMemoryReader MemoryReader(ObjectBytes, true);
	
	// Create and read save game header
	FLDWSaveGameHeader SaveGameHeader;
	SaveGameHeader.Read(MemoryReader);

	// Try to find or load class from class path stored in save game header
	UClass* SaveGameClass = FindObject<UClass>(ANY_PACKAGE, *SaveGameHeader.SaveGameClassNamePath);
	if (SaveGameClass == nullptr)
	{
		SaveGameClass = LoadObject<UClass>(nullptr, *SaveGameHeader.SaveGameClassNamePath);
	}

	// Create SaveGame object if found SaveGame class
	if (SaveGameClass != nullptr)
	{
		OutSaveGame = NewObject<USaveGame>(GetTransientPackage(), SaveGameClass);
		FObjectAndNameAsStringProxyArchive StringProxyArchive(MemoryReader, true);
		OutSaveGame->Serialize(StringProxyArchive);
	}

	if (OutSaveGame != nullptr)
	{
		UE_LOG(LogLDWSaveLoad, Log, TEXT("Succeeded load save game at %s"), *SaveGamePath);
	}
	else
	{
		UE_LOG(LogLDWSaveLoad, Log, TEXT("Failed load save game at %s"), *SaveGamePath);
	}
	return OutSaveGame;
}

bool ULDWSaveLoadGISS::SaveSaveGameToCurrentSlot(USaveGame* SaveGameToSave, const FString& SaveGameName)
{
	return SaveSaveGameToSlot(SaveGameToSave, *GetCurrentSaveSlotName(), SaveGameName);
}

USaveGame* ULDWSaveLoadGISS::LoadSaveGameFromCurrentSlot(const FString& SaveGameName)
{
	return LoadSaveGameFromSlot(*GetCurrentSaveSlotName(), SaveGameName);
}
