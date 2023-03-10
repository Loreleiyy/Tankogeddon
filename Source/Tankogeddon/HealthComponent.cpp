// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}

void UHealthComponent::TakeDamage(FDamageData DamageData)
{
	CurrentHealth -= DamageData.DamageValue;

	if (CurrentHealth <= 0) {
		if (OnDie.IsBound()) {
			OnDie.Broadcast();
		}
	}
	else {
		if (OnHealthChanget.IsBound()) {
			OnHealthChanget.Broadcast(DamageData.DamageValue);
		}
	}
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::AddHealth(float newHealth)
{
	CurrentHealth += newHealth;
	if (CurrentHealth > MaxHealth) {
		CurrentHealth = MaxHealth;
	}
}



void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
		
}



//void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//	// ...
//}

