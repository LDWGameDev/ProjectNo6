// Copy Right LDW. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(Config = GameUserSettings, configdonotcheckdefaults, BlueprintType)
class PROJECTNO6_API UCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

protected:
	// Gameplay Settings
	UPROPERTY(Config)
		bool bDashToCursor = true;

	// Audio Settings
	UPROPERTY(Config)
		int32 MasterVolume = 100;
	UPROPERTY(Config)
		int32 MusicVolume = 100;
	UPROPERTY(Config)
		int32 EffectVolume = 100;

	// Graphic Settings
	UPROPERTY(Config)
		int32 OverallGraphicSettings = 1;

	UPROPERTY(Config)
		TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::WindowedFullscreen;

public:
	UCustomGameUserSettings();

	/** Get global instance of this game user settings */
	static UCustomGameUserSettings* Get();

	UFUNCTION(BlueprintPure, Category = "Custom Game User Settings")
		bool GetDashToCursor() { return bDashToCursor; }
	UFUNCTION(BlueprintCallable, Category = "Custom Game User Settings")
		void SetDashToCursor(bool NewValue) { bDashToCursor = NewValue; }

	/** Setters and getters for Audio Settings */
	UFUNCTION(BlueprintPure, Category = "Custom Game User Settings")
		int32 GetMasterVolume() { return MasterVolume; }
	UFUNCTION(BlueprintCallable, Category = "Custom Game User Settings")
		void SetMasterVolume(int32 Value) { MasterVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "Custom Game User Settings")
		int32 GetMusicVolume() { return MusicVolume; }
	UFUNCTION(BlueprintCallable, Category = "Custom Game User Settings")
		void SetMusicVolume(int32 Value) { MusicVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "Custom Game User Settings")
		int32 GetEffectVolume() { return EffectVolume; }
	UFUNCTION(BlueprintCallable, Category = "Custom Game User Settings")
		void SetEffectVolume(int32 Value) { EffectVolume = Value; }

	UFUNCTION(BlueprintCallable, Category = "Custom Game User Settings")
		void SetOverallGraphicSettings(int32 Value) { OverallGraphicSettings = FMath::Clamp(Value, 0, 4); }
	UFUNCTION(BlueprintPure, Category = "Custom Game User Settings")
		int32 GetOverallGraphicSettings() { return OverallGraphicSettings; }

	UFUNCTION(BlueprintPure, Category = "Custom Game User Settings")
		TEnumAsByte<EWindowMode::Type> GetWindowMode() { return WindowMode; }
	UFUNCTION(BlueprintCallable, Category = "Custom Game User Settings")
		void SetWindowMode(TEnumAsByte<EWindowMode::Type> NewMode) { WindowMode = NewMode; }
};
