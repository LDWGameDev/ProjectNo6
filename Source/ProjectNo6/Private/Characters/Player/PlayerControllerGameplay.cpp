// Copy Right LDW. All Rights Reserved.


#include "Characters/Player/PlayerControllerGameplay.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "System/AssetManager/CustomAssetManager.h"
#include "Characters/Player/CharacterPlayerBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "GameInstanceCustom.h"
#include "CustomInterface.h"


APlayerControllerGameplay::APlayerControllerGameplay()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void APlayerControllerGameplay::BeginPlay()
{
	Super::BeginPlay();

	DisableDefaultFocusInput();
	bShowMouseCursor = true;

	// Set default input mode that enhanced input action can work
	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetWidgetToFocus(nullptr);
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputModeGameAndUI);

	// Add default input mapping context
	UInputMappingContext* DefaultInputMappingContext = UCustomAssetManager::GetObjectFromSoftPtr(DefaultInputMappingContextSoftObject);
	if (DefaultInputMappingContext && GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			EnhancedInputSubsystem->AddMappingContext(DefaultInputMappingContext, 10);
		}
	}

	if (UGameInstanceCustom* CustomGameInstance = Cast<UGameInstanceCustom>(UGameplayStatics::GetGameInstance(this)))
	{
		CustomGameInstance->OnCompletedFloorObjectiveDelegate.AddDynamic(this, &APlayerControllerGameplay::HandleOnCompletedObjectiveDelegate);
	}
}

void APlayerControllerGameplay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bUpdateGameplayMouseLocation)
	{
		CalculatesGameplayMouseLocation();
	}
}

void APlayerControllerGameplay::AddInteractableActor(AActor* Actor)
{
	if (Actor && Actor->Implements<UCustomInterface>())
	{
		InteractableActors.Add(Actor);
	}
}

void APlayerControllerGameplay::RemoveInteractableActor(AActor* Actor)
{
	for (int i = 0; i < InteractableActors.Num(); i++)
	{
		if (InteractableActors[i] == Actor)
		{
			InteractableActors.RemoveAtSwap(i, 1, true);
		}
	}
}

void APlayerControllerGameplay::TriggerTempleSelection_Implementation()
{
	
}

void APlayerControllerGameplay::BackToPersistentGameplay_Implementation()
{
	bUpdateGameplayMouseLocation = true;
}

void APlayerControllerGameplay::SetupInputComponent()
{
	Super::SetupInputComponent();

	// CastChecked to manually crash game if cast fail
	if (UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (UInputAction* MoveInputAction = UCustomAssetManager::GetObjectFromSoftPtr(MovementInputActionSoftObject))
		{
			EnhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleInputActionTriggered_Movement);
			EnhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &APlayerControllerGameplay::HandleInputActionCompleted_Movement);
		}
		if (UInputAction* DashInputAction = UCustomAssetManager::GetObjectFromSoftPtr(DashInputActionSoftObject))
		{
			EnhancedInputComp->BindAction(DashInputAction, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleInputActionTriggered_Dash);
		}
		if (UInputAction* LightAttackInputAction = UCustomAssetManager::GetObjectFromSoftPtr(LightAttackInputActionSoftObject))
		{
			EnhancedInputComp->BindAction(LightAttackInputAction, ETriggerEvent::Started, this, &APlayerControllerGameplay::HandleInputAction_LightAttack_Started);
			EnhancedInputComp->BindAction(LightAttackInputAction, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleInputAction_LightAttack_Triggered);
			EnhancedInputComp->BindAction(LightAttackInputAction, ETriggerEvent::Completed, this, &APlayerControllerGameplay::HandleInputAction_LightAttack_Completed);
			EnhancedInputComp->BindAction(LightAttackInputAction, ETriggerEvent::Canceled, this, &APlayerControllerGameplay::HandleInputAction_LightAttack_Canceled);
		}
		if (UInputAction* HeavyAttackInputAction = UCustomAssetManager::GetObjectFromSoftPtr(HeavyAttackInputActionSoftObject))
		{
			EnhancedInputComp->BindAction(HeavyAttackInputAction, ETriggerEvent::Started, this, &APlayerControllerGameplay::HandleInputAction_HeavyAttack_Started);
			EnhancedInputComp->BindAction(HeavyAttackInputAction, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleInputAction_HeavyAttack_Triggered);
			EnhancedInputComp->BindAction(HeavyAttackInputAction, ETriggerEvent::Completed, this, &APlayerControllerGameplay::HandleInputAction_HeavyAttack_Completed);
			EnhancedInputComp->BindAction(HeavyAttackInputAction, ETriggerEvent::Canceled, this, &APlayerControllerGameplay::HandleInputAction_HeavyAttack_Canceled);
		}
		if (UInputAction* QSkillInputAction = UCustomAssetManager::GetObjectFromSoftPtr(QSkillInputActionSoftObject))
		{
			EnhancedInputComp->BindAction(QSkillInputAction, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleInputActionTriggered_QSkill);
		}
		if (UInputAction* QSkillInputAction = UCustomAssetManager::GetObjectFromSoftPtr(MiddleMouseScrollInputActionSoftObject))
		{
			EnhancedInputComp->BindAction(QSkillInputAction, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleInputActionTriggered_MiddleMouseScroll);
		}
		if (UInputAction* EscapeInput = UCustomAssetManager::GetObjectFromSoftPtr<UInputAction>(EscapeInputAction))
		{
			EnhancedInputComp->BindAction(EscapeInput, ETriggerEvent::Triggered, this, &APlayerControllerGameplay::HandleEscapeTriggered);
		}
	}
}

