// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomData.h"
#include "AreaControl.generated.h"

class ACharacterEnemyBase;
class USceneComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAreaControlCompleted, AAreaControl*, CompletedAreaControl);

UCLASS()
class PROJECTNO6_API AAreaControl : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USceneComponent* RootSceneComp;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UBoxComponent* AreaBound;
public:
	AAreaControl();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		FString LevelId;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		EAreaControlType AreaControlType;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		FVector2D WavesNumberRange = FVector2D(1.0f, 3.0f);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		FVector2D TrapNumberScaleWithDifficulty = FVector2D(0.6f, 1.0f);
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		FVector2D EnemySpawnerNumberScaledWithDifficulty = FVector2D(0.6f, 1.0f);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		TMap<EAreaControlType, int32> ControlTypeRatioMap;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Area Control")
		float TimeToSpawnNextWave = 2.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Area Control")
		bool bHasStartedControllingArea = false;
	UPROPERTY(BlueprintReadOnly, Category = "Area Control")
		bool bIsCompleted = false;

	UPROPERTY(BlueprintAssignable)
		FOnAreaControlCompleted OnAreaControlCompletedDelegate;

	int32 NumberOfWaves;
	int32 WaveCount;
	TArray<ACharacterEnemyBase*> SpawnedEnemiesArray;

protected:
	int32 GetTotalPercentValue();
	void SpawnWaveOfEnemies();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartControllingArea();
	virtual void StartControllingArea_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void CompleteControllingArea();
	virtual void CompleteControllingArea_Implementation();

protected:
	UFUNCTION()
		void HandleDelegate_OnEnemyDied(ACharacterEnemyBase* DeathEnemy);

	bool AreSpawnedEnemiesAllDeath();

	UFUNCTION(BlueprintNativeEvent)
		void HandleOnBeginOverlap_AreaBound(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void HandleOnBeginOverlap_AreaBound_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
