// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ShipPawnMovementComponent.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	CameraSpringArm->TargetArmLength = 800.f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());

	MovementComponent = CreateDefaultSubobject<UShipPawnMovementComponent>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentAddedVelocity = FVector(0.f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BoxComponent->SetAllPhysicsLinearVelocity(-CurrentAddedVelocity, true);
	//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), CurrentAddedVelocity.X, CurrentAddedVelocity.Y, CurrentAddedVelocity.Z);
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("RotateRight", this, &AShip::RotateRight);
	PlayerInputComponent->BindAxis("Boost", this, &AShip::Boost);
}

void AShip::RotateRight(float value) {

	if (MovementComponent != nullptr && value != 0.0f) {
		BoxComponent->SetAllPhysicsAngularVelocityInRadians(FVector(value*0.01f, 0.f, 0.f), true);
	}
}

void AShip::Boost(float value) {

	if (MovementComponent != nullptr && value != 0.0f) {
		
		FVector Up = GetActorUpVector();
		//FVector AddedVelocity = Up + CurrentAddedVelocity;
		BoxComponent->SetAllPhysicsLinearVelocity(Up, true);
	}
}

UPawnMovementComponent* AShip::GetMovementComponent() const {
	return MovementComponent;
}