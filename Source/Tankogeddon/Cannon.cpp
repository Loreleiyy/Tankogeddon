// Fill out your copyright notice in the Description page of Project Settings.

#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "Cannon.h"
#include "Projectile.h"
#include "DamageTaker.h"
#include "TankPawn.h"
#include <Engine/Engine.h>
#include <TimerManager.h>
#include <DrawDebugHelpers.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include <Camera/CameraShakeBase.h>

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);
	CannonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);

	ShotEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShotEffect"));
	ShotEffect->SetupAttachment(ProjectileSpawnPoint);
	ShotEffect->SetAutoActivate(false);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);
	AudioEffect->SetAutoActivate(false);
}

void ACannon::Fire()
{

	if (!IsReadyToFire()) {
		return;
	}

	if (Cannontype == ECannonType::FireProjectile) {
		FireProjectile();
		TimeRate();
	}
	else if(Cannontype == ECannonType::FireTrace){
		FireTrace();

		TimeRate();
	}
	else {
		countAmmo = 4;
		bReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(SeriesAmmoTimer, this, &ACannon::FireSeries, FireRate / 4, true);
	}

	if (ShotEffect) {
		ShotEffect->ActivateSystem();
	}

	if (AudioEffect) {
		AudioEffect->Play();
	}

	if (CameraShake) {
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake);
	}
}


void ACannon::FireSpecial()  // an alternative shooting option is shooting without delay
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

void ACannon::TimeRate()  // waiting between shots
{
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

void ACannon::AddAmmo(int sum)
{
	SumCountAmmunition += sum;
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
	if (Cannontype == ECannonType::FireProjectile) {
		PoolInitial();
	}
}

void ACannon::Destroyed()
{
	for (auto i : PoolProjectile) {
		if (i) {
			i->Destroy();
			i = nullptr;
		}
	}
}

void ACannon::PoolInitial()
{
	PoolProjectile.Reserve(CountAmmunition);
	for (int i = 0; i < CountAmmunition; ++i) {
		FVector local = FVector(0.0f, 0.0f, -100.0f);		// the shells are hidden while in the bullet
		FActorSpawnParameters param;
		param.Owner = this;
		
		AProjectile* tempProjectile = GetWorld()->SpawnActor<AProjectile>(PrjectileClass,
			param);
		if (tempProjectile) {
			tempProjectile->SetActorEnableCollision(false);
			tempProjectile->setLocal(local);
			
			ATankPawn* Tank = Cast<ATankPawn>(GetOwner());
			if (Tank) {
				tempProjectile->OnDieScore.AddUObject(Tank, &ATankPawn::AddScore);
			}
			PoolProjectile.Add(tempProjectile);
		}

	}
}

void ACannon::FireProjectile()
{
	GEngine->AddOnScreenDebugMessage(-1, FireRate, FColor::Purple,
		FString::Printf(TEXT("Fire Projectile %d sum - %d"), --CountAmmunition, SumCountAmmunition));
	AProjectile* Projectile = FindProjectile();

	if (Projectile) {

		Projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
		Projectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
		Projectile->SetActorEnableCollision(true);
		Projectile->Start();
	}
	
}

void ACannon::FireTrace()
{
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

			IDamageTaker* damageActor = Cast<IDamageTaker>(OverlappedActor);
			if (damageActor) {
				FDamageData damageData;
				damageData.DamageValue = Damage;
				damageData.Instigator = GetOwner();
				damageData.DamageMaker = this;

				damageActor->TakeDamage(damageData);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OverlappedActor->GetName());
				OverlappedActor->Destroy();
			}
		}
	}
	else {
		DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
	}
}

// Called every frame
//void ACannon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

