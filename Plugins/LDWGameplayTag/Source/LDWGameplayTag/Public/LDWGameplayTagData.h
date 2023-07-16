// Copyright LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "LDWGameplayTagData.generated.h"


/**
 * GameplayTagNativeAdder responsible for creating native gameplay tags and holding registered gameplay tag variables
 */
struct LDWGAMEPLAYTAG_API FLDWGlobalTag : public FGameplayTagNativeAdder
{
public:
	FORCEINLINE static const FLDWGlobalTag& Get() { return LDWGlobalTagInstance; }

	/**
	 * SAMPLE CODE
	 * 
	 * FGameplayTag TestingTag;
	 */

protected:
	// Native create gameplay tags and register tag variables
	virtual void AddTags() override
	{
		UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();

		/**
		 * SAMPLE CODE
		 * 
		 * Native create tags
		 * TagManager.AddNativeGameplayTag(TEXT("UIType.PauseMenu.PauseMenuOption"));
		 * 
		 * Register tag variables 
		 * TestingTag = TagManager.RequestGameplayTag(TEXT("Item.Equipment"), true);
		 */
		
		TagManager.AddNativeGameplayTag(TEXT("State.Locomotion"));
		TagManager.AddNativeGameplayTag(TEXT("State.Action.Dash"));
		TagManager.AddNativeGameplayTag(TEXT("State.Action.LightAttack"));
		TagManager.AddNativeGameplayTag(TEXT("State.Action.LightAttackFinisher"));
		TagManager.AddNativeGameplayTag(TEXT("State.Action.LightAttackDash"));
		TagManager.AddNativeGameplayTag(TEXT("State.Action.HeavyAttack"));
		TagManager.AddNativeGameplayTag(TEXT("State.Action.QSkill"));

		TagManager.AddNativeGameplayTag(TEXT("Creature.Human"));
		TagManager.AddNativeGameplayTag(TEXT("Creature.Animal"));

		TagManager.AddNativeGameplayTag(TEXT("Creature.Object.Training"));
	
		TagManager.AddNativeGameplayTag(TEXT("Side.Ally"));
		TagManager.AddNativeGameplayTag(TEXT("Side.EnemySideA"));
		TagManager.AddNativeGameplayTag(TEXT("Side.EnemySideB"));
		TagManager.AddNativeGameplayTag(TEXT("Side.EnemySideC"));
		TagManager.AddNativeGameplayTag(TEXT("Side.Damageable"));
	}

private:
	static FLDWGlobalTag LDWGlobalTagInstance;
};


UCLASS()
class LDWGAMEPLAYTAG_API ULDWGameplayTagData : public UObject
{
	GENERATED_BODY()
};
