// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCannon.h"
#include "PhysicsProjectile.h"
#include "TankPawn.h"
#include <Components/ArrowComponent.h>


void APhysicsCannon::FireProjectile()
{
	GEngine->AddOnScreenDebugMessage(-1, FireRate, FColor::Purple,
		FString::Printf(TEXT("Fire Projectile %d sum - %d"), --CountAmmunition, SumCountAmmunition));
	APhysicsProjectile* Projectile = FindProjectile();

	if (Projectile) {

		Projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
		Projectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
		Projectile->SetActorEnableCollision(true);
		Projectile->Start();
	}
}

void APhysicsCannon::PoolInitial()
{
	PoolPHProjectile.Reserve(CountAmmunition);
	for (int i = 0; i < CountAmmunition; ++i) {
		FVector local = FVector(0.0f, 0.0f, -100.0f);		// the shells are hidden while in the bullet
		FActorSpawnParameters param;
		param.Owner = this;

		APhysicsProjectile* tempProjectile = GetWorld()->SpawnActor<APhysicsProjectile>(PrjectileClass,
			param);
		if (tempProjectile) {
			tempProjectile->SetActorEnableCollision(false);
			tempProjectile->setLocal(local);

			ATankPawn* Tank = Cast<ATankPawn>(GetOwner());
			if (Tank) {
				tempProjectile->OnDieScore.AddUObject(Tank, &ATankPawn::AddScore);
			}
			PoolPHProjectile.Add(tempProjectile);
		}

	}
}

APhysicsProjectile* APhysicsCannon::FindProjectile()
{
	for (auto i : PoolPHProjectile) {
		if (i->isAvaivable()) {
			return i;
		}
	}
	return nullptr;
}
