// Copy Right LDW. All Rights Reserved.


#include "Actors/AreaControl.h"
#include "Components/BoxComponent.h"
#include "GameInstanceCustom.h"
#include "Actors/TrapBase.h"
#include "Actors/EnemySpawner.h"
#include "Characters/Enemies/CharacterEnemyBase.h"
#include "Characters/Player/CharacterPlayerBase.h"
#include "CustomFunctionLibrary.h"

AAreaControl::AAreaControl()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	SetRootComponent(RootSceneComp);
	AreaBound = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaBoundBoxComp"));
	AreaBound->SetupAttachment(RootComponent);

	AreaBound->OnComponentBeginOverlap.AddDynamic(this, &AAreaControl::HandleOnBeginOverlap_AreaBound);

	LevelId = TEXT("Empty");
}

void AAreaControl::BeginPlay()
{
	Super::BeginPlay();
	// Trigger controller right now
	/*FTimerHandle DelayControllingHandle;
	FTimerDelegate DelayControllingDelegate;
	DelayControllingDelegate.BindLambda([&]()
		{
			StartControllingArea();
		});
	GetWorldTimerManager().SetTimer(DelayControllingHandle, DelayControllingDelegate, 1.0f, false);*/
}

void AAreaControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAreaControl::StartControllingArea_Implementation()
{
	if (bHasStartedControllingArea) return;
	bHasStartedControllingArea = true;
	bIsCompleted = false;
	
	if (UGameInstanceCustom* CustomGI = GetWorld()->GetGameInstance<UGameInstanceCustom>())
	{
		int32 TotalPercent = GetTotalPercentValue();
		int32 RandomRatio = FMath::RandRange(0, TotalPercent);

		int32 RatioCounter = 0;
		for (const TPair<EAreaControlType, int32>& RatioPair : ControlTypeRatioMap)
		{
			RatioCounter += RatioPair.Value;
			if (RandomRatio <= RatioCounter)
			{
				AreaControlType = RatioPair.Key;
				switch (AreaControlType)
				{
					case EAreaControlType::OvercomeObstacles:
					{
						CompleteControllingArea();
						TArray<AActor*> OverlappingTraps;
						AreaBound->GetOverlappingActors(OverlappingTraps, ATrapBase::StaticClass());

						if (OverlappingTraps.Num() > 0)
						{
							float NumberOfTrapsMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, CustomGI->NumberOfFloors - 1), TrapNumberScaleWithDifficulty, CustomGI->FloorIndexCount);
							int32 NumberOfTrapsToActivate = FMath::RoundToInt32(NumberOfTrapsMultiplier * OverlappingTraps.Num());

							for (AActor* TempTrap : OverlappingTraps)
							{
								if (ATrapBase* BaseTrap = Cast<ATrapBase>(TempTrap))
								{
									
								}
							}
							UE_LOG(LogTemp, Warning, TEXT("Start controlling area type OvercomeObstacles with number of traps: %d and multiplier of %f"), NumberOfTrapsToActivate, NumberOfTrapsMultiplier);
						}
						break;
					}
					case EAreaControlType::EnemyWaves:
					{
						NumberOfWaves = FMath::RandRange(FMath::RoundToInt32(WavesNumberRange.X), FMath::RoundToInt32(WavesNumberRange.Y));
						WaveCount = 0;
						if (NumberOfWaves <= 0) return;
						UE_LOG(LogTemp, Warning, TEXT("Start controlling area type EnemyWaves with number of waves: %d"), NumberOfWaves);
						SpawnWaveOfEnemies();
						break;
					}
				}
				break;
			}
		}
	}
}

void AAreaControl::CompleteControllingArea_Implementation()
{
	bIsCompleted = true;
	if (OnAreaControlCompletedDelegate.IsBound())
	{
		OnAreaControlCompletedDelegate.Broadcast(this);
	}
}

int32 AAreaControl::GetTotalPercentValue()
{
	int32 TotalPercent = 0;
	for (const TPair<EAreaControlType, int32>& RatioPair : ControlTypeRatioMap)
	{
		TotalPercent += RatioPair.Value;
	}
	return TotalPercent;
}

void AAreaControl::SpawnWaveOfEnemies()
{
	if (UGameInstanceCustom* CustomGI = GetWorld()->GetGameInstance<UGameInstanceCustom>())
	{
		if (WaveCount < NumberOfWaves)
		{
			WaveCount++;
			TArray<AActor*> OverlappingSpawners;
			AreaBound->GetOverlappingActors(OverlappingSpawners, AEnemySpawner::StaticClass());
			UE_LOG(LogTemp, Warning, TEXT("SpawnWaveOfEnemies with number overlapping spawner %d"), OverlappingSpawners.Num());
			float SpawnerMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, CustomGI->NumberOfFloors - 1), EnemySpawnerNumberScaledWithDifficulty, CustomGI->FloorIndexCount);
			int32 NumberOfSpawners = FMath::RoundToInt32(SpawnerMultiplier * OverlappingSpawners.Num());
			if (NumberOfSpawners > 0)
			{
				TArray<int32> SpawnerIndexes;
				UCustomFunctionLibrary::GenerateRandomUniqueIndexesArray(NumberOfSpawners, OverlappingSpawners.Num(), SpawnerIndexes);
				for (int i = 0; i < SpawnerIndexes.Num(); i++)
				{
					if (SpawnerIndexes[i] < OverlappingSpawners.Num())
					{
						if (AEnemySpawner* Spawner = Cast<AEnemySpawner>(OverlappingSpawners[SpawnerIndexes[i]]))
						{
							ACharacterEnemyBase* SpawnedEnemy = Spawner->SpawnEnemy();
							if (SpawnedEnemy)
							{
								SpawnedEnemy->OnDiedDelegate.AddDynamic(this, &AAreaControl::HandleDelegate_OnEnemyDied);
								SpawnedEnemiesArray.Add(SpawnedEnemy);
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("EnemySpawner failed to spawn enemy in wave"));
							}
						}
					}
				}
			}
			else
			{
				CompleteControllingArea();
			}
		}
		else
		{
			CompleteControllingArea();
		}
	}
}

void AAreaControl::HandleDelegate_OnEnemyDied(class ACharacterEnemyBase* DeathEnemy)
{
	SpawnedEnemiesArray.Remove(DeathEnemy);
	if (AreSpawnedEnemiesAllDeath())
	{
		FTimerHandle DelaySpawnNextWaveHandle;
		FTimerDelegate DelaySpawnNextWaveDelegate;
		DelaySpawnNextWaveDelegate.BindLambda([&]()
		{
			SpawnWaveOfEnemies();
		});
		GetWorldTimerManager().SetTimer(DelaySpawnNextWaveHandle, DelaySpawnNextWaveDelegate, TimeToSpawnNextWave, false);
	}
}

bool AAreaControl::AreSpawnedEnemiesAllDeath()
{
	for (ACharacterEnemyBase* BaseEnemy : SpawnedEnemiesArray)
	{
		if (BaseEnemy && !BaseEnemy->bIsDeath) return false;
	}
	return true;
}

void AAreaControl::HandleOnBeginOverlap_AreaBound_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacterPlayerBase* PlayerCharacter = Cast<ACharacterPlayerBase>(OtherActor))
	{
		StartControllingArea();
	}
}
