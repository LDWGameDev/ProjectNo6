// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomData.h"
#include "GameInstanceCustom.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCompletedFloorObjectiveSignature, int32, ObjectiveIndex, int32, FloorIndex, EFloorObjectiveType, CompletedObjective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloorCompletedSignature, int32, CurrentFloor);

class ULevelStreamingDynamic;
class ACharacterEnemyBase;
class UBaseUpgrade;

/**
 * 
 */
UCLASS()
class PROJECTNO6_API UGameInstanceCustom : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Game Instance")
		TArray<FCustomSound> CustomSoundArray;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Game Instance : Run")
		TArray<FTemple> TemplesArray;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Game Instance : Run")
		FVector LevelStreamingLocation1 = FVector(-10000.0f, 0.0f, 0.0f);
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Game Instance : Run")
		FVector LevelStreamingLocation2 = FVector(10000.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		int32 CurrentTempleIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		int32 NumberOfFloors;
	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		int32 FloorIndexCount;

	UPROPERTY(BlueprintAssignable)
		FOnFloorCompletedSignature OnPreCompletedFloorDelegate;
	UPROPERTY(BlueprintAssignable)
		FOnFloorCompletedSignature OnPostCompletedFloorDelegate;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EnterBossFloor();
	virtual void EnterBossFloor_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CompleteCurrentFloor();
	virtual void CompleteCurrentFloor_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnPreCompletedFloor(int32 CompletedFloorIndex);
	void OnPreCompletedFloor_Implementation(int32 CompletedFloorIndex);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnPostCompletedFloor(int32 NextFloorIndex);
	void OnPostCompletedFloor_Implementation(int32 NextFloorIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartRunWithCurrentFloorIndex();
	void StartRunWithCurrentFloorIndex_Implementation();

	UFUNCTION(BlueprintCallable)
		void GenerateFloorIndexes();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Upgrade")
		TArray<UBaseUpgrade*> UpgradesArrayTypeOne;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Upgrade")
		TArray<UBaseUpgrade*> UpgradesArrayTypeTwo;

	UFUNCTION(BlueprintPure)
		TArray<UBaseUpgrade*> GetRandomUpgrades();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ApplyUpgrade(UBaseUpgrade* UpgradeToApply);
	virtual void ApplyUpgrade_Implementation(UBaseUpgrade* UpgradeToApply);

public:
	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		int32 CurrentFloorIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		int32 CurrentObjectiveIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		int32 HoveredObjectiveIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Custom Game Instance : Run")
		TArray<FFloor> Floors;
		
	UPROPERTY(BlueprintReadOnly, Category = "Custom Game Instance : Run")
		FCompletedFloorObjectiveSignature OnCompletedFloorObjectiveDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "Custom Game Instance : Run")
		bool bCanGenerateObjectives = true;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Custom Game Instance: Run")
		ULevelStreamingDynamic* CurrentActiveLevelStreaming;
	UPROPERTY(BlueprintReadOnly, Category = "Custom Game Instance: Run")
		ULevelStreamingDynamic* LastActiveLevelStreaming;

public:
	UGameInstanceCustom();
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Custom Game Instance")
		void CustomSpawnSoundAtLocation(const FString& SoundId, const FVector& Location);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Game Instance")
		void ApplyAudioSettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Game Instance")
		void ApplyGraphicsSettings(); 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Game Instance")
		void GenerateFloorObjectives();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Game Instance")
		void StartRunWithCurrentIndexes();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Game Instance")
		void ResetCurrentFloorIndex();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Custom Game Instance")
		bool CompleteCurrentObjective();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Custom Game Instance")
		EFloorObjectiveType GetCurrentObjectiveType();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Custom Game Instance")
		void GetParentObjectivesIndexes(TArray<int32>& OutParentIndexes, int32 ObjectiveIndex, int32 FloorIndex);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Sound Component")
		void InitSoundMix();

	UFUNCTION(BlueprintNativeEvent, Category = "Sound Component")
		void LoadSettings();

	UFUNCTION(BlueprintPure, Category = "Sound Component")
		bool AreCurrentObjectiveAndFloorIndexesValid();
	UFUNCTION(BlueprintPure, Category = "Sound Component")
		bool AreObjectiveAndFloorIndexesValid(int32 ObjectiveIndex, int32 FloorIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom Game Instance")
		void HandleCompletedObjective(int32 ObjectiveIndex, int32 FloorIndex, EFloorObjectiveType ObjectiveType);

	int32 ProduceRandomNumberOfObjectives(float ThreeObjectivesRate = 70.0f);
	EFloorObjectiveType ProduceRandomObjectiveType();
	bool IsFloorObjectiveNotEmpty(int32 FloorIndex, int32 ObjectiveIndex);
	int32 GetRandomInLinkableIndexes(int32 Index);

	bool IsEmptyAndHasValidChildObjective(FFloor& Floor, int32 FloorIndex, int32 ObjectiveIndex);
	bool IsValidIndexAndNotEmptyObjective(int32 FloorIndex, int32 ObjectiveIndex);
	int32 GetNumberOfNotEmptyObjectivesOfFloor(int32 FloorIndex);
	int32 GetNumberOfNotEmptyLinkableChildObjectives(int32 FloorIndexOfObjective, int32 ObjectiveIndexToCheck);
	int32 GetNumberOfNotEmptyLinkableParentObjectives(int32 FloorIndexOfObjective, int32 ObjectiveIndexToCheck);

	UFUNCTION(BlueprintNativeEvent)
		void HandleOnCurrentLevelStreamingShown();
	virtual void HandleOnCurrentLevelStreamingShown_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void OnLoadedNewFloor();

	UFUNCTION()
		APlayerStart* GetClosetPlayerStartToCurrentActiveLevelStream();

public:
	UFUNCTION(BlueprintNativeEvent)
		void OnEnemyDied(ACharacterEnemyBase* DeathEnemy);
	virtual void OnEnemyDied_Implementation(ACharacterEnemyBase* DeathEnemy);

public:
	UFUNCTION(BlueprintCallable)
		void SetTimeDilationForDuration(float TimeDilation = 1.0f, float Duration = 1.0f);
private:
	FTimerHandle TimeDilationHandle;
};
