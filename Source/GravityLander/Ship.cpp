// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CelestialBody.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ShipPawnMovementComponent.h"
#include "UObject/NameTypes.h"
#include "LandingPlatform.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(false);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	CameraSpringArm->TargetArmLength = 800.f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());

	BottomCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Bottom Collider"));
	BottomCollider->SetupAttachment(GetRootComponent());
	BottomCollider->SetCollisionProfileName(TEXT("Pawn"));
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetEnableGravity(false);

	CelestialBody = nullptr;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentAddedForce = FVector(0.f, 0.f, 0.f);

	Fuel = 100.f;
	MaxFuel = 100.f;
	FuelDrainRate = 10.f;
	BoostStatus = EBoostStatus::EBS_Normal;
	Score = 0;
	bSpaceKeyPressed = false;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
	BottomCollider->OnComponentBeginOverlap.AddDynamic(this, &AShip::OnOverlapBeginBottomBox);

	UWorld* World = GetWorld();

	AActor* Actor = UGameplayStatics::GetActorOfClass(World, ACelestialBody::StaticClass());
	if (Actor) {
		ACelestialBody* Body = Cast<ACelestialBody>(Actor);
		if (Body) {
			CelestialBody = Body;
		}
	}
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FName name;
	BoxComponent->AddForce(-CurrentAddedForce, name, true);

	if (bSpaceKeyPressed) {
		
		float DeltaFuel = FuelDrainRate * DeltaTime;
		if (Fuel - DeltaFuel >= 0) {
			Fuel -= DeltaFuel;
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

	if (value != 0.0f) {
		FRotator Rotator = GetActorRotation();
		Rotator.Roll += value;
		SetActorRotation(Rotator);
	}
}

void AShip::Boost(float value) {

	if (value != 0.0f && Fuel > 0.05f) {
		
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

void AShip::OnOverlapBeginBottomBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		ALandingPlatform* Platform = Cast<ALandingPlatform>(OtherActor);
		if (Platform) {
			if (Platform->PlatformType == EPlatformType::EPT_Finish) {
				Score += 1;
				CelestialBody->NextLevel();
				UE_LOG(LogTemp, Warning, TEXT("%d"), Score);
			}
		}
	}
}