// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "DamageTaker.h"
#include "HeavyGunsPawn.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class ATankPlayerController;
class UArrowComponent;
class ACannon;
class UHealthComponent;


UCLASS()
class TANKOGEDDON_API ATankPawn : public AHeavyGunsPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

/*	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;*///

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	UStaticMeshComponent* TurretMesh;//

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	UBoxComponent* BoxCollision;//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	UHealthComponent* HealthComponent;//

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	//	ACannon* Cannon;//

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	//	TSubclassOf< ACannon> CannonClass;//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		TSubclassOf< ACannon> CannonClassAdditional;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
	//	UArrowComponent* CannonSetupPoint;//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
		float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements | Rotation")
		float TurretRotationInterpolationKey = 0.5f;

	UPROPERTY()
		ATankPlayerController* TankController;

private:
	float targetForwardAxisValue = 0.0f;
	float targetRightAxisValue = 0.0f;
	float targetRotateRightAxisValue = 0.0f;
	int GameScore = 0;

	void MoveTick(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	

	void MoveRight(float Value);
	void RotateRight(float Value);
	//void Fire();//
	void FireSpecial();
	//void SetupCannnon(TSubclassOf<ACannon> newCannon);//
	void Recharge();
	void SwapCannon();
	void AddAmmo(int sum);
	void AddScore(int score);

	//virtual void TakeDamage(FDamageData DamageData) override;//

	//UFUNCTION()
	//void DamageTaked(float Value);//

	//UFUNCTION()
	//void Die();//

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
