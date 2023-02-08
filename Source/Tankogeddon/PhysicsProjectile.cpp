// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "PhysicsComponent.h"
#include "DamageTaker.h"
#include "GameSruct.h"
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>


APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	moveVector = GetActorForwardVector() * TrajectorySpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectoty(GetActorLocation(), moveVector, 
		TrajectoryMaxTime, TrajectoryTimeStep, GetActorLocation().Z - 1.0f);

	FVector debugVector = CurrentTrajectory[CurrentTrajectory.Num() - 1];
	float debugDistance = FVector::Distance(debugVector, GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), debugDistance);
	UE_LOG(LogTemp, Warning, TEXT("H: %f"), GetActorLocation().Z);

	if (bShowTrajectory) {
		for (FVector position : CurrentTrajectory) {
			DrawDebugSphere(GetWorld(), position, 8.0f, 8, FColor::Purple, true, 1.0f, 0, 2);
		}
	}
	TrajectoryCurrentIndex = 0;
	Super::Start();
}

void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryCurrentIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	FVector newLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(newLocation);
	if (FVector::Distance(newLocation, CurrentTrajectory[TrajectoryCurrentIndex]) <= MoveAccurency) {
		++TrajectoryCurrentIndex;
		if (TrajectoryCurrentIndex >= CurrentTrajectory.Num()) {
			Explode();
			
		}
		else {
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), 
				CurrentTrajectory[TrajectoryCurrentIndex]);

			SetActorRotation(newRotation);
		}
	}
}


