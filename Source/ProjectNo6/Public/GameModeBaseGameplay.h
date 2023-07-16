// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBaseGameplay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNO6_API AGameModeBaseGameplay : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec, Category = "Commands")
		void KillPlayer();

	UFUNCTION(Exec, Category = "Commands")
		void KillAllEnemies();
};
