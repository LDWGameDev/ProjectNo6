// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemplateBase.generated.h"

UCLASS()
class PROJECTNO6_API ATemplateBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATemplateBase();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		USceneComponent* RootSceneComp;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UChildActorComponent* PlayerStart;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UStaticMeshComponent* FloorMesh;

protected:
	virtual void BeginPlay() override;

};
