// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UPrimitiveComponent;
class USphereComponent;


UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(AProjectile, FOnDieScore, int);
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Start();
	bool isAvaivable() { return bAvailable; }
	void deAvailable();
	void Stop();
	void setLocal(FVector &start);

	FOnDieScore OnDieScore;

	void AddScore(int score);

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* ProjectileMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float lifeTime = 10.0f; // время жизни

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ExplodeRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bExplode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		float PushForce = 1000.0f;

	FTimerHandle MovementTimer;
	FTimerHandle DisableTimer;
	FVector startLocation;	// позиция в пуле снарядов

	UFUNCTION()
		virtual void Move();

	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Explode();
	void DamageAndRepulsion(AActor* OtherActor);

private:
	bool bAvailable = true;
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
