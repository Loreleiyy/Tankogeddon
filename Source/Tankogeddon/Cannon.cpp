// Fill out your copyright notice in the Description page of Project Settings.

#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "Cannon.h"
#include "Projectile.h"
#include <Engine/Engine.h>
#include <TimerManager.h>
#include <DrawDebugHelpers.h>

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	
}

void ACannon::Fire()
{
	if (start) {
		start = false;
		PoolProjectile.Reserve(CountAmmunition);
		for (int i = 0; i < CountAmmunition; ++i) {
			FVector local = FVector(0, 0, -100 - i * 100);		// снаряды спратаны, пока в пуле
			PoolProjectile.Add(GetWorld()->SpawnActor<AProjectile>(PrjectileClass,     // не знаю, как правильнее создавать
				local, ProjectileSpawnPoint->GetComponentRotation()));
			PoolProjectile[i]->setLocal(local);
		}
	}
	if (!IsReadyToFire()) {
		return;
	}

	if (Cannontype == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(-1, FireRate, FColor::Purple, 
			FString::Printf(TEXT("Fire Projectile %d sum - %d"), --CountAmmunition, SumCountAmmunition));
		AProjectile* Projectile = FindProjectile();

		if (Projectile) {
			
			Projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
			Projectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
			Projectile->Start();
		}
		TimeRate();
	}
	else if(Cannontype == ECannonType::FireTrace){
		GEngine->AddOnScreenDebugMessage(-1, FireRate, FColor::Red, 
			FString::Printf(TEXT("Fire trace %d sum - %d"), --CountAmmunition, SumCountAmmunition));

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams();
		traceParams.AddIgnoredActor(this);
		traceParams.bReturnPhysicalMaterial = false;

		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility)) {
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 1.0f, 0, 5);
			AActor* OverlappedActor = hitResult.GetActor();
			if (OverlappedActor) {
				UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *OverlappedActor->GetName());
				OverlappedActor->Destroy();
			}
		}
		else {
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}

		TimeRate();
	}
	else {
		countAmmo = 4;
		bReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(SeriesAmmoTimer, this, &ACannon::FireSeries, FireRate / 4, true);
	}

}

// альтернативный вариант стрельбы
void ACannon::FireSpecial()
{
	if (!IsReadyToFire()) {
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, FireRate / 2, FColor::Green, 
		FString::Printf(TEXT("Fire Special %d sum - %d"), --CountAmmunition, SumCountAmmunition));

	
}

// стрельба серией снарядов
void ACannon::FireSeries()
{
	GEngine->AddOnScreenDebugMessage(-1, FireRate+2, FColor::Blue,
		FString::Printf(TEXT("Fire series")));
	
	if (--countAmmo < 1) {
		GetWorldTimerManager().ClearTimer(SeriesAmmoTimer);
		Reload();
		
		GEngine->AddOnScreenDebugMessage(-1, FireRate, FColor::Blue,
			FString::Printf(TEXT("ammo %d sum - %d"), --CountAmmunition, SumCountAmmunition));
	}
}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire && CountAmmunition > 0;
}

void ACannon::Reload()
{
	bReadyToFire = true;
}

void ACannon::Recharge()  // перезарядка  R
{
	if (SumCountAmmunition < 1) {
		return;
	}
	int missingAmmo = 10 - CountAmmunition;
	if (missingAmmo >= SumCountAmmunition) {
		CountAmmunition += SumCountAmmunition;
		SumCountAmmunition = 0;
	}
	else {
		SumCountAmmunition -= missingAmmo;
		CountAmmunition += missingAmmo;
	}
}

void ACannon::TimeRate()  // ожидание между выстрелами
{
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

void ACannon::AddAmmo(int sum)
{
	SumCountAmmunition += sum;
}

void ACannon::DestroyProjectile()
{
	for (auto i : PoolProjectile) {
		if (i) {
			i->Destroy();
			i = nullptr;
		}
	}
}

AProjectile* ACannon::FindProjectile()
{
	for (auto i : PoolProjectile) {
		if (i->isAvaivable()) {
			return i;
		}
	}
	return nullptr;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	bReadyToFire = true;
	
}

// Called every frame
//void ACannon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

