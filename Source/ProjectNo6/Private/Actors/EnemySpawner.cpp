// Copy Right LDW. All Rights Reserved.


#include "Actors/EnemySpawner.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "GameInstanceCustom.h"
#include "../../Public/Characters/Enemies/CharacterEnemyBase.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);
	SpawnerTriggerComp = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnerTriggerComp"));
	SpawnerTriggerComp->SetupAttachment(RootSceneComp);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACharacterEnemyBase* AEnemySpawner::SpawnEnemy_Implementation()
{
	float TotalRatio = GetTotalRatioScaledWithDifficulty();
	float ShouldSpawnRatio = FMath::RandRange(0.0f, TotalRatio);
	float RatioCount = 0.0f;

	int32 CurrentFloorIndex = 0;
	int32 MaxFloorIndex = 0;
	if (UGameInstanceCustom* CustomGI = GetWorld()->GetGameInstance<UGameInstanceCustom>())
	{
		CurrentFloorIndex = CustomGI->FloorIndexCount;
		MaxFloorIndex = CustomGI->NumberOfFloors - 1;

		for (FEnemySpawnData& TempSpawnData : EnemySpawnDataArray)
		{
			RatioCount += FMath::GetMappedRangeValueClamped(FVector2D(0, MaxFloorIndex), TempSpawnData.SpawnRatioScaleWithDifficulty, CurrentFloorIndex);
			if (RatioCount >= ShouldSpawnRatio)
			{
				FVector SpawnLocation = GetSpawnLocation();
				float RandomYawValue = FMath::RandRange(-180.0f, 180.0f);
				FRotator SpawnRotation = FRotator(0.0f, RandomYawValue, 0.0f);
				FActorSpawnParameters SpawnParameters;
				AActor* NewSpawnedEnemyCharacter = GetWorld()->SpawnActor<ACharacterEnemyBase>(TempSpawnData.EnemyClass, SpawnLocation, SpawnRotation, SpawnParameters);
				return Cast<ACharacterEnemyBase>(NewSpawnedEnemyCharacter);
				break;
			}
		}
	}

	return nullptr;
}

float AEnemySpawner::GetTotalRatioScaledWithDifficulty()
{
	float TotalRatio = 0.0f;
	int32 CurrentFloorIndex = 0;
	int32 MaxFloorIndex = 0;
	if (UGameInstanceCustom* CustomGI = GetWorld()->GetGameInstance<UGameInstanceCustom>())
	{
		CurrentFloorIndex = CustomGI->FloorIndexCount;
		MaxFloorIndex = CustomGI->NumberOfFloors - 1;

		for (FEnemySpawnData& TempSpawnData : EnemySpawnDataArray)
		{
			TotalRatio += FMath::GetMappedRangeValueClamped(FVector2D(0, MaxFloorIndex), TempSpawnData.SpawnRatioScaleWithDifficulty, CurrentFloorIndex);
		}
	}
	return TotalRatio;
}

FVector AEnemySpawner::GetSpawnLocation_Implementation()
{
	return GetActorLocation() + 70.0f;
}
