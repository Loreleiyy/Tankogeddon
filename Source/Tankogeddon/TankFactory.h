// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTaker.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankFactory.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class UHealthComponent;
class ATankPawn;
class ATargetPoint;

UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	

	ATankFactory();

	virtual void TakeDamage(FDamageData DamageData) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* TankSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
		TSubclassOf<ATankPawn> TankSpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
		float SpawnTankRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
		TArray<ATargetPoint*> TankWayPoints;

	UFUNCTION()
	void DamageTaked(float DamageValue);

	UFUNCTION()
	void Die();

	void SpawnNewTank();

	//virtual void Tick(float DeltaTime) override;
};
