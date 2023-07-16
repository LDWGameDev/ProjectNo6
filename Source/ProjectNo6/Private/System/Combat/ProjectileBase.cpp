// Copy Right LDW. All Rights Reserved.


#include "System/Combat/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/GameplayTagComponent.h"
#include "System/Combat/CombatMove.h"
#include "Components/ArrowComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(RootSphereComp);
	RootSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::HandleOnComponentBeginOverlap_RootComp);
	RootSphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::HandleOnComponentHit_RootComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->Velocity = FVector(600.0f, 0.0f, 0.0f);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if ((TriggerType == EProjectileTriggerType::TriggerAfterDuration || TriggerType == EProjectileTriggerType::TriggerWhenHitOrTimeOut) && TriggerTime > 0.0f)
	{
		FTimerHandle DelayDestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayDestroyTimerHandle, [this]() {
			TriggerProjectile();
		}, TriggerTime, false);
	}
}

void AProjectileBase::Init_Implementation(UCombatMoveProjectile* OwnerProjectileCombatMove)
{
	OwnerProjectileMove = OwnerProjectileCombatMove;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::HandleOnComponentHit_RootComp(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HitComponent || !OtherActor) return;
	if (TriggerType == EProjectileTriggerType::TriggerWhenHit || TriggerType == EProjectileTriggerType::TriggerWhenHitOrTimeOut)
	{
		TriggerProjectile();
	}
}

void AProjectileBase::HandleOnComponentBeginOverlap_RootComp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OverlappedComponent) return;

	if (UGameplayTagComponent* OverlappedTagComp = Cast<UGameplayTagComponent>(OtherActor->GetComponentByClass(UGameplayTagComponent::StaticClass())))
	{
		if (OverlappedTagComp->GetOwnerTagContainer().HasAny(OverlapTargetTag))
		{
			TriggerProjectile();
		}
	}
}

void AProjectileBase::TriggerProjectile_Implementation()
{
	if (bHasTriggered || IsUnreachable()) return;
	bHasTriggered = true;
	if (OnProjectileTriggeredDelegate.IsBound())
	{
		OnProjectileTriggeredDelegate.Broadcast(this);
	}
}

bool AProjectileBase::IsSelfUnreachable()
{
	return IsUnreachable();
}
