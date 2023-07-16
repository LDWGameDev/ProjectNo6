// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LDWSaveLoadData.h"
#include "LDWSaveLoadGISS.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLDWSaveLoad, Log, All);

class USaveGame;

/**
 * SaveGame: .sav files stored in SaveSlot folder
 * SaveSlot: folders in /Saved/SaveGames/...(SaveSlot)s, contain SaveGame files
 */
UCLASS()
class LDWSAVELOAD_API ULDWSaveLoadGISS : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	const FString cDefaultSaveSlotName = TEXT("DefaultSaveSlot");
	const FString cSaveSlotInfoSaveGameName = TEXT("SaveSlotInfo");
	FString CurrentSaveSlotName;

public:
	ULDWSaveLoadGISS();

	/** UGameInstanceSubsystem override functions */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Returns the global instance of this */
	static ULDWSaveLoadGISS* Get(UObject* WorldContextObject);

	/** Getters, setters */
	UFUNCTION(BlueprintPure, Category = "LDW Save Load")
		FString GetCurrentSaveSlotName() { return CurrentSaveSlotName.IsEmpty() ? cDefaultSaveSlotName : CurrentSaveSlotName; }
	UFUNCTION(BlueprintPure, Category = "LDW Save Load")
		FString GetSaveGameFullPath(const FString& SaveSlotName, const FString& SaveGameName);
	UFUNCTION(BlueprintCallable, Category = "LDW Save Load")
		void SetCurrentSaveSlotName(const FString& NewSaveSlotName) { CurrentSaveSlotName = NewSaveSlotName; }


	/** Returns a boolean value indicates if a SaveSlot exists in Saved/SaveGames/ directory, if not creates one and returns the creation result */
	UFUNCTION(BlueprintCallable, Category = "LDW Save Load")
		bool CheckOrCreateSaveSlot(const FString& SaveSlotName);
	/** Returns a boolean value indicates if a SaveSlot exists in Saved/SaveGames/ directory */
	UFUNCTION(BlueprintPure, Category = "LDW Save Load")
		bool CheckSaveSlotExist(const FString SaveSlotName);
	/** Returns a boolean value indicates if a SaveGame exists in SaveSlot */
	UFUNCTION(BlueprintPure, Category = "LDW Save Load")
		bool CheckSaveGameExist(const FString& SaveSlotName, const FString& SaveGameName);


	/** Saves a SaveGame to SaveSlot and returns the result */
	UFUNCTION(BlueprintCallable, Category = "LDW Save Load")
		bool SaveSaveGameToSlot(USaveGame* SaveGameToSave, const FString& SaveSlotName, const FString& SaveGameName);
	/** Loads a SaveGame to slot and returns the loaded SaveGame */
	UFUNCTION(BlueprintCallable, Category = "LDW Save Load")
		USaveGame* LoadSaveGameFromSlot(const FString& SaveSlotName, const FString& SaveGameName);

	/** Saves a SaveGame to current save slot and returns the result */
	UFUNCTION(BlueprintCallable, Category = "LDW Save Load")
		bool SaveSaveGameToCurrentSlot(USaveGame* SaveGameToSave, const FString& SaveGameName);
	/** Loads a SaveGame from current save slot and return the loaded SaveGame */
	UFUNCTION(BlueprintCallable, Category = "LDW Save Load")
		USaveGame* LoadSaveGameFromCurrentSlot(const FString& SaveGameName);
};
