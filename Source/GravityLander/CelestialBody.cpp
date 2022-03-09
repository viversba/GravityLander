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
#include "Collectible.h"
#include "Booster.h"
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

ABooster* ACelestialBody::SpawnCollectible(const FVector& Location, const FRotator& Rotator) {
	
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;

	if (World) {

		ABooster* Collectible = World->SpawnActor<ABooster>(ABooster::StaticClass(), Location, Rotator, SpawnParams);

		FVector NormStartPlatformPos = (Location - GetActorLocation());
		NormStartPlatformPos.Normalize();
		NormStartPlatformPos.X = 0.f;

		FVector Start = NormStartPlatformPos *  2300.f;

		Collectible->SetActorLocation(Start);

		return Collectible;
	}
	else {
		return nullptr;
	}
}

void ACelestialBody::GameOver() {

	NextLevel();
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

	int32 Angle1, Angle2;

	GetAngles(&Angle1, &Angle2);

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

	InstantiateCollectibles();
}

void ACelestialBody::GetAngles(int32* Angle1, int32* Angle2) {
	
	*Angle1 = FMath::RandRange(0, 360);

	bool before = FMath::RandBool(); // Determnines if the second paltform will be spawned left or right

	int32 Score = Ship->Score;
	if (Score < 2) {
		*Angle2 = before ?
			FMath::RandRange(*Angle1 - PlatformSpawnPadding, *Angle1 - PlatformSpawnPadding - 30) :
			FMath::RandRange(*Angle1 + PlatformSpawnPadding, *Angle1 + PlatformSpawnPadding + 30);
	}
	else if (Score < 10) {
		*Angle2 = before ?
			FMath::RandRange(*Angle1 - PlatformSpawnPadding, *Angle1 - PlatformSpawnPadding - 70) :
			FMath::RandRange(*Angle1 + PlatformSpawnPadding, *Angle1 + PlatformSpawnPadding + 70);
	}
	else if (Score < 15) {
		*Angle2 = before ?
			FMath::RandRange(*Angle1 - PlatformSpawnPadding, *Angle1 - PlatformSpawnPadding - 130) :
			FMath::RandRange(*Angle1 + PlatformSpawnPadding, *Angle1 + PlatformSpawnPadding + 130);
	}
	else {
		*Angle2 = before ?
			FMath::RandRange(*Angle1 - PlatformSpawnPadding, *Angle1 - PlatformSpawnPadding - 180) :
			FMath::RandRange(*Angle1 + PlatformSpawnPadding, *Angle1 + PlatformSpawnPadding + 180);
	}
}

void ACelestialBody::InstantiateCollectibles() {
	
	UWorld* World = GetWorld();
	TArray<AActor*> Boosters;
	UGameplayStatics::GetAllActorsOfClass(World, ABooster::StaticClass(), Boosters);
	for (AActor* Booster : Boosters) {
		Booster->Destroy();
	}
	Collectibles.Empty();

	int32 Angle1, Angle2, Angle3;

	Angle1 = FMath::RandRange(0, 120);
	Angle2 = FMath::RandRange(120, 240);
	Angle3 = FMath::RandRange(240, 360);

	FVector Location1, Location2, Location3;
	FRotator Rotator1, Rotator2, Rotator3;

	GetSpawnPoint(Angle1, Location1, Rotator1);
	GetSpawnPoint(Angle2, Location2, Rotator2);
	GetSpawnPoint(Angle3, Location3, Rotator3);

	ABooster* Collectible1 = SpawnCollectible(Location1, Rotator1);
	ABooster* Collectible2 = SpawnCollectible(Location2, Rotator2);
	ABooster* Collectible3 = SpawnCollectible(Location3, Rotator3);

	if (Collectible1) Collectibles.Add(Collectible1->StaticClass());
	if (Collectible2) Collectibles.Add(Collectible2->StaticClass());
	if (Collectible3) Collectibles.Add(Collectible3->StaticClass());
}