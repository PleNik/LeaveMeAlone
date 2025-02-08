//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "LevelActors/Damage/LMADemageActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ALMADemageActor::ALMADemageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->SetSphereRadius(SphereRadius);
    SetRootComponent(SphereComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetupAttachment(SphereComponent);
    StaticMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
    StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
}

void ALMADemageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALMADemageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

