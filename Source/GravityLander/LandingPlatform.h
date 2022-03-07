// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandingPlatform.generated.h"

UENUM(BlueprintType)
enum class EPlatformType : uint8 {
	EPT_Start UMETA(DisplayName = "Start"),
	EPT_Finish UMETA(DisplayName = "Finish"),

	EPT_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class GRAVITYLANDER_API ALandingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandingPlatform();

	/**
	* @brief Root Box component
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box")
	class UBoxComponent* BoxComponent;

	/**
	* @brief Mesh component
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;

	/**
	* @brief Mesh component
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform Type")
	EPlatformType PlatformType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Size")
	float Size;

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

	void SetPlatformType(EPlatformType PlatformType);
};
