// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsCannon.h"
#include "PhysicsProjectile.h"
#include "TankPawn.h"
#include <Components/ArrowComponent.h>




void APhysicsCannon::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void APhysicsCannon::FireProjectile()
{
	GEngine->AddOnScreenDebugMessage(-1, FireRate, FColor::Purple,
		FString::Printf(TEXT("Fire Projectile %d sum - %d"), --CountAmmunition, SumCountAmmunition));
	APhysicsProjectile* Projectile = FindProjectile();

	FRotator rotation = GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("Pitsh: %f Roll: %f Yaw: %f"), rotation.Pitch, rotation.Roll, rotation.Yaw);
	
	if (Projectile) {
		FVector local = ProjectileSpawnPoint->GetComponentLocation();
		Projectile->SetActorLocation(local);
		Projectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
		Projectile->SetActorEnableCollision(true);

		// horizontally the distance between the player and the projectile
		local.Z = 0;
		FVector PlayerLocal;
		if (PlayerPawn) {
			PlayerLocal = PlayerPawn->GetActorLocation();
		}
		PlayerLocal.Z = 0;
		float distance = FVector::Distance(local, PlayerLocal);
		UE_LOG(LogTemp, Warning, TEXT("PlayerRange: %f"), distance);
		float initSpeed = Projectile->GetInitialSpeed(distance, GetActorRotation().Pitch);
		Projectile->SetTrajectorySpeed(initSpeed);
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

