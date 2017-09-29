// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"



// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}


void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds(); // so that first fire is after initial reload
}


void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	} 
	else if (IsBarrelMoving()) 
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}


bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) return false;
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection,0.01);

}


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	 
	if( bHaveAimSolution )
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards( AimDirection );
	}

}


void UTankAimingComponent::MoveBarrelTowards( FVector AimDirection )
{
	if (!ensure(Barrel && Turret)) return;

	// work out difference between current barrel rotation and aimdirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	// Move the barrel the right amount this frame given the max elev speed and frame time
	Barrel->Elevate(DeltaRotator.Pitch);

	Turret->Rotate(DeltaRotator.Yaw);

}


void UTankAimingComponent::Fire()
{
	if (FiringState != EFiringState::Reloading)
	{
		// Spawn a projectile at the socket location of the barrel
		if (!ensure(Barrel && ProjectileBlueprint)) return;
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