// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialBody.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Ship.h"
#include "LandingPlatform.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleRadius = 2000.f;
	CapsuleHalfHeight = CapsuleRadius + 50.f;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Box Component"));
	SetRootComponent(CapsuleComponent);

	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetCollisionProfileName(TEXT("Actor"));
	CapsuleComponent->SetCapsuleRadius(CapsuleRadius);
	CapsuleComponent->AddLocalRotation(FQuat::MakeFromEuler(FVector(0.f, 90.f, 0.f)));
	CapsuleComponent->SetCapsuleHalfHeight(CapsuleHalfHeight);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	float SphereScale = CapsuleRadius / 50.f;

	if (MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleRadius));
		MeshComponent->SetWorldScale3D(FVector(SphereScale, SphereScale, SphereScale));
	}

	Ship = nullptr;

	Mass = 100;
	GravitationalConstant = 0.00001f;
	BodyMass = Mass * GravitationalConstant;
}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();

	APawn* Pawn= UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	AShip* FoundShip = Cast<AShip>(Pawn);
	if (FoundShip) {
		Ship = FoundShip;

		int32 Angle1 = 135, Angle2 = 244;
		FVector SpawnPos1, SpawnPos2;
		FRotator SpawnRotation1, SpawnRotation2;
		GetSpawnPoint(Angle1, SpawnPos1, SpawnRotation1);
		GetSpawnPoint(Angle2, SpawnPos2, SpawnRotation2);

		ALandingPlatform* StartPlatform = SpawnLandingPlatform(SpawnPos1, SpawnRotation1, 0);
		ALandingPlatform* FinishPlatform = SpawnLandingPlatform(SpawnPos2, SpawnRotation2, 1);

		if (StartPlatform) {
			Ship->SetActorLocation(StartPlatform->GetActorLocation() + FVector(0.f, 100.f, 100.f));

		}
	}
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Ship) {
		FVector A = GetActorLocation();
		FVector B = Ship->GetActorLocation();
		FVector Distance = (A - B);
		Distance.Normalize();
		FVector CurrentShipVelocity = Ship->GetCurrentAddedVelocity();
		CurrentShipVelocity -= (Distance * BodyMass * (1/FMath::Pow(Distance.Size(), 2)));
		Ship->SetCurrentAddedVelocity(CurrentShipVelocity);
	}

}

void ACelestialBody::GetSpawnPoint(int32 Angle, FVector &Position, FRotator &Rotator){

	FVector CelestialBodyLocation = GetActorLocation();
	float angleRad = (Angle* PI)/180;
	float YPosition, ZPosition;
	FMath::SinCos(&YPosition, &ZPosition, angleRad);

	Position = FVector(0.f, CapsuleRadius * ZPosition * 2, CapsuleRadius * YPosition * 2) + CelestialBodyLocation;
	Rotator = FRotator::MakeFromEuler(FVector(Angle, 0.f, 0.f));
}

ALandingPlatform* ACelestialBody::SpawnLandingPlatform(const FVector& Location, const FRotator& Rotator, int32 PlatformType) {

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	EPlatformType Type = PlatformType == 0 ? EPlatformType::EPT_Start : EPlatformType::EPT_Finish;

	if (World) {
		ALandingPlatform* Platform = World->SpawnActor<ALandingPlatform>(ALandingPlatform::StaticClass(), Location, Rotator, SpawnParams);
		Platform->SetPlatformType(Type);
		return Platform;
	}
	else {
		return nullptr;
	}
}
