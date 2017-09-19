// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Engine/World.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("PlayController not possessing a tank"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayController possessing: %s"),*(ControlledTank->GetName()));
	}

	
}


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}




ATank* ATankPlayerController::GetControlledTank() const 
{
	return Cast<ATank>(GetPawn());
}



void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) return;

	FVector HitLocation; // Out param

	

	// Get world location of linetrace through crosshair.
	if ( GetSightRayHitLocation(HitLocation) ) { // If it hits the landscape
		UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *(HitLocation.ToString()));
		// TODO: Tell controlled tank to aim at this point
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
		if (GetLookVectorHitLocation(LookDirection, HitLocation))
		{
			return true;
		}
		return false;
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
			ECollisionChannel::ECC_Visibility)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}

	HitLocation = FVector(0.f);
	return false;
}
