// Copy Right LDW. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/DataTable.h"
#include "CustomData.generated.h"

class UAnimMontage;
class UParticleSystem;
class UNiagaraSystem;
class UStaticMesh;
class AHoldingActor;
class USoundBase;
class USplineComponent;
class UFXSystemComponent;
class ACharacterEnemyBase;

UENUM(BlueprintType)
enum class EAlphaCurveType : uint8
{
	Linear UMETA(DisplayName = "Linear"),
	EaseInOut UMETA(DisplayName = "Ease In Out"),
	HardInOut UMETA(DisplayName = "Hard In Out"), 
	HardInEaseOut UMETA(DisplayName = "Hard In Ease Out"),
	Custom UMETA(DisplayName = "Custom")
};


USTRUCT(BlueprintType)
struct FAlphaCurve
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alpha Curve")
		EAlphaCurveType AlphaCurveType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alpha Curve")
		TSoftObjectPtr<UCurveFloat> AlphaFloatCurveSoftPtr;
};


/**
 * Holds custom montage information
 */
USTRUCT(BlueprintType)
struct FCustomMontage : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Montage")
		FString MontageID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Montage")
		TSoftObjectPtr<UAnimMontage> AnimMontageSoftObject;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Montage")
		FName SectionToPlay;
	/** The higher the value, the higher the priority */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage Section")
		int32 Priority = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage Section")
		bool bCanPlayIfSamePriority = true;

	/** Returns a boolean indicates if this custom montage is valid to use */
	bool CheckValidMontage() { return (!MontageID.IsEmpty() /*&& AnimMontageSoftObject.Get()*/); }
};


UENUM(BlueprintType)
enum class EFXType : uint8
{
	Cascade UMETA(DisplayName = "Cascade"),
	Niagara UMETA(DisplayName = "Niagara")
};


USTRUCT(BlueprintType)
struct FCustomFX : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom FX")
		FString ID = "";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom FX")
		EFXType FXType = EFXType::Cascade;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom FX", meta = (EditConditionHides, EditCondition = "FXType==EFXType::Cascade"))
		TSoftObjectPtr<UParticleSystem> CascadeSystemSoftObject = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom FX", meta = (EditConditionHides, EditCondition = "FXType==EFXType::Niagara"))
		TSoftObjectPtr<UNiagaraSystem> NiagaraSystemSoftObject = nullptr;
};


USTRUCT(BlueprintType)
struct FHitTargetFX : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit Target FX")
		FString FXId = TEXT("");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit Target FX")
		bool bInLocalCoordinate = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit Target FX")
		FVector Location;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit Target FX")
		FRotator Rotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit Target FX")
		FVector Scale = FVector(1.0f, 1.0f, 1.0f);
};


UENUM(BlueprintType)
enum class ETargetLocationControlType : uint8 
{
	None UMETA(DisplayName = "None"), 
	CircleDistribution UMETA(DisplayName = "Circle Distribution"), 
	DirectionalForceAngleDistribution UMETA(DisplayName = "Directional Force Angle Distribution"), 
	RadialForce UMETA(DisplayName = "Radial Force"), 
	RadialDistribution UMETA(DisplayName = "Radial Distribution"), 

};


UENUM(BlueprintType)
enum class ETargetRotationControlType : uint8
{
	None UMETA(DisplayName = "None"),
	LookAtInstigator UMETA(DisplayName = "Look At Instigator")
};


UENUM(BlueprintType)
enum class EHitType : uint8
{
	NormalHit UMETA(DisplayName = "Normal Hit"), 
	StunHit UMETA(DisplayName = "Stun Hit"), 
	LightHit UMETA(DisplayName = "Light Hit"), 
	HeavyHit UMETA(DisplayName = "Heavy Hit")
};

