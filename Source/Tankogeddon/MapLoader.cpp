// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "TankPawn.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

AMapLoader::AMapLoader()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	MapLoaderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapLoaderMesh"));
	MapLoaderMesh->SetupAttachment(BoxCollision);

	MapLoaderMesh->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnMeshOverlapBegin);
}

void AMapLoader::FactoryDestroy()
{
	factoryDestroy = true;
}

void AMapLoader::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (PlayerPawn && PlayerPawn == OtherActor && factoryDestroy) {
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}



//void AMapLoader::BeginPlay()
//{
//	Super::BeginPlay();
//}


//void AMapLoader::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

