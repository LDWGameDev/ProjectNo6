// Copy Right LDW. All Rights Reserved.


#include "GameInstanceCustom.h"
#include "Kismet/GameplayStatics.h"
#include "System/Settings/CustomGameUserSettings.h"
#include "System/AssetManager/CustomAssetManager.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/PlayerStart.h"
#include "Characters/Player/CharacterPlayerBase.h"
#include "Characters/Player/PlayerControllerGameplay.h"
#include "System/Combat/CombatComponent.h"
#include "Characters/Enemies/CharacterEnemyBase.h"
#include "System/Combat/CombatMove.h"
#include "Characters/Player/BaseUpgrade.h"
#include "GameFramework/PlayerController.h"

UGameInstanceCustom::UGameInstanceCustom()
{

}

void UGameInstanceCustom::Init()
{
	Super::Init();

	FTimerHandle HandleDelayInitSound;
	GetTimerManager().SetTimer(HandleDelayInitSound, this, &UGameInstanceCustom::InitSoundMix, 0.5f, false);
	FTimerHandle HandleLoadSettings;
	GetTimerManager().SetTimer(HandleLoadSettings, this, &UGameInstanceCustom::LoadSettings, 1.0f, false);
	GenerateFloorIndexes();
}

void UGameInstanceCustom::CustomSpawnSoundAtLocation(const FString& SoundId, const FVector& Location)
{
	for (const FCustomSound& Sound : CustomSoundArray)
	{
		if (Sound.Id.Equals(SoundId))
		{
			if (USoundBase* SoundBaseObject = UCustomAssetManager::GetObjectFromSoftPtr<USoundBase>(Sound.SoundObject))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, SoundBaseObject, Location);
			}
			return;
		}
	}
}

void UGameInstanceCustom::EnterBossFloor_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Entered boss floor of temple with index: %d"), CurrentTempleIndex);
}

void UGameInstanceCustom::CompleteCurrentFloor_Implementation()
{
	if (FloorIndexCount >= 0 && FloorIndexCount <= NumberOfFloors - 1)
	{
		if (OnPreCompletedFloorDelegate.IsBound()) OnPreCompletedFloorDelegate.Broadcast(FloorIndexCount);
		OnPreCompletedFloor(FloorIndexCount);
		FloorIndexCount++;
		if (OnPostCompletedFloorDelegate.IsBound()) OnPostCompletedFloorDelegate.Broadcast(FloorIndexCount);
		OnPostCompletedFloor(FloorIndexCount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to complete current floor count: %d"), FloorIndexCount);
	}
}

void UGameInstanceCustom::OnPreCompletedFloor_Implementation(int32 CompletedFloorIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Complete current floor with index: %d"), CompletedFloorIndex);
}

void UGameInstanceCustom::OnPostCompletedFloor_Implementation(int32 NextFloorIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("New floor index: %d"), NextFloorIndex);
	if (NextFloorIndex >= NumberOfFloors - 1)
	{
		// Should enter boss floor
	}
}

