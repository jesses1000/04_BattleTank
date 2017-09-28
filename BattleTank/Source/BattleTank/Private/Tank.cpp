// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Projectile.h"
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

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankAimingComponent = FindComponentByClass < UTankAimingComponent >();
}



void ATank::AimAt(FVector HitLocation) 
{
	if (!ensure(TankAimingComponent)) return;
	TankAimingComponent->AimAt( HitLocation, LaunchSpeed );

}


void ATank::Fire() 
{
	if (!ensure(Barrel)) return;

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (isReloaded) 
	{
		// Spawn a projectile at the socket location of the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);

		// reset reload time
		LastFireTime = FPlatformTime::Seconds();
	}
}



