//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FEmptyClipSignature)


class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoWeapon 
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Bullets;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Clips;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  bool Infinite;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ALMABaseWeapon();

	FEmptyClipSignature EmptyClip;

	void Fire();
    void StopFire();
    void ChangeClip();

	bool IsCurrentClipEmpty() const;
    bool IsCurrentClipFull() const;

	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

	FTimerHandle ShootTimer;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName MuzzleSocket = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoWeapon DefaultAmmoWeapon{30, 0, true};

	virtual void BeginPlay() override;

	void Shoot();

	void DecrementBullets();
   

private:
    FAmmoWeapon CurrentAmmoWeapon;

	bool Shooting = false;

	bool CanShooting();

};
