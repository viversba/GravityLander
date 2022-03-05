// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"

UCLASS()
class GRAVITYLANDER_API ACelestialBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialBody();

	/**
	* @brief Mesh component to be used by the ship
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;

	/**
	* @brief Root component of the ship
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UCapsuleComponent* CapsuleComponent;

	/**
	* @brief Capsule component radius
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule")
	float CapsuleRadius;

	/**
	* @brief Capsule half height
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule")
	float CapsuleHalfHeight;

	/**
	* @brief Sphere that will influence gravitational behaviour of the ship
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity")
	class USphereComponent* GravitySphere;

	/**
	* @brief Capsule half height
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
	float Mass;

	/**
	* @brief Gravitational constant
	*/
	UPROPERTY()
	float GravitationalConstant;

	/**
	* @brief Boolean that determines if the celestial body is overlapping with the sip
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gravity")
	bool bOverlappingShip;

	/**
	* @brief Reference to the ship
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ship")
	class AShip* Ship;

private:

	/**
	* @brief Body mass calculated with the Mass parameter and the gravitational constant
	*/
	UPROPERTY()
	float BodyMass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};