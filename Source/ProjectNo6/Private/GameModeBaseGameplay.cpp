// Copy Right LDW. All Rights Reserved.


#include "GameModeBaseGameplay.h"
#include "Characters/Enemies/CharacterEnemyBase.h"
#include "Characters/Player/CharacterPlayerHuman.h"
#include "Kismet/GameplayStatics.h"

void AGameModeBaseGameplay::KillPlayer()
{
	if (ACharacterPlayerHuman* PlayerHuman = Cast<ACharacterPlayerHuman>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		PlayerHuman->ForceDie();
	}
}

void AGameModeBaseGameplay::KillAllEnemies()
{
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacterEnemyBase::StaticClass(), AllEnemies);
	for (AActor* TempActor : AllEnemies)
	{
		if (ACharacterEnemyBase* TempEnemy = Cast<ACharacterEnemyBase>(TempActor))
		{
			TempEnemy->ForceOnDied();
		}
	}
}

