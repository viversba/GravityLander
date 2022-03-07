// Fill out your copyright notice in the Description page of Project Settings.


#include "LandingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALandingPlatform::ALandingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Size = 25.f;
	float Scale = Size / 50.f;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	BoxComponent->SetBoxExtent(FVector(Size, Size, Size));

	MeshComponent= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	if (MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -Size));
		MeshComponent->SetWorldScale3D(FVector(Scale, Scale, Scale));
	}
}

// Called when the game starts or when spawned
void ALandingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALandingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandingPlatform::SetPlatformType(EPlatformType Type) {
	PlatformType = Type;
}

void ALandingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap Beggin"));
}

void ALandingPlatform::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
}

