// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyGunsPawn.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "TankPlayerController.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Components/ArrowComponent.h>

AHeavyGunsPawn::AHeavyGunsPawn()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	RootComponent = HitCollider;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(HitCollider);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanget.AddUObject(this, &AHeavyGunsPawn::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &AHeavyGunsPawn::Die);

}

void AHeavyGunsPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AHeavyGunsPawn::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

void AHeavyGunsPawn::Die()
{
	if (Cannon) {
		Cannon->Destroy();
	}
	Destroy();
}


void AHeavyGunsPawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(CannonClass);
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn != this) {
		GetWorldTimerManager().SetTimer(ChangeTimer, this, &AHeavyGunsPawn::SwapCannon, ChangeCannonTime, true);
	}
}

void AHeavyGunsPawn::SetupCannon(TSubclassOf<ACannon> newCannonClass)
{
	if (!newCannonClass) {
		return;
	}
	if (Cannon) {

		Cannon->Destroy();
	}
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;
	spawnParams.Owner = this;
	CannonClass = newCannonClass;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannonClass, spawnParams);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AHeavyGunsPawn::Fire()
{
	
	if (Cannon) {
		
		Cannon->Fire();
	}
	
}

void AHeavyGunsPawn::SwapCannon()
{
	TSubclassOf<ACannon> TempClass = CannonClass;
	SetupCannon(CannonClassAdditional);
	
	CannonClassAdditional = TempClass;
}
