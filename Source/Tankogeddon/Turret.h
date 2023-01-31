// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "DamageTaker.h"
#include "HeavyGunsPawn.h"
#include "Scorable.h"
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
class TANKOGEDDON_API ATurret : public AHeavyGunsPawn, public IScorable
{
	GENERATED_BODY()
	
public:	
	
	ATurret();

	UFUNCTION()
	virtual int GetScore() override;

	UFUNCTION()
	virtual bool isDie() override;



protected:
	
	

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int Score = 10;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	
	void Targeting();
	void RotateToPlayer();
	
	bool IsPlayerInRange();
	bool CanFire();

//public:	
//	
//	virtual void Tick(float DeltaTime) override;
//
};
