// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTaker.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HeavyGunsPawn.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UArrowComponent;
class ACannon;
class UHealthComponent;

UCLASS()
class TANKOGEDDON_API AHeavyGunsPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:

	AHeavyGunsPawn();

	UFUNCTION()
	virtual void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
	void DamageTaked(float Value);

	UFUNCTION()
	void Die();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		TSubclassOf< ACannon> CannonClassAdditional;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		ACannon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		float ChangeCannonTime = 5.0f;

	FTimerHandle ChangeTimer;
	
public:
 
	void SetupCannon(TSubclassOf<ACannon> newCannonClass);
	void Fire();
	void SwapCannon();
};
