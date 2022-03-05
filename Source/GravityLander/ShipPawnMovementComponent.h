// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ShipPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYLANDER_API UShipPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	UShipPawnMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