void UGameInstanceCustom::StartRunWithCurrentFloorIndex_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("StartRunWithCurrentFloorIndex() called"));
	if (FloorIndexCount >= 0 && FloorIndexCount < NumberOfFloors - 1)
	{
		int32 RandomTemplateIndex = FMath::RandRange(0, TemplesArray[CurrentTempleIndex].MaxTemplateId);
		FString TemplateLevelString = "Temple_" + FString::FromInt(CurrentTempleIndex) + "_" + FString::FromInt(RandomTemplateIndex);

		FVector TemplateLocation = LevelStreamingLocation1;
		if (CurrentActiveLevelStreaming && CurrentActiveLevelStreaming->LevelTransform.GetLocation().Equals(LevelStreamingLocation1, 100.0f)) TemplateLocation = LevelStreamingLocation2;

		bool LoadingResult = false;
		FString NewLevelStreamName = TemplateLevelString + "_Floor_" + FString::FromInt(FloorIndexCount);
		FRotator LevelStreamRotation = FRotator(0.0f, 0.0f, 0.0f);
		ULevelStreamingDynamic* NewLevelStreamingDynamic = ULevelStreamingDynamic::LoadLevelInstance(this, TemplateLevelString, TemplateLocation, LevelStreamRotation, LoadingResult, NewLevelStreamName);

		UE_LOG(LogTemp, Warning, TEXT("StartRunWithCurrentFloorIndex new level streaming name: %s"), *NewLevelStreamName);

		if (LoadingResult && NewLevelStreamingDynamic)
		{
			LastActiveLevelStreaming = CurrentActiveLevelStreaming;
			CurrentActiveLevelStreaming = NewLevelStreamingDynamic;
			CurrentActiveLevelStreaming->OnLevelShown.AddDynamic(this, &UGameInstanceCustom::HandleOnCurrentLevelStreamingShown);
		}
	}
	else
	{
		FString TemplateLevelString = "Temple_" + FString::FromInt(CurrentTempleIndex) + "_Boss";

		FVector TemplateLocation = FVector(10000.0f, 10000.0f, 0.0f);
		//FVector TemplateLocation = LevelStreamingLocation1;
		//if (CurrentActiveLevelStreaming && CurrentActiveLevelStreaming->LevelTransform.GetLocation().Equals(LevelStreamingLocation1, 100.0f)) TemplateLocation = LevelStreamingLocation2;

		bool LoadingResult = false;
		FString NewLevelStreamName = TemplateLevelString + "_Floor_" + FString::FromInt(FloorIndexCount);
		FRotator LevelStreamRotation = FRotator(0.0f, 0.0f, 0.0f);
		ULevelStreamingDynamic* NewLevelStreamingDynamic = ULevelStreamingDynamic::LoadLevelInstance(this, TemplateLevelString, TemplateLocation, LevelStreamRotation, LoadingResult, NewLevelStreamName);

		UE_LOG(LogTemp, Warning, TEXT("StartRunWithCurrentFloorIndex new level streaming name: %s"), *NewLevelStreamName);

		if (LoadingResult && NewLevelStreamingDynamic)
		{
			LastActiveLevelStreaming = CurrentActiveLevelStreaming;
			CurrentActiveLevelStreaming = NewLevelStreamingDynamic;
			CurrentActiveLevelStreaming->OnLevelShown.AddDynamic(this, &UGameInstanceCustom::HandleOnCurrentLevelStreamingShown);
		}
	}
}

void UGameInstanceCustom::GenerateFloorIndexes()
{
	NumberOfFloors = 5;
	FloorIndexCount = 0;
	UE_LOG(LogTemp, Warning, TEXT("Generated number of floors: %d, floor index count: %d"), NumberOfFloors, FloorIndexCount);
}

TArray<UBaseUpgrade*> UGameInstanceCustom::GetRandomUpgrades()
{
	UE_LOG(LogTemp, Warning, TEXT("GetRandomUpgrades() called"));
	TArray<UBaseUpgrade*> Upgrades;
	TArray<UBaseUpgrade*> CopiedUpgradesArray;
	if (FloorIndexCount == 2 /*|| FloorIndexCount == 5*/)
	{
		CopiedUpgradesArray = UpgradesArrayTypeTwo;
	}
	else 
	{
		CopiedUpgradesArray = UpgradesArrayTypeOne;
	}

	if (CopiedUpgradesArray.Num() > 0)
	{
		if (CopiedUpgradesArray.Num() <= 2)
		{
			Upgrades.Add(CopiedUpgradesArray[FMath::RandRange(0, CopiedUpgradesArray.Num() - 1)]);
			Upgrades.Add(CopiedUpgradesArray[FMath::RandRange(0, CopiedUpgradesArray.Num() - 1)]);
			Upgrades.Add(CopiedUpgradesArray[FMath::RandRange(0, CopiedUpgradesArray.Num() - 1)]);
		}
		else
		{
			int32 Breaker = 30;
			int32 ElementsCount = 0;
			while (Breaker > 0)
			{
				Breaker--;
				int32 RandomIndex = FMath::RandRange(0, CopiedUpgradesArray.Num() - 1);
				if (!Upgrades.Contains(CopiedUpgradesArray[RandomIndex]))
				{ 
					Upgrades.Add(CopiedUpgradesArray[RandomIndex]);
					ElementsCount ++;
					if (ElementsCount >= 3)
					{
						Breaker = 0;
						break;
					}
				}
			}
		}
	}
	return Upgrades;
}

