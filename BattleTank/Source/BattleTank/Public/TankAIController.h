// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"




UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup") // how close the AI tank can get to the player tank
	float AcceptanceRadius = 8000.f;


private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
