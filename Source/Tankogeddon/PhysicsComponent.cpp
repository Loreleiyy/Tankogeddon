// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsComponent.h"


//UPhysicsComponent::UPhysicsComponent()
//{
//	PrimaryComponentTick.bCanEverTick = true;
//}



//void UPhysicsComponent::BeginPlay()
//{
//	Super::BeginPlay();	
//}



//void UPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

TArray<FVector> UPhysicsComponent::GenerateTrajectoty(FVector StartPos, FVector Velosity, float MaxTime, float TimeStep, float MinZValue)
{
	 TArray<FVector> trajectory;
	 FVector GravityVector(0.0f, 0.0f, Gravity);

	 for (float time = 0; time < MaxTime; time += TimeStep) {
		 FVector position = StartPos + Velosity * time + GravityVector * time * time / 2;
		 if (position.Z <= MinZValue) {
			 break;
		 }
		 trajectory.Add(position);
	 }
	 return trajectory;
}
