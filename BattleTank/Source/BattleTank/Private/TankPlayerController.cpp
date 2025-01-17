// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	FoundAimingComponent(AimingComponent);

}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) return;

		// subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);

	}
}


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}



void ATankPlayerController::AimTowardsCrosshair() 
{
	if (!GetPawn()) return;
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;

	FVector HitLocation; // Out param

	// Get world location of linetrace through crosshair.
	if (GetSightRayHitLocation(HitLocation)) { // If it hits the landscape
		AimingComponent->AimAt(HitLocation);
	}

	
}


bool ATankPlayerController::GetSightRayHitLocation( FVector& HitLocation ) const
{
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D( ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation );

	// De-project the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Linetrace along that look direction, and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, HitLocation);

	}
	return false;
}


bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		CameraWorldLocation, 
		LookDirection
	);
}


bool ATankPlayerController::GetLookVectorHitLocation( FVector LookDirection, FVector& HitLocation ) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + ( LookDirection * LineTraceRange );

	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Camera)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}

	HitLocation = FVector(0.f);
	return false;
}



void ATankPlayerController::OnPossessedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Your Tank is dead"));

	StartSpectatingOnly();
}

