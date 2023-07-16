// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNPC.generated.h"

UCLASS()
class PROJECTNO6_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()

public:	
	ABaseNPC();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

};
