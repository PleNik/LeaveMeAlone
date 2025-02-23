//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    SetRootComponent(WeaponMesh);

}

void ALMABaseWeapon::Fire()
{
  if (!CanShooting() || IsCurrentClipEmpty())
    return;
  Shooting = true;
 
  GetWorldTimerManager().SetTimer(ShootTimer, this, &ALMABaseWeapon::Shoot, 0.1f, true, -1.0f);
  // Shoot();
}

void ALMABaseWeapon::StopFire()
{ 
	Shooting = false;
	GetWorldTimerManager().ClearTimer(ShootTimer);
}

void ALMABaseWeapon::BeginPlay() 
{
	Super::BeginPlay();

	CurrentAmmoWeapon = DefaultAmmoWeapon;
}


void ALMABaseWeapon::ChangeClip()
{
  CurrentAmmoWeapon.Bullets = DefaultAmmoWeapon.Bullets;
}


void ALMABaseWeapon::Shoot()
{
  if (IsCurrentClipEmpty())
    return;
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

  if (!IsCurrentClipEmpty ())
    DecrementBullets();
  
}

void ALMABaseWeapon::DecrementBullets()
{
  CurrentAmmoWeapon.Bullets--;
  UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const 
{ 
	return CurrentAmmoWeapon.Bullets <= 0;
}

bool ALMABaseWeapon::CanShooting() 
{ 
	return !Shooting; 
}

