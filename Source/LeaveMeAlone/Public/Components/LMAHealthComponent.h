//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULMAHealthComponent();

	UFUNCTION(BlueprintCallable)
    float GetHealth() const { return Health; }

	UFUNCTION()
	void OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                         class AController *InstigatedBy, AActor *DamageCauser);

	UFUNCTION(BlueprintCallable)
    bool IsDead() const;

	bool AddHealth(float NewHealth);
    bool IsHealthFull() const;

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

protected:
     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
     float MaxHealth = 100.0f;

	virtual void BeginPlay() override;


private:
        float Health = 0.0f;
		
};
