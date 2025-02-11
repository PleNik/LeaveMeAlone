//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAWeaponComponent.h"
//#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void ULMAWeaponComponent::Fire()
{
  if (Weapon) {
    Weapon->Fire();
  }
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    SpawnWeapon();
}

void ULMAWeaponComponent::SpawnWeapon()
{
  //if (!GetWorld())
    //return;
    Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
    if (Weapon)
    {
       const auto Character = Cast<ACharacter>(GetOwner());
       if (Character)
       {
         FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
         Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponSocket);
       }
    }
}

