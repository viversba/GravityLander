// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ShipPawnMovementComponent.h"
#include "UObject/NameTypes.h"

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

	Fuel = 100.f;
	MaxFuel = 100.f;
	FuelDrainRate = 10.f;
	BoostStatus = EBoostStatus::EBS_Normal;
	score = 0;
	bSpaceKeyPressed = false;
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

	FName name;
	BoxComponent->AddForce(-CurrentAddedVelocity, name, true);

	if (bSpaceKeyPressed) {
		
		float DeltaFuel = FuelDrainRate * DeltaTime;
		if (Fuel - DeltaFuel >= 0) {
			Fuel -= DeltaFuel;

		UE_LOG(LogTemp, Warning, TEXT("%f"), Fuel);
		}
	}
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AShip::SpaceKeyDown);
	PlayerInputComponent->BindAction("Boost", IE_Released, this, &AShip::SpaceKeyUp);

	PlayerInputComponent->BindAxis("RotateRight", this, &AShip::RotateRight);
	PlayerInputComponent->BindAxis("Boost", this, &AShip::Boost);
}

void AShip::RotateRight(float value) {

	if (MovementComponent != nullptr && value != 0.0f) {
		BoxComponent->SetAllPhysicsAngularVelocityInRadians(FVector(value*0.01f, 0.f, 0.f), true);
	}
}

void AShip::Boost(float value) {

	if (MovementComponent != nullptr && value != 0.0f && Fuel > 0.05f) {
		
		FVector Up = GetActorUpVector() * 10000;
		BoxComponent->AddForce(Up);
	}
}

void AShip::SpaceKeyDown() {
	
	bSpaceKeyPressed = true;
}

void AShip::SpaceKeyUp() {
	
	bSpaceKeyPressed = false;
}

UPawnMovementComponent* AShip::GetMovementComponent() const {
	return MovementComponent;
}