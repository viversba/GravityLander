// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

UENUM(BlueprintType)
enum class EBoostStatus : uint8 {
	EBS_Normal UMETA(DisplayName = "Normal"),
	EBS_Pressed UMETA(DisplayName = "Pressed"),

	EBS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class GRAVITYLANDER_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

	/**
	* @brief Mesh component to be used by the ship
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class UStaticMeshComponent* MeshComponent;

	/**
	* @brief Root component of the ship
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class UCapsuleComponent* CapsuleComponent;

	/**
	* @brief Root component of the ship
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class UBoxComponent* BoxComponent;

	/**
	* @brief For positioning the camera behind of the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraSpringArm;

	/**
	* @brief For positioning the camera behind of the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	/**
	* @brief Movement Component
	*/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UPawnMovementComponent* MovementComponent;

	/**
	* @brief Velocity component that accounts for gravitational influence of celestial bodies
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector CurrentAddedVelocity;

	UPROPERTY(BlueprintReadWrite, Category = "Ship Stats")
	float Fuel;

	UPROPERTY(BlueprintReadOnly, Category = "Ship Stats")
	float MaxFuel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship Stats")
	float FuelDrainRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ship Stats")
	EBoostStatus BoostStatus;

	UPROPERTY(BlueprintReadWrite, Category = "Ship Stats")
	int32 score;

	bool bSpaceKeyPressed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void RotateRight(float value);

	void Boost(float value);

	void SpaceKeyDown();

	void SpaceKeyUp();

	FORCEINLINE UCameraComponent* GetCameraComponent() { return PlayerCamera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera) { PlayerCamera = InCamera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return CameraSpringArm; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArm) { CameraSpringArm = InSpringArm; }

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* InMesh) { MeshComponent = InMesh; }
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() { return CapsuleComponent; }
	FORCEINLINE void SetCapsuleComponent(UCapsuleComponent* InCapsule) { CapsuleComponent = InCapsule; }

	FORCEINLINE FVector GetCurrentAddedVelocity() { return CurrentAddedVelocity; }
	FORCEINLINE void SetCurrentAddedVelocity(FVector Velocity) { CurrentAddedVelocity = Velocity; }
};
