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

	OutterGravitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	OutterGravitySphere->SetupAttachment(GetRootComponent());
	OutterGravitySphere->InitSphereRadius(CapsuleRadius * 4.f);

	InnerGravitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere Component"));
	InnerGravitySphere->SetupAttachment(GetRootComponent());
	InnerGravitySphere->InitSphereRadius(CapsuleRadius + 5.f);

	Ship = nullptr;

	Mass = 100;
	GravitationalConstant = 0.000001f;
	BodyMass = Mass * GravitationalConstant;
}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
	
	OutterGravitySphere->OnComponentBeginOverlap.AddDynamic(this, &ACelestialBody::OnOverlapBeginOutterSphere);
	OutterGravitySphere->OnComponentEndOverlap.AddDynamic(this, &ACelestialBody::OnOverlapEndOutterSphere);

	InnerGravitySphere->OnComponentBeginOverlap.AddDynamic(this, &ACelestialBody::OnOverlapBeginInnerSphere);
	InnerGravitySphere->OnComponentEndOverlap.AddDynamic(this, &ACelestialBody::OnOverlapEndInnerSphere);
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Ship && bOverlappingShip) {
		FVector A = GetActorLocation();
		FVector B = Ship->GetActorLocation();
		FVector Velocity = (A - B);

		//UE_LOG(LogTemp, Warning, TEXT("VELOCITY IS %f %f %f"), Velocity.X, Velocity.Y, Velocity.Z);
		//UE_LOG(LogTemp, Warning, TEXT("VELOCITY SIZE IS %f"), Velocity.Size());

		Velocity.Normalize();
		FVector CurrentShipVelocity = Ship->GetCurrentAddedVelocity();
		CurrentShipVelocity -= (Velocity * BodyMass);
		Ship->SetCurrentAddedVelocity(CurrentShipVelocity);

		//if (Velocity.Size() > CapsuleRadius + 50.f) {
			
		//}
		//else {
		//	UE_LOG(LogTemp, Warning, TEXT("Close enough"));
		//	Ship->SetCurrentAddedVelocity(FVector(0.f, 0.f, 0.f));
		//}
	}

}

void ACelestialBody::OnOverlapBeginOutterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){

	if (OtherActor) {
		AShip* OverlappedShip = Cast<AShip>(OtherActor);
		if (OverlappedShip) {
			UE_LOG(LogTemp, Warning, TEXT("Outter:: OnOverlapBegin"));
			Ship = OverlappedShip;
			bOverlappingShip = true;
		}
	}
}

void ACelestialBody::OnOverlapEndOutterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){

	if (OtherActor) {
		AShip* OverlappedShip = Cast<AShip>(OtherActor);
		if (OverlappedShip) {
			UE_LOG(LogTemp, Warning, TEXT("Outter:: OnOverlapEnd"));
			OverlappedShip->SetCurrentAddedVelocity(FVector(0.f, 0.f, 0.f));
			Ship = nullptr;
			bOverlappingShip = false;
		}
	}
}


void ACelestialBody::OnOverlapBeginInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (OtherActor) {
		AShip* OverlappedShip = Cast<AShip>(OtherActor);
		if (OverlappedShip) {
			UE_LOG(LogTemp, Warning, TEXT("Inner:: OnOverlapBegin"));
			OverlappedShip->SetCurrentAddedVelocity(FVector(0.f, 0.f, 0.f));
			Ship = nullptr;
			bOverlappingShip = false;
		}
	}
	
}

void ACelestialBody::OnOverlapEndInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	if (OtherActor) {
		AShip* OverlappedShip = Cast<AShip>(OtherActor);
		if (OverlappedShip) {
			UE_LOG(LogTemp, Warning, TEXT("Inner:: OnOverlapEnd"));
			Ship = OverlappedShip;
			bOverlappingShip = true;
		}
	}
}

