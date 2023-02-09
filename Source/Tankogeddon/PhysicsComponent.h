// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	//UPhysicsComponent();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
		float Gravity = -9.8f;

	TArray<FVector> GenerateTrajectoty(FVector StartPos, FVector Velosity, float MaxTime, float TimeStep, float MinZValue = 0);

	float GetInitialSpeed(float distance, float angle);
	

protected:

	//virtual void BeginPlay() override;



	

		
};
