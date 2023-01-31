// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSruct.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"


class UStaticMeshComponent;
class UArrowComponent;
class AProjectile;
class UParticleSystemComponent;
class UAudioComponent;
class UCameraShakeBase;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();

	void Fire();
	void FireSpecial();
	void FireSeries();
	bool IsReadyToFire();
	void Reload();
	void Recharge();
	void TimeRate();
	void AddAmmo(int sum);


	AProjectile* FindProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		ECannonType Cannontype = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		TSubclassOf<AProjectile> PrjectileClass;

	TArray<AProjectile*> PoolProjectile;  // пулл объектов

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int CountAmmunition = 10; // учёт количества снарядов

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int SumCountAmmunition = 10; // общее колличество снарядов

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystemComponent* ShotEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UAudioComponent* AudioEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<UCameraShakeBase> CameraShake;

	void PoolInitial();

	bool bReadyToFire = true;
	FTimerHandle ReloadTimer;
	FTimerHandle SeriesAmmoTimer;
	int countAmmo;
	void FireProjectile();
	void FireTrace();

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