UENUM(BlueprintType)
enum class EProjectileTriggerType : uint8
{
	TriggerWhenHit UMETA(DisplayName = "Trigger When Hit"),
	TriggerWhenHitOrTimeOut UMETA(DisplayName = "Trigger When Hit Or Timer Out"),
	TriggerAfterDuration UMETA(DisplayName = "Trigger After Duration"),
	CustomTrigger UMETA(DisplayName = "Custom Trigger")
};


UENUM(BlueprintType)
enum class ECustomStatType : uint8
{
	None UMETA(DisplayName = "None"), 
	HP UMETA(DisplayName = "Health"), 
	ATK UMETA(DisplayName = "Attack"), 
	DEF UMETA(DisplayName = "Defense"), 
	Power UMETA(DisplayName = "Power")
};


USTRUCT(BlueprintType)
struct FMoveStat
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ECustomStatType StatType = ECustomStatType::ATK;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Value;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsPercentage = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bGetFromStatComp = false;
};


USTRUCT(BlueprintType)
struct FDefaultWeapon
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<AHoldingActor> HoldingActorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector OffsetLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRotator OffsetRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName SocketAttachTo;
};


UENUM(BlueprintType)
enum class EHoldingObjectType : uint8
{
	None UMETA(DisplayName = "None"), 
	Torch UMETA(DisplayName = "Torch"), 
	GunSword UMETA(DisplayName = "GunSword")
};


USTRUCT(BlueprintType)
struct FCustomSound
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSoftObjectPtr<USoundBase> SoundObject;
};



USTRUCT(BlueprintType)
struct FCustomSplinePoint
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Location;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRotator Rotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector ArriveTangent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector LeaveTangent;
};


USTRUCT(BlueprintType)
struct FSplineTrail
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Id;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FCustomSplinePoint> SplinePoints;

	// Set value at runtime
	UPROPERTY(BlueprintReadWrite)
		USplineComponent* RuntimeCreatedSplineComp;
};


UENUM(BlueprintType)
enum class EFloorObjectiveType : uint8
{
	Empty UMETA(DisplayName = "Empty"),
	Unknown UMETA(DisplayName = "Unknown"),
	Upgrade UMETA(DisplayName = "Upgrade"), 
	Gold UMETA(DisplayName = "Gold"), 
	MagicBook UMETA(DisplayName = "Maigc Book"), 
	Healing UMETA(DisplayName = "Healing")
};


USTRUCT(BlueprintType)
struct FFloor
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EFloorObjectiveType> Objectives;
};


USTRUCT(BlueprintType)
struct FTemple
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 TempleId = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString TempleName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxTemplateId = -1;
};


UENUM(BlueprintType)
enum class EAreaControlType : uint8
{
	OvercomeObstacles UMETA(DisplayName = "Overcome Obstacle"),
	EnemyWaves UMETA(DisplayName = "Wave"), 
};


USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString EnemeyId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D SpawnRatioScaleWithDifficulty = FVector2D(0.0f, 1.0f);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<ACharacterEnemyBase> EnemyClass;
};


UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	ENormal UMETA(DisplayName = "Normal"),
	EGrantMission UMETA(DisplayName = "Grant Mission")
};


USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EDialogueType DialogueType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = true))
		FString Content;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USoundBase* DialogueSound;
};


USTRUCT(BlueprintType)
struct FDialogueGraph
{
	GENERATED_BODY()
public:
	TArray<FDialogueLine> DialogueLines;
};


/**
 * 
 */
UCLASS()
class PROJECTNO6_API UCustomData : public UObject
{
	GENERATED_BODY()

public:
	/** Material parameters used in Aura Outline effect  */
	// Aura Outline effect alpha blend value parameter name
	static const FName AuraOutline;
	// Aura Outline effect color parameter name
	static const FName AuraOutlineColor;
	static const FName AuraOutlinePower;
	// Aura Outline effect white color
	static const FVector AuraOutlineWhiteColor;
	// Aura Outline effect blue color
	static const FVector AuraOutlineBlueColor;
	// Aura Outline effect red color
	static const FVector AuraOutlineRedColor;
};
