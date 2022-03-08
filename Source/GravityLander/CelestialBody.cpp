// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialBody.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PawnMovementComponent.h"
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

	CurrentStartPlatform = nullptr;
	CurrentFinishPlatform = nullptr;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	float SphereScale = CapsuleRadius / 50.f;

	if (MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleRadius));
		MeshComponent->SetWorldScale3D(FVector(SphereScale, SphereScale, SphereScale));
	}

	Ship = nullptr;

	Mass = 1000000;
	GravitationalConstant = 0.001f;
	PlatformSpawnPadding = 7;
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
		NextLevel();
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
		Ship->BoxComponent->AddForce(Distance * BodyMass * (1 / FMath::Pow(Distance.Size(), 2)));
	}

}

void ACelestialBody::GetSpawnPoint(int32 Angle, FVector &Position, FRotator &Rotator){

	FVector CelestialBodyLocation = GetActorLocation();
	float angleRad = (Angle* PI)/180;
	float YPosition, ZPosition;
	FMath::SinCos(&YPosition, &ZPosition, angleRad);

	Position = FVector(0.f, CapsuleRadius * YPosition, CapsuleRadius * ZPosition) + CelestialBodyLocation;
	Rotator = FRotator::MakeFromEuler(FVector(Angle, 0.f, 0.f));
}

ALandingPlatform* ACelestialBody::SpawnLandingPlatform(const FVector& Location, const FRotator& Rotator, int32 PlatformType) {

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	EPlatformType Type = PlatformType == 0 ? EPlatformType::EPT_Start : EPlatformType::EPT_Finish;

	if (World) {
		ALandingPlatform* Platform = World->SpawnActor<ALandingPlatform>(ALandingPlatform::StaticClass(), Location, Rotator, SpawnParams);
		Platform->SetPlatformType(Type);
		Platform->SetMaterial();
		return Platform;
	}
	else {
		return nullptr;
	}
}

void ACelestialBody::GameOver() {

}

void ACelestialBody::NextLevel() {

	if (CurrentStartPlatform) {
		CurrentStartPlatform->Destroy();
		CurrentStartPlatform = nullptr;
	}
	if (CurrentFinishPlatform) {
		CurrentFinishPlatform->Destroy();
		CurrentFinishPlatform = nullptr;
	}

	int32 Angle1 = 15, Angle2 = 1;

	Angle1 = FMath::RandRange(0,360);

	bool before = FMath::RandBool(); // Determnines if the second paltform will be spawned left or right

	int32 Score = Ship->Score;
	if (Score < 2) {
		Angle2 = before ?
			FMath::RandRange(Angle1 - PlatformSpawnPadding, Angle1 - PlatformSpawnPadding - 30) :
			FMath::RandRange(Angle1 + PlatformSpawnPadding, Angle1 + PlatformSpawnPadding + 30);
	}
	else if (Score < 10) {
		Angle2 = before ?
			FMath::RandRange(Angle1 - PlatformSpawnPadding, Angle1 - PlatformSpawnPadding - 70) :
			FMath::RandRange(Angle1 + PlatformSpawnPadding, Angle1 + PlatformSpawnPadding + 70);
	}
	else if (Score < 15) {
		Angle2 = before ?
			FMath::RandRange(Angle1 - PlatformSpawnPadding, Angle1 - PlatformSpawnPadding - 130) :
			FMath::RandRange(Angle1 + PlatformSpawnPadding, Angle1 + PlatformSpawnPadding + 130);
	}
	else {
		Angle2 = before ?
			FMath::RandRange(Angle1 - PlatformSpawnPadding, Angle1 - PlatformSpawnPadding - 180) :
			FMath::RandRange(Angle1 + PlatformSpawnPadding, Angle1 + PlatformSpawnPadding + 180);
	}


	FVector SpawnPos1, SpawnPos2;
	FRotator SpawnRotation1, SpawnRotation2;
	GetSpawnPoint(Angle1, SpawnPos1, SpawnRotation1);
	GetSpawnPoint(Angle2, SpawnPos2, SpawnRotation2);

	ALandingPlatform* StartPlatform = SpawnLandingPlatform(SpawnPos1, SpawnRotation1, 0);
	ALandingPlatform* FinishPlatform = SpawnLandingPlatform(SpawnPos2, SpawnRotation2, 1);

	if (StartPlatform) {

		FVector NormStartPlatformPos = (SpawnPos1 - GetActorLocation());
		NormStartPlatformPos.Normalize();
		NormStartPlatformPos.X = 0.f;

		FVector Start = NormStartPlatformPos * (StartPlatform->Size * 82.f);
		NormStartPlatformPos *= (StartPlatform->Size * 100.f);

		Ship->SetActorLocation(Start);
		Ship->SetActorRotation(SpawnRotation1);

		CurrentStartPlatform = StartPlatform;
		CurrentFinishPlatform = FinishPlatform;
	}
}
