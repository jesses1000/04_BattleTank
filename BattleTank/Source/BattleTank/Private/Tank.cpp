// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Engine/World.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// REPLACED with BlueprintSpawnableComponent UCLASS parameter in relevant class(es)
	//TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component")); 
	//TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));

}


float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	auto DamageToApply = FMath::Clamp<int32>(DamagePoints, 0.f, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Tank is dead"));
	}

	UE_LOG(LogTemp, Warning, TEXT("DamageAmount=%f, DamageToApply=%i"),DamageAmount, DamageToApply);
	return DamageToApply;
}



float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}