void UGameInstanceCustom::ApplyUpgrade_Implementation(UBaseUpgrade* UpgradeToApply)
{
	if (UpgradeToApply)
	{
		UpgradeToApply->DoUpgrade(UGameplayStatics::GetPlayerController(this, 0));
	}
}

bool UGameInstanceCustom::CompleteCurrentObjective_Implementation()
{
	/*if (GetCurrentObjectiveType() == EFloorObjectiveType::Empty)
	{
		UE_LOG(LogTemp, Warning, TEXT("Did not meet conditions to complete current objective, current objective index: %d, current floor index %d"), CurrentObjectiveIndex, CurrentFloorIndex);
		return false;
	}
	if (OnCompletedFloorObjectiveDelegate.IsBound()) OnCompletedFloorObjectiveDelegate.Broadcast(CurrentObjectiveIndex, CurrentFloorIndex, Floors[CurrentFloorIndex].Objectives[CurrentObjectiveIndex]);
	HandleCompletedObjective(CurrentObjectiveIndex, CurrentFloorIndex, GetCurrentObjectiveType());

	FString ObjectiveAsString = *UEnum::GetValueAsName(Floors[CurrentFloorIndex].Objectives[CurrentObjectiveIndex]).ToString();
	UE_LOG(LogTemp, Warning, TEXT("Completed objective %s at index %d, floor index %d"), *ObjectiveAsString, CurrentObjectiveIndex, CurrentFloorIndex);

	CurrentFloorIndex++;
	CurrentObjectiveIndex = -1;*/

	return true;
}

EFloorObjectiveType UGameInstanceCustom::GetCurrentObjectiveType()
{
	if (!AreCurrentObjectiveAndFloorIndexesValid()) return EFloorObjectiveType::Empty;
	return Floors[CurrentFloorIndex].Objectives[CurrentObjectiveIndex];
}

void UGameInstanceCustom::GetParentObjectivesIndexes(TArray<int32>& OutParentIndexes, int32 ObjectiveIndex, int32 FloorIndex)
{
	OutParentIndexes.Empty();
	// UE_LOG(LogTemp, Warning, TEXT("GetParentObjectivesIndexes called"));
	if (!AreObjectiveAndFloorIndexesValid(ObjectiveIndex, FloorIndex) || FloorIndex == 7) return;

	if (IsFloorObjectiveNotEmpty(FloorIndex + 1, ObjectiveIndex - 1)) OutParentIndexes.Add(ObjectiveIndex - 1);
	if (IsFloorObjectiveNotEmpty(FloorIndex + 1, ObjectiveIndex)) OutParentIndexes.Add(ObjectiveIndex);
	if (IsFloorObjectiveNotEmpty(FloorIndex + 1, ObjectiveIndex + 1)) OutParentIndexes.Add(ObjectiveIndex + 1);

	/*FString ParentIndexes = "";
	UE_LOG(LogTemp, Warning, TEXT("Parent objectives of index %d in floor %d is "), ObjectiveIndex, FloorIndex);
	for (int i = 0; i < OutParentIndexes.Num(); i++)
	{
		ParentIndexes = ParentIndexes + FString::FromInt(i) + " ";
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ParentIndexes);*/
}

void UGameInstanceCustom::ResetCurrentFloorIndex_Implementation()
{
	CurrentFloorIndex = 0;
}

bool UGameInstanceCustom::AreCurrentObjectiveAndFloorIndexesValid()
{
	return AreObjectiveAndFloorIndexesValid(CurrentObjectiveIndex, CurrentFloorIndex);
}

bool UGameInstanceCustom::AreObjectiveAndFloorIndexesValid(int32 ObjectiveIndex, int32 FloorIndex)
{
	return (FloorIndex > -1 && FloorIndex < Floors.Num() && ObjectiveIndex > -1 && ObjectiveIndex < Floors[FloorIndex].Objectives.Num());
}

int32 UGameInstanceCustom::ProduceRandomNumberOfObjectives(float ThreeObjectivesRate /*= 70.0f*/)
{
	if (FMath::RandRange(0.0f, 100.0f) < ThreeObjectivesRate) return 3;
	return ((int32)FMath::RandRange(2, 4));
}

