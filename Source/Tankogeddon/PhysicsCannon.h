// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "PhysicsCannon.generated.h"


class APhysicsProjectile;

UCLASS()
class TANKOGEDDON_API APhysicsCannon : public ACannon
{
	GENERATED_BODY()
	
protected:
	virtual void FireProjectile() override;
	virtual void PoolInitial() override;
	APhysicsProjectile* FindProjectile();
	TArray<APhysicsProjectile*> PoolPHProjectile;
};
