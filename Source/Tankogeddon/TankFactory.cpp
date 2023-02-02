// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "HealthComponent.h"
#include "TankPawn.h"
#include "MapLoader.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Engine/TargetPoint.h>
#include <Kismet/GameplayStatics.h>


ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(BoxCollision);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TankSpawnPoint"));
	TankSpawnPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanget.AddUObject(this, &ATankFactory::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);

	DestroyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroyMesh"));
	DestroyMesh->SetupAttachment(BoxCollision);
	
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}


void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	
	
	GetWorldTimerManager().SetTimer(spawnTimer, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
	DestroyMesh->SetHiddenInGame(true);
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank factory damage: %f, health: %f"), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::Die()
{
	if (MapLoader) {
		MapLoader->FactoryDestroy();
	}
	GetWorldTimerManager().ClearTimer(spawnTimer);
	BuildingMesh->SetHiddenInGame(true);
	DestroyMesh->SetHiddenInGame(false);
	SetActorEnableCollision(false);
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(TankSpawnClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	newTank->SetPatrollingPath(TankWayPoints);
	UGameplayStatics::FinishSpawningActor(newTank, newTank->GetTransform());
}


//void ATankFactory::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

