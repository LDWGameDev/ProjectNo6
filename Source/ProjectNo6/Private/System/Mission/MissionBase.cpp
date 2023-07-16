// Copy Right LDW. All Rights Reserved.


#include "System/Mission/MissionBase.h"
#include "Engine/GameInstance.h"
#include "ActiveSound.h"
#include "AudioDeviceManager.h"
#include "Audio/ActorSoundParameterInterface.h"
#include "Engine/World.h"
#include "AudioDevice.h"

UMissionBase::UMissionBase()
{

}

void UMissionBase::OnStarted_Implementation()
{

}

UGameInstance* UMissionBase::CustomGetGameInstance()
{
	return (GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr);
}

void UMissionBase::CustomPlaySound2D(USoundBase* Sound, float VolumeMultiplier /*= 1.f*/, float PitchMultiplier /*= 1.f*/, float StartTime /*= 0.f*/, USoundConcurrency* ConcurrencySettings /*= nullptr*/, const AActor* OwningActor /*= nullptr*/, bool bIsUISound /*= true*/)
{
	if (!Sound || !GEngine || !GEngine->UseSound())
	{
		return;
	}

	UWorld* ThisWorld = GetWorld();
	if (!ThisWorld || !ThisWorld->bAllowAudioPlayback || ThisWorld->IsNetMode(NM_DedicatedServer))
	{
		return;
	}

	if (FAudioDeviceHandle AudioDevice = ThisWorld->GetAudioDevice())
	{
		FActiveSound NewActiveSound;
		NewActiveSound.SetSound(Sound);
		NewActiveSound.SetWorld(ThisWorld);

		NewActiveSound.SetPitch(PitchMultiplier);
		NewActiveSound.SetVolume(VolumeMultiplier);

		NewActiveSound.RequestedStartTime = FMath::Max(0.f, StartTime);

		NewActiveSound.bIsUISound = bIsUISound;
		NewActiveSound.bAllowSpatialization = false;

		if (ConcurrencySettings)
		{
			NewActiveSound.ConcurrencySet.Add(ConcurrencySettings);
		}

		NewActiveSound.Priority = Sound->Priority;
		NewActiveSound.SubtitlePriority = Sound->GetSubtitlePriority();

		// If OwningActor isn't supplied to this function, derive an owner from the WorldContextObject
		const AActor* WorldContextOwner = Cast<const AActor>(GetOuter());
		const AActor* ActiveSoundOwner = OwningActor ? OwningActor : WorldContextOwner;

		NewActiveSound.SetOwner(ActiveSoundOwner);

		TArray<FAudioParameter> Params;
		UActorSoundParameterInterface::Fill(ActiveSoundOwner, Params);

		AudioDevice->AddNewActiveSound(NewActiveSound, &Params);
	}
}

APlayerController* UMissionBase::CustomGetPlayerController()
{
	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		ULocalPlayer* LocalPlayer = GetWorld()->GetGameInstance()->GetLocalPlayerByIndex(0);
		if (LocalPlayer)
		{
			return LocalPlayer->PlayerController;
		}
	}
	return nullptr;
}
