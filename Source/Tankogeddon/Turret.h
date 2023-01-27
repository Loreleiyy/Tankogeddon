// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTaker.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"


class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class ACannon;
class APawn;
class UHealthComponent;


UCLASS()
class TANKOGEDDON_API ATurret : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	
	ATurret();

	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float Value);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		TSubclassOf<ACannon> CannonClass;

	UPROPERTY()
		ACannon* Cannon;

	UPROPERTY()
		APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 10.0f;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void SetupCannon(TSubclassOf<ACannon> newCannonClass);
	void Targeting();
	void RotateToPlayer();
	void Fire();
	bool IsPlayerInRange();
	bool CanFire();

//public:	
//	
//	virtual void Tick(float DeltaTime) override;
//
};
