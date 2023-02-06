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
		TrajectoryMaxTime, TrajectoryTimeStep, 0.0f);

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
			Stop();
		}
		else {
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), 
				CurrentTrajectory[TrajectoryCurrentIndex]);

			SetActorRotation(newRotation);
		}
	}
}

void APhysicsProjectile::Explode()
{
	FVector startPos = GetActorLocation();
	FVector endPos = startPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	params.bTraceComplex = true;
	params.TraceTag = "Explode Trace";

	TArray<FHitResult> AttachHit;
	FQuat Rotation = FQuat::Identity;

	bool bSweepResult = GetWorld()->SweepMultiByChannel(AttachHit, startPos, endPos, Rotation,
		ECollisionChannel::ECC_Visibility, Shape, params);

	DrawDebugSphere(GetWorld(), startPos, ExplodeRadius, 5, FColor::Green, false, 2.0f);

	if (bSweepResult) {
		for (FHitResult hitResult : AttachHit) {
			AActor* OtherActor = hitResult.GetActor();
			if (!OtherActor) {
				continue;
			}

			IDamageTaker* DamageTakeActor = Cast<IDamageTaker>(OtherActor);
			if (DamageTakeActor) {
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;
				DamageTakeActor->TakeDamage(damageData);
			}
			else {
				UPrimitiveComponent* mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
				if (mesh) {
					if (mesh->IsSimulatingPhysics()) {
						FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
						forceVector.Normalize();
						mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
					}
				}
			}
		}
	}
}
