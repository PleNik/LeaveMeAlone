//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Weapon/LMABaseWeapon.h"

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    SetRootComponent(WeaponMesh);

}

void ALMABaseWeapon::Fire()
{ 
	Shoot(); 
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALMABaseWeapon::Shoot()
{
  const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocket);
  const FVector TraceStart = SocketTransform.GetLocation();
  const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
  const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
  DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

  if (HitResult.bBlockingHit)
  {
    DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
  }
}
