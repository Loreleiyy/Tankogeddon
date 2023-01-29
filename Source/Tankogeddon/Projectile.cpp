// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Scorable.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "DamageTaker.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	//SphereCollision = CreateDefaultSubobject< USphereComponent>(TEXT("BoxCollision"));
	//SphereCollision->SetupAttachment(SceneComp);
	//SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneComp);
	//ProjectileMesh->SetupAttachment(SphereCollision);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	
}

void AProjectile::Start()
{
	deAvailable();
	
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AProjectile::Move, MoveRate, true, 0.0f);
	GetWorldTimerManager().SetTimer(DisableTimer, this, &AProjectile::Stop, lifeTime, false);
}

void AProjectile::deAvailable()
{
	bAvailable = !bAvailable;
}

void AProjectile::Stop()
{
	deAvailable();
	GetWorldTimerManager().ClearTimer(MovementTimer);
	GetWorldTimerManager().ClearTimer(DisableTimer);
	SetActorEnableCollision(false);
	SetActorLocation(startLocation);
}

void AProjectile::setLocal(FVector& start)
{
	startLocation = start;
	SetActorLocation(startLocation);
}

void AProjectile::AddScore(int score)
{
	if (OnDieScore.IsBound()) {
		OnDieScore.Broadcast(score);
	}
}



// Called when the game starts or when spawned
//void AProjectile::BeginPlay()
//{
//	Super::BeginPlay();
//}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* owner = GetOwner(); // ACAnnon
	AActor* OwnewByOwner = owner != nullptr ? owner->GetOwner() : nullptr; // ATankPawn or ATurret


	if (OtherActor != owner && OtherActor != OwnewByOwner) {
		IDamageTaker* DamageActor = Cast<IDamageTaker>(OtherActor);
		if (DamageActor) {
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.Instigator = owner;
			DamageData.DamageMaker = this;

			DamageActor->TakeDamage(DamageData);
		
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OtherActor->GetName());

			OtherActor->Destroy();
		}

		IScorable* ScoreActor = Cast<IScorable>(OtherActor);
		if (ScoreActor) {
			if (ScoreActor->isDie()) {
				int score = ScoreActor->GetScore();
				AddScore(score);
			}
		}

		Stop();
	}
}

// Called every frame
//void AProjectile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

