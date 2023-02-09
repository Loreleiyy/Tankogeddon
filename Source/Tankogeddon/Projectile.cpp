// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Scorable.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "DamageTaker.h"
#include <DrawDebugHelpers.h>


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
	if (bExplode) {
		Explode();
	}
	else {
		DamageAndRepulsion(OtherActor);
	}
	
}

void AProjectile::Explode()
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
		for (const FHitResult& hitResult : AttachHit) {
			AActor* OtherActor = hitResult.GetActor();
			if (!OtherActor) {
				continue;
			}

			TakeDamageOrImpulse(OtherActor, true);
		}
	}
	Stop();
}

void AProjectile::DamageAndRepulsion(AActor* OtherActor)
{
	AActor* owner = GetOwner(); // ACAnnon
	AActor* OwnewByOwner = owner != nullptr ? owner->GetOwner() : nullptr; // ATankPawn or ATurret


	if (OtherActor != owner && OtherActor != OwnewByOwner) {
		
		TakeDamageOrImpulse(OtherActor, false);

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

void AProjectile::TakeDamageOrImpulse(AActor* OtherActor, bool impulse)
{
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
				if (impulse) {
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
				else {
					mesh->AddForce(forceVector * PushForce, NAME_None, true);
				}
			}
		}
	}
}

// Called every frame
//void AProjectile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

