// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "BoxShell.h"

#include <Components/StaticMeshComponent.h>


void ABoxShell::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn) {
		TankPawn->AddAmmo(projectile);
		Destroy();
	}
}
