//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);


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

	UPROPERTY(BlueprintAssignable)
    FOnDeathSignature OnDeath;

protected:
     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
     float MaxHealth = 100.0f;

	virtual void BeginPlay() override;


private:
        float Health = 0.0f;
		
};