EFloorObjectiveType UGameInstanceCustom::ProduceRandomObjectiveType()
{
	uint8 RandomEnum = FMath::RandRange(1, static_cast<int>(EFloorObjectiveType::Healing));
	return static_cast<EFloorObjectiveType>(RandomEnum);
}

bool UGameInstanceCustom::IsFloorObjectiveNotEmpty(int32 FloorIndex, int32 ObjectiveIndex)
{
	if (!AreObjectiveAndFloorIndexesValid(ObjectiveIndex, FloorIndex)) return false;
	return (Floors[FloorIndex].Objectives[ObjectiveIndex] != EFloorObjectiveType::Empty);
}

int32 UGameInstanceCustom::GetRandomInLinkableIndexes(int32 Index)
{
	TArray<int32> LinkableIndexes;
	if (Index < 0 || Index > 4) return -1;
	if (Index == 0)
	{
		LinkableIndexes.Add(0);
		LinkableIndexes.Add(1);
	}
	else if (Index == 4)
	{
		LinkableIndexes.Add(3);
		LinkableIndexes.Add(4);
	}
	else
	{
		LinkableIndexes.Add(Index - 1);
		LinkableIndexes.Add(Index);
		LinkableIndexes.Add(Index + 1);
	}
	return LinkableIndexes[FMath::RandRange(0, LinkableIndexes.Num() - 1)];
}

void UGameInstanceCustom::GenerateFloorObjectives_Implementation()
{
	if (true) return;
	if(!bCanGenerateObjectives) return;
	bCanGenerateObjectives = false;
	Floors.Reserve(8);
	for (int i = 0; i < 8; i++)
	{
		FFloor TempFloor;
		TempFloor.Objectives.Reserve(5);
		for (int j = 0; j < 5; j++)
		{
			TempFloor.Objectives.Add(EFloorObjectiveType::Empty);
		}
		int32 NumberOfObjectives = ProduceRandomNumberOfObjectives();
		int32 Counter = 0;
		int32 BreakPointCounter = 0;

		if (i == 0)
		{
			// Full random objectives in first floor
			while (Counter < NumberOfObjectives)
			{
				int32 RandomIndex = FMath::RandRange(0, 4);
				if (TempFloor.Objectives[RandomIndex] == EFloorObjectiveType::Empty)
				{
					TempFloor.Objectives[RandomIndex] = ProduceRandomObjectiveType();
					Counter++;
				}

				BreakPointCounter++;
				if (BreakPointCounter > 50) break;
			}
		}
		else
		{
			// Random but make sure every objectives of lower floor has at least 1 connection to objectives of higher floor
			int32 LowerFloorNotEmptyObjectivesNumber = GetNumberOfNotEmptyObjectivesOfFloor(i - 1);
			if (NumberOfObjectives >= LowerFloorNotEmptyObjectivesNumber)
			{
				for (int j = 0; j < 5; j++)
				{
					int32 RandomLinkableIndex = GetRandomInLinkableIndexes(j);
					if (Floors[i - 1].Objectives[j] != EFloorObjectiveType::Empty && TempFloor.Objectives[RandomLinkableIndex] == EFloorObjectiveType::Empty)
					{
						TempFloor.Objectives[RandomLinkableIndex] = ProduceRandomObjectiveType();
						Counter++;
						if (Counter >= NumberOfObjectives) break;
					}
				}
			}
			else
			{
				int32 DeltaAmount = LowerFloorNotEmptyObjectivesNumber - NumberOfObjectives;
				for (int j = 0; j < 5; j++)
				{
					if (GetNumberOfNotEmptyLinkableChildObjectives(i, j) >= 2)
					{
						TempFloor.Objectives[j] = ProduceRandomObjectiveType();
						Counter++;
						DeltaAmount --;
						if (DeltaAmount <= 0 || Counter > 4) break;
					}
				}

				for (int j = 0; j < 5; j++)
				{
					int32 RandomLinkableIndex = GetRandomInLinkableIndexes(j);
					if (GetNumberOfNotEmptyLinkableParentObjectives(i - 1, j) == 0 && TempFloor.Objectives[RandomLinkableIndex] == EFloorObjectiveType::Empty)
					{
						TempFloor.Objectives[j] = ProduceRandomObjectiveType();
						Counter++;
						if (Counter > 4) break;
					}
				}
			}

			// Random remain objectives if current number of objectives not enough
			while (Counter < NumberOfObjectives)
			{
				int32 RandomIndex = FMath::RandRange(0, 4);
				if (IsEmptyAndHasValidChildObjective(TempFloor, i, RandomIndex))
				{
					TempFloor.Objectives[RandomIndex] = ProduceRandomObjectiveType();
					Counter++;
				}
				BreakPointCounter++;
				if (BreakPointCounter > 50) break;
			}
		}
		

		Floors.Add(TempFloor);
	}
}

