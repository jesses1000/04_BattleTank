// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"



void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	//auto ControlledTank = GetControlledTank();


	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("AIController not accessing PlayerTank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController accessing: %s"), *(PlayerTank->GetName()));
	}
}


ATank * ATankAIController::GetPlayerTank() const
{
	//return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) return nullptr;
	return Cast<ATank>(PlayerPawn);
}


ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
