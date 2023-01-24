// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		ATankPawn* TankPawn;

	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSecond) override;
	FVector GetMousePosition() { return MousePos; }
	void Fire();
	void FireSpecial();
	void Recharge();
	void SwapCannon();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	FVector MousePos;
};
