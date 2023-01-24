// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameSruct.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireTrace = 0 UMETA(DisplayName = "Use Trace"),
	FireProjectile = 1 UMETA(DisplayName = "Use projectile"),
	FireSeries = 2 UMETA(DisplayName = "Use series")
};

UCLASS()
class TANKOGEDDON_API UGameSruct : public UObject
{
	GENERATED_BODY()
	
};
