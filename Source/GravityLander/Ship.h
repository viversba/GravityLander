// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

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

	FORCEINLINE UCameraComponent* GetCameraComponent() { return PlayerCamera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera) { PlayerCamera = InCamera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return CameraSpringArm; }
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArm) { CameraSpringArm = InSpringArm; }

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* InMesh) { MeshComponent = InMesh; }
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() { return CapsuleComponent; }
	FORCEINLINE void SetCapsuleComponent(UCapsuleComponent* InCapsule) { CapsuleComponent = InCapsule; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float value);

	void MoveRight(float value);

	void RotateRight(float value);

	void Boost(float value);
};