bool UGameInstanceCustom::IsEmptyAndHasValidChildObjective(FFloor& Floor, int32 FloorIndex, int32 ObjectiveIndex)
{
	bool IsEmptyObject = (Floor.Objectives.Num() > ObjectiveIndex && Floor.Objectives[ObjectiveIndex] == EFloorObjectiveType::Empty);
	if (FloorIndex == 0)
	{
		return IsEmptyObject;
	}
	else if (FloorIndex > 0)
	{
		bool HasPathFromLowerFloor = false;
		if (IsValidIndexAndNotEmptyObjective(FloorIndex - 1, ObjectiveIndex) ||
			IsValidIndexAndNotEmptyObjective(FloorIndex - 1, ObjectiveIndex + 1) ||
			IsValidIndexAndNotEmptyObjective(FloorIndex - 1, ObjectiveIndex - 1))
		{
			HasPathFromLowerFloor = true;
		}
		return (IsEmptyObject && HasPathFromLowerFloor);
	}
	return false;
}

bool UGameInstanceCustom::IsValidIndexAndNotEmptyObjective(int32 FloorIndex, int32 ObjectiveIndex)
{
	if (FloorIndex < 0 || ObjectiveIndex < 0 || ObjectiveIndex > 4) return false;
	return (Floors[FloorIndex].Objectives[ObjectiveIndex] != EFloorObjectiveType::Empty);
}

int32 UGameInstanceCustom::GetNumberOfNotEmptyObjectivesOfFloor(int32 FloorIndex)
{
	if(FloorIndex < 0 || FloorIndex > 7) return -1;
	int32 Counter = 0;
	for (int i = 0; i < 5; i++)
	{
		if (Floors[FloorIndex].Objectives[i] != EFloorObjectiveType::Empty) Counter ++;
	}
	return Counter;
}

int32 UGameInstanceCustom::GetNumberOfNotEmptyLinkableChildObjectives(int32 FloorIndexOfObjective, int32 ObjectiveIndexToCheck)
{
	if(FloorIndexOfObjective < 1 || FloorIndexOfObjective > 7 || ObjectiveIndexToCheck < 0 || ObjectiveIndexToCheck > 4) return -1;
	int32 Counter = 0;
	if (IsFloorObjectiveNotEmpty(FloorIndexOfObjective - 1, FloorIndexOfObjective - 1)) Counter++;
	if (IsFloorObjectiveNotEmpty(FloorIndexOfObjective - 1, FloorIndexOfObjective)) Counter++;
	if (IsFloorObjectiveNotEmpty(FloorIndexOfObjective - 1, FloorIndexOfObjective + 1)) Counter++;
	return Counter;
}

int32 UGameInstanceCustom::GetNumberOfNotEmptyLinkableParentObjectives(int32 FloorIndexOfObjective, int32 ObjectiveIndexToCheck)
{
	if (FloorIndexOfObjective < 0 || FloorIndexOfObjective > 6 || ObjectiveIndexToCheck < 0 || ObjectiveIndexToCheck > 4) return -1;
	int32 Counter = 0;
	if (IsFloorObjectiveNotEmpty(FloorIndexOfObjective + 1, FloorIndexOfObjective - 1)) Counter++;
	if (IsFloorObjectiveNotEmpty(FloorIndexOfObjective + 1, FloorIndexOfObjective)) Counter++;
	if (IsFloorObjectiveNotEmpty(FloorIndexOfObjective + 1, FloorIndexOfObjective + 1)) Counter++;
	return Counter;
}

