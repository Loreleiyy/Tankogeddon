// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Cannon.h"
#include "TankPlayerController.h"
#include "HealthComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>


ATankPawn::ATankPawn()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);
}

FVector ATankPawn::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = TurretRotation.Pitch;
	targetRotation.Roll = TurretRotation.Roll;
	FRotator newTurretRotation = FMath::Lerp(TurretRotation, targetRotation, TurretRotationInterpolationKey);
	//UE_LOG(LogTemp, Warning, TEXT("targetRotation: %s, LerpRotation: %s"), *targetRotation.ToString(), *newTurretRotation.ToString());

	TurretMesh->SetWorldRotation(newTurretRotation);
}

FVector ATankPawn::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}

//Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTick(DeltaTime);

	// Tank Rotation
	float yamRotation = RotationSpeed * targetRotateRightAxisValue * DeltaTime;
	float LerpRotateValue = FMath::Lerp(targetRotateRightAxisValue, LerpRotateValue, TurretRotationInterpolationKey);

	// UE_LOG(LogTemp, Warning, TEXT("No Lerp: %f, Lerp: %f"), targetRotateRightAxisValue, LerpRotateValue);

	FRotator currentRotation = GetActorRotation();
	yamRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yamRotation, 0.0f);
	SetActorRotation(newRotation);

	// turret Rotation
	if (TankController) {
		FVector mousePos = TankController->GetMousePosition();
		RotateTurretTo(mousePos);
	}
}

void ATankPawn::MoveForward(float Value)
{
	targetForwardAxisValue = Value;
}


// Task 2.1, 2.3
void ATankPawn::MoveTick(float DeltaTime)
{
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition = CurrentPosition + ForwardVector * MoveSpeed * targetForwardAxisValue * DeltaTime +
		RightVector * MoveSpeed * targetRightAxisValue * DeltaTime;
	SetActorLocation(movePosition, true);
}

// Task 2.2
void ATankPawn::MoveRight(float Value)
{
	targetRightAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	targetRotateRightAxisValue = Value;
}



void ATankPawn::FireSpecial()
{
	if (Cannon) {
		Cannon->FireSpecial();
	}
}


void ATankPawn::Recharge()
{
	if (Cannon) {
		Cannon->Recharge();
	}
}

void ATankPawn::SwapCannon()
{
	TSubclassOf<ACannon> TempClass = CannonClass;
	SetupCannon(CannonClassAdditional);
	
	CannonClassAdditional = TempClass;
}

void ATankPawn::AddAmmo(int sum)
{
	if (Cannon) {
		Cannon->AddAmmo(sum);
	}
}

void ATankPawn::AddScore(int score)
{
	GameScore += score;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), GameScore);
}

// Called to bind functionality to input
//void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}

