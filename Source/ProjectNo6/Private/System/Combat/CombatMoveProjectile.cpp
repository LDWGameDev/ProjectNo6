// Copy Right LDW. All Rights Reserved.


#include "System/Combat/CombatMoveProjectile.h"
#include "System/Combat/ProjectileBase.h"
#include "System/Combat/CombatMove.h"
#include "System/Combat/CombatComponent.h"
#include "System/AssetManager/CustomAssetManager.h"

UCombatMoveProjectile::UCombatMoveProjectile()
{
	
}

void UCombatMoveProjectile::OnMoveTriggered_Implementation()
{
	Super::OnMoveTriggered_Implementation();
	if (!OwnerCombatComp || !OwnerCombatComp->GetOwner() || !GetWorld()) return;

	if (TSubclassOf<AProjectileBase> ProjectileClass = UCustomAssetManager::GetClassFromSoftPtr(ProjectileSoftClassPtr))
	{
		FVector ProjectileLocation = (bSpawnInLocal) ? OwnerCombatComp->GetOwner()->GetActorTransform().TransformPosition(SpawnLocation) : SpawnLocation;
		FRotator ProjectileRotation = (bSpawnInLocal) ? OwnerCombatComp->GetOwner()->GetActorTransform().TransformRotation(SpawnRotation.Quaternion()).Rotator() : SpawnRotation;
		FActorSpawnParameters SpawnParams;
		ProjectileReference = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileLocation, ProjectileRotation, SpawnParams);
		if (ProjectileReference)
		{
			ProjectileReference->OwnerProjectileMove = this;
			ProjectileReference->OnProjectileTriggeredDelegate.AddDynamic(this, &UCombatMoveProjectile::HandleOnProjectileTriggered);
		}
	}
}

void UCombatMoveProjectile::DealDamageToTargets_Implementation(const TArray<AActor*>& TargetActorArray)
{
	Super::DealDamageToTargets_Implementation(TargetActorArray);
}

void UCombatMoveProjectile::HandleOnProjectileTriggered_Implementation(AProjectileBase* Projectile)
{
	
}