bool APlayerControllerGameplay::CheckInputKeyInInteractableActors(const FKey& Key)
{
	for (AActor* TempActor : InteractableActors)
	{
		if (TempActor && TempActor->Implements<UCustomInterface>())
		{
			bool Result = false;
			ICustomInterface::Execute_TriggerInteractionKey(TempActor, this, Key, Result);
			if (Result) return true;
		}
	}
	return false;
}

void APlayerControllerGameplay::HandleOnCompletedObjectiveDelegate_Implementation(int32 ObjectiveIndex, int32 FloorIndex, EFloorObjectiveType ObjectiveType)
{
	
}

void APlayerControllerGameplay::HandleInputActionTriggered_Movement(const FInputActionValue& Value)
{
	FVector2D MoveInputValue = Value.Get<FVector2D>();
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputActionTriggered_Movement(MoveInputValue);
	}
}

void APlayerControllerGameplay::HandleInputActionCompleted_Movement(const FInputActionValue& Value)
{
	FVector2D MoveInputValue = Value.Get<FVector2D>();
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputActionCompleted_Movement(MoveInputValue);
	}
}

void APlayerControllerGameplay::HandleInputActionTriggered_Dash(const FInputActionValue& Value)
{
	bool DashInput = Value.Get<bool>();
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputActionTriggered_Dash(DashInput);
	}
}

void APlayerControllerGameplay::HandleInputAction_LightAttack_Started(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_LightAttack_Started();
	}
}

void APlayerControllerGameplay::HandleInputAction_LightAttack_Triggered(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_LightAttack_Triggered();
	}
}

void APlayerControllerGameplay::HandleInputAction_LightAttack_Completed(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_LightAttack_Completed();
	}
}

void APlayerControllerGameplay::HandleInputAction_LightAttack_Canceled(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_LightAttack_Canceled();
	}
}

void APlayerControllerGameplay::HandleInputAction_HeavyAttack_Started(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_HeavyAttack_Started();
	}
}

void APlayerControllerGameplay::HandleInputAction_HeavyAttack_Triggered(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_HeavyAttack_Triggered();
	}
}

void APlayerControllerGameplay::HandleInputAction_HeavyAttack_Completed(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_HeavyAttack_Completed();
	}
}

void APlayerControllerGameplay::HandleInputAction_HeavyAttack_Canceled(const FInputActionValue& Value)
{
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputAction_HeavyAttack_Canceled();
	}
}

void APlayerControllerGameplay::HandleInputActionTriggered_QSkill(const FInputActionValue& Value)
{
	bool QSkillInput = Value.Get<bool>();
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputActionTriggered_QSkill(QSkillInput);
	}
}

void APlayerControllerGameplay::HandleInputActionTriggered_MiddleMouseScroll(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();
	if (ACharacterPlayerBase* ControlledPlayerCharacter = GetPawn<ACharacterPlayerBase>())
	{
		ControlledPlayerCharacter->OnInputActionTriggered_MiddleMouseScroll(ScrollValue);
	}
}

void APlayerControllerGameplay::HandleEscapeTriggered_Implementation(const FInputActionValue& Value)
{
	
}

void APlayerControllerGameplay::CalculatesGameplayMouseLocation()
{
	if (ACharacterPlayerBase* ControlledBasePlayer = GetPawn<ACharacterPlayerBase>())
	{
		// Get mouse world location, mouse forward direction and character foot location
		FVector MouseWorldLocation;
		FVector MouseForwardDirection;
		DeprojectMousePositionToWorld(MouseWorldLocation, MouseForwardDirection);
		FVector CharacterFootLocation = ControlledBasePlayer->GetFootLocation();

		// Formula to calculate gameplay mouse location: MouseWorldLocation.Z + X * MouseForwardDirection.Z = CharacterFootLocation.Z --> Finds X 
		float Multiplier = UKismetMathLibrary::SafeDivide((CharacterFootLocation.Z - MouseWorldLocation.Z), MouseForwardDirection.Z);
		if (Multiplier != 0.0f)
		{
			GameplayMouseLocation = MouseWorldLocation + Multiplier * MouseForwardDirection;
		}
		else
		{
			GameplayMouseLocation = FVector::ZeroVector;
		}
	}
	else
	{
		GameplayMouseLocation = FVector::ZeroVector;
	}
}

void APlayerControllerGameplay::DisableDefaultFocusInput()
{
	TSharedRef<FNavigationConfig> Navigation = MakeShared<FNavigationConfig>();
	Navigation->bKeyNavigation = false;
	Navigation->bTabNavigation = false;
	Navigation->bAnalogNavigation = false;
	FSlateApplication::Get().SetNavigationConfig(Navigation);
}

bool APlayerControllerGameplay::AddToCoins(int32 AmountToAdd)
{
	if (AmountToAdd < 0 && Coins + AmountToAdd < 0) return false;
	int32 LastValue = Coins;
	Coins += AmountToAdd;
	OnCoinsValueChanged(AmountToAdd, LastValue, Coins);
	return true;
}

void APlayerControllerGameplay::OnCoinsValueChanged_Implementation(int32 ChangeAmount, int32 LastValue, int32 NewValue)
{

}
