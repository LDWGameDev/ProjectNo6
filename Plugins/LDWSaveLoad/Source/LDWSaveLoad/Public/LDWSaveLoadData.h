// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/SaveGame.h"
#include "LDWSaveLoadData.generated.h"

/**
 * Custom SaveGame header similar to FSaveGameHeader in UGameplayStatics.cpp file
 * Create this one because cannot include the whole UGameplayStatics.cpp file to get FSaveGameHeader
 */
struct FLDWSaveGameHeader
{
public:
	FLDWSaveGameHeader() {};
	FLDWSaveGameHeader(TSubclassOf<USaveGame> SaveGameSubClass) : SaveGameClassNamePath(SaveGameSubClass->GetPathName()) {};

	void Empty() { SaveGameClassNamePath.Empty(); }

	void Read(FMemoryReader& MemoryReader)
	{
		Empty();
		MemoryReader << SaveGameClassNamePath;
	}
	void Write(FMemoryWriter& MemoryWriter) 
	{ 
		MemoryWriter << SaveGameClassNamePath;
	}

	FString SaveGameClassNamePath;
};



/**
 * 
 */
UCLASS()
class LDWSAVELOAD_API ULDWSaveLoadData : public UObject
{
	GENERATED_BODY()
	
};
