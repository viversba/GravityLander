// Fill out your copyright notice in the Description page of Project Settings.


#include "Booster.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Ship.h"

ABooster::ABooster() {

	BoostAmount = 0.8f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	if (MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
		MeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> CollectibleMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Basic_Wall_Blue.M_Basic_Wall_Blue'"));

	if (CollectibleMaterial.Succeeded()) {
		MeshComponent->SetMaterial(0, CollectibleMaterial.Object);
	}
}

void ABooster::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABooster::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ABooster::OnOverlapEnd);
}


void ABooster::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor) {
		AShip* Ship = Cast<AShip>(OtherActor);
		if (Ship) {
			Ship->AddedSpeed += BoostAmount;
			Destroy();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlap Beggin"));
}

void ABooster::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
}

void ABooster::DestroyObject() {
	Destroy();
}