void UGameInstanceCustom::HandleOnCurrentLevelStreamingShown_Implementation()
{
	// TODO: unload last active level, teleport player to new location
	UE_LOG(LogTemp, Warning, TEXT("Loaded new temple map: %s at location %s"), *CurrentActiveLevelStreaming->GetName(), *CurrentActiveLevelStreaming->LevelTransform.GetLocation().ToString());
	
	APlayerStart* ClosetPlayerStartToCurrentActiveLevelStream = GetClosetPlayerStartToCurrentActiveLevelStream();
	ACharacterPlayerBase* CharacterPlayerBase = Cast<ACharacterPlayerBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	APlayerControllerGameplay* GameplayPlayerController = Cast<APlayerControllerGameplay>(UGameplayStatics::GetPlayerController(this, 0));
	if (ClosetPlayerStartToCurrentActiveLevelStream && CharacterPlayerBase && GameplayPlayerController)
	{
		CharacterPlayerBase->SetActorLocation(ClosetPlayerStartToCurrentActiveLevelStream->GetActorLocation());
		//GameplayPlayerController->TransitionOutOfBlack();
		UE_LOG(LogTemp, Warning, TEXT("Teleported player character to new temple location"));
		UE_LOG(LogTemp, Warning, TEXT("CurrentLevelStreamDynamic rotation: %s, location: %s"), *CurrentActiveLevelStreaming->LevelTransform.GetRotation().Rotator().ToString(), 
			*CurrentActiveLevelStreaming->LevelTransform.GetLocation().ToString());

		FTimerHandle DelayOutOfBlackHandle;
		FTimerDelegate DelayOutOfBackDelegate;
		DelayOutOfBackDelegate.BindLambda([&]{
			APlayerControllerGameplay* GameplayPlayerController = Cast<APlayerControllerGameplay>(UGameplayStatics::GetPlayerController(this, 0));
			if (GameplayPlayerController)
			{
				GameplayPlayerController->TransitionOutOfBlack();
			}
		});
		GetWorld()->GetTimerManager().SetTimer(DelayOutOfBlackHandle, DelayOutOfBackDelegate, 0.25f, false);
		OnLoadedNewFloor();
	}
	
	if (LastActiveLevelStreaming)
	{
		LastActiveLevelStreaming->SetIsRequestingUnloadAndRemoval(true);
		UE_LOG(LogTemp, Warning, TEXT("Requested unload and remove last active level stream"));
	}
}

void UGameInstanceCustom::OnLoadedNewFloor_Implementation()
{
	
}

APlayerStart* UGameInstanceCustom::GetClosetPlayerStartToCurrentActiveLevelStream()
{
	TArray<AActor*> OutPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), OutPlayerStarts);
	if (OutPlayerStarts.Num() == 0 || !CurrentActiveLevelStreaming) return nullptr;

	UE_LOG(LogTemp, Warning, TEXT("Number of PlayerStart: %d"), OutPlayerStarts.Num());
	
	AActor* ClosetPlayerStart = OutPlayerStarts[0];
	float ClosetDistance = 100000.0f;
	for (int i = 1; i < OutPlayerStarts.Num(); i++)
	{
		float Distance = (OutPlayerStarts[i]->GetActorLocation() - CurrentActiveLevelStreaming->LevelTransform.GetLocation()).Length();
		if (Distance < ClosetDistance)
		{
			ClosetPlayerStart = OutPlayerStarts[i];
			ClosetDistance = Distance;
		}
	}
	return Cast<APlayerStart>(ClosetPlayerStart);
}

void UGameInstanceCustom::OnEnemyDied_Implementation(ACharacterEnemyBase* DeathEnemy)
{
	if (!DeathEnemy || !DeathEnemy->GetCombatComp() || !DeathEnemy->GetCombatComp()->LastDamageDetails || !DeathEnemy->GetCombatComp()->LastDamageDetails->InstigatorCombatMove) return;
	TArray<FString> SlowMotionEffectMoves = {
			TEXT("gun_finish"),
			TEXT("torch_swing"), 
			TEXT("sword_la_swing"),
			TEXT("gun_fire_crit_impact"),
			TEXT("sword_la_combo3")
	};
	if (SlowMotionEffectMoves.Contains(DeathEnemy->GetCombatComp()->LastDamageDetails->InstigatorCombatMove->MoveID))
	{
		SetTimeDilationForDuration(0.02f, 0.25f);
	}
}

