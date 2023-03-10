// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATankPawn;
class APawn;

UCLASS()
class TANKOGEDDON_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		ATankPawn* TankPawn;  // AI Tank

	UPROPERTY()
		APawn* PlayerPawn;   // Gamer

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TArray<FVector> PatrollingPoints;

	UPROPERTY()
		float MovementAccurency = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
		float ChangeCannonTime = 5.0f;

	int32 CurrentPatrollingIndex = 0;

	float GetRotationValue();
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerRange();
	bool CanFire();
	void Fire();

	bool IsPlayerSeen();
	void RangeTime();
	void Initialize();


	bool bplayerRange = false;
	FTimerHandle RangeTimer;
};
