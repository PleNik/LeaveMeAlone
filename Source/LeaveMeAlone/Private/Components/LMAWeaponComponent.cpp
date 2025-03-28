//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void ULMAWeaponComponent::Fire()
{
  if (Weapon && !AnimReloading)
  {
    Weapon->Fire();
  }
}

void ULMAWeaponComponent::StopFire() 
{ 
    Weapon->StopFire();
}

void ULMAWeaponComponent::Reload()
{
    AutoReload(); 
}

void ULMAWeaponComponent::AutoReload() 
{
  if (!CanReload())
    return;
  Weapon->ChangeClip();
  AnimReloading = true;
  ACharacter *Character = Cast<ACharacter>(GetOwner());
  Character->PlayAnimMontage(ReloadMontage);
}

bool ULMAWeaponComponent::GetCurrentWeaponAmmmo(FAmmoWeapon &AmmoWeapon) const 
{
    if (Weapon)
    {
      AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
      return true;
    }
    return false;
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    SpawnWeapon();
    InitAnimNotify();
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

    Weapon->EmptyClip.AddUObject(this, &ULMAWeaponComponent::EmptyClipCallback);
}

void ULMAWeaponComponent::InitAnimNotify()
{
  if (!ReloadMontage)
    return;

  const auto NotifiesEvents = ReloadMontage->Notifies;
  for (auto NotifyEvent : NotifiesEvents) {
    auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
    if (ReloadFinish) 
    {
      ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
      break;
    }
  }
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent *SkeletalMesh)
{
  const auto Character = Cast<ACharacter>(GetOwner());
  if (Character->GetMesh() == SkeletalMesh)
  {
    AnimReloading = false;
  }
}

bool ULMAWeaponComponent::CanReload() const
{ 
    return !(AnimReloading || Weapon->IsCurrentClipFull());
}

void ULMAWeaponComponent::EmptyClipCallback()
{ 
    AutoReload();
}