void UGameInstanceCustom::SetTimeDilationForDuration(float TimeDilation /*= 1.0f*/, float Duration /*= 1.0f*/)
{
	if (TimeDilation > 0.0f && TimeDilation < 3.0f)
	{
		if (GetTimerManager().IsTimerActive(TimeDilationHandle)) GetTimerManager().ClearTimer(TimeDilationHandle);
		UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
		
		float DelayTime = Duration * TimeDilation;
		FTimerDelegate TimeDilationDelegate;
		TimeDilationDelegate.BindLambda([&]{
			UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
		});
		GetTimerManager().SetTimer(TimeDilationHandle, TimeDilationDelegate, DelayTime, false);
	}
}

void UGameInstanceCustom::StartRunWithCurrentIndexes_Implementation()
{
	/*if (CurrentTempleIndex > -1 && CurrentTempleIndex < 6 && AreCurrentObjectiveAndFloorIndexesValid() && CurrentTempleIndex < TemplesArray.Num() && TemplesArray[CurrentTempleIndex].MaxTemplateId >= 0)
	{
		int32 RandomTemplateIndex = FMath::RandRange(0, TemplesArray[CurrentTempleIndex].MaxTemplateId);
		FString TemplateLevelString = "Temple_" + FString::FromInt(CurrentTempleIndex) + "_" + FString::FromInt(RandomTemplateIndex);

		FVector TemplateLocation = LevelStreamingLocation1;
		if(CurrentActiveLevelStreaming && CurrentActiveLevelStreaming->LevelTransform.GetLocation().Equals(LevelStreamingLocation1, 100.0f)) TemplateLocation = LevelStreamingLocation2;

		bool LoadingResult = false;
		FString NewLevelStreamName = TemplateLevelString + "_Temple_" + FString::FromInt(CurrentTempleIndex) + "_" + FString::FromInt(CurrentFloorIndex) + "_" + FString::FromInt(CurrentObjectiveIndex);
		FRotator LevelStreamRotation = FRotator(0.0f, 0.0f, 0.0f);
		ULevelStreamingDynamic* NewLevelStreamingDynamic = ULevelStreamingDynamic::LoadLevelInstance(this, TemplateLevelString, TemplateLocation, LevelStreamRotation, LoadingResult, NewLevelStreamName);
		if (LoadingResult && NewLevelStreamingDynamic)
		{
			LastActiveLevelStreaming = CurrentActiveLevelStreaming;
			CurrentActiveLevelStreaming = NewLevelStreamingDynamic;
			CurrentActiveLevelStreaming->OnLevelShown.AddDynamic(this, &UGameInstanceCustom::HandleOnCurrentLevelStreamingShown);
		}
	}*/
}

void UGameInstanceCustom::LoadSettings_Implementation()
{
	ApplyAudioSettings();
	ApplyGraphicsSettings();
}

void UGameInstanceCustom::ApplyGraphicsSettings_Implementation()
{
	if (UCustomGameUserSettings* CustomUserSettings = UCustomGameUserSettings::Get())
	{
		TArray<FIntPoint> Resolutions;
		if (UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions))
		{
			switch (CustomUserSettings->GetWindowMode())
			{
				case EWindowMode::Fullscreen:
				{
					CustomUserSettings->SetScreenResolution(Resolutions.Last());
					break;
				}
				case EWindowMode::Windowed:
				{
					int32 TargetResolutionIndex = FMath::CeilToInt32((float)Resolutions.Num() / 2);
					TargetResolutionIndex = FMath::Clamp(TargetResolutionIndex, 0, Resolutions.Num());
					CustomUserSettings->SetScreenResolution(Resolutions[TargetResolutionIndex]);
				}
			}
		}

		CustomUserSettings->SetFullscreenMode(CustomUserSettings->GetWindowMode());
		// CustomUserSettings->ApplyResolutionSettings(true);
		CustomUserSettings->ApplySettings(true);
	}
}

void UGameInstanceCustom::InitSoundMix_Implementation()
{
	
}

void UGameInstanceCustom::ApplyAudioSettings_Implementation()
{
	
}