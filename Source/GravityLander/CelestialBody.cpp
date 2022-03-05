// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialBody.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Ship.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleRadius = 100.f;
	CapsuleHalfHeight = CapsuleRadius + 50.f;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Box Component"));
	SetRootComponent(CapsuleComponent);

	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetCollisionProfileName(TEXT("Actor"));
	CapsuleComponent->SetCapsuleRadius(CapsuleRadius);
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

	GravitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	GravitySphere->SetupAttachment(GetRootComponent());
	GravitySphere->InitSphereRadius(CapsuleRadius * 4.f);

	Ship = nullptr;

	Mass = 1000;
	GravitationalConstant = 0.000001f;
	BodyMass = Mass * GravitationalConstant;
}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	
	GravitySphere->OnComponentBeginOverlap.AddDynamic(this, &ACelestialBody::OnOverlapBegin);
	GravitySphere->OnComponentEndOverlap.AddDynamic(this, &ACelestialBody::OnOverlapEnd);
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOverlappingShip) {
		FVector A = GetActorLocation();
		FVector B = Ship->GetActorLocation();
		FVector Velocity = (A - B);
		Velocity.Normalize();
		FVector CurrentShipVelocity = Ship->GetCurrentAddedVelocity();
		CurrentShipVelocity += (Velocity * BodyMass);
		Ship->SetCurrentAddedVelocity(CurrentShipVelocity);
	}

}

void ACelestialBody::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){

	if (OtherActor) {
		AShip* OverlappedShip = Cast<AShip>(OtherActor);
		if (OverlappedShip) {
			UE_LOG(LogTemp, Warning, TEXT("Super:: OnOverlapBegin"));
			Ship = OverlappedShip;
			bOverlappingShip = true;
		}
	}
}

void ACelestialBody::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){

	if (OtherActor) {
		AShip* OverlappedShip = Cast<AShip>(OtherActor);
		if (OverlappedShip) {
			UE_LOG(LogTemp, Warning, TEXT("Super:: OnOverlapEnd"));
			Ship = nullptr;
			bOverlappingShip = false;
		}
	}
}

