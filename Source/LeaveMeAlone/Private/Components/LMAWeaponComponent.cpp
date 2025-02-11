//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAWeaponComponent.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();


}


void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

