// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"


void UTankTrack::SetThrottle(float Throttle) 
{
	UE_LOG(LogTemp, Warning, TEXT("%s s throttle %s"), *GetName(), *(FString::SanitizeFloat(Throttle)));

	// TODO: clamp actual throttle value


	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);

}

