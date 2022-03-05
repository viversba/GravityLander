// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPawnMovementComponent.h"

UShipPawnMovementComponent::UShipPawnMovementComponent() {
	
	
}

void UShipPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) return;

	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(5.0f);

	if (DesiredMovementThisFrame.IsNearlyZero()) return;

	FHitResult Hit;
	SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
	UpdateComponentVelocity();

	UE_LOG(LogTemp, Warning, TEXT("VELOCITY"));
	UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), Velocity.X, Velocity.Y, Velocity.Z);

	if (Hit.IsValidBlockingHit()) {
		SlideAlongSurface(DesiredMovementThisFrame, 1.f-Hit.Time, Hit.Normal, Hit);
		UE_LOG(LogTemp, Warning, TEXT("Valid Blocking Hit"));
	}
}