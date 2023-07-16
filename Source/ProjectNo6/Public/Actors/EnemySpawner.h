// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomData.h"
#include "EnemySpawner.generated.h"

class ACharacterEnemyBase;
class USceneComponent;
class USphereComponent;

UCLASS()
class PROJECTNO6_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FEnemySpawnData> EnemySpawnDataArray;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USceneComponent* RootSceneComp;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USphereComponent* SpawnerTriggerComp;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		ACharacterEnemyBase* SpawnEnemy();
	ACharacterEnemyBase* SpawnEnemy_Implementation();

protected:
	UFUNCTION(BlueprintCallable)
		float GetTotalRatioScaledWithDifficulty();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FVector GetSpawnLocation();
	virtual FVector GetSpawnLocation_Implementation(); 
};
