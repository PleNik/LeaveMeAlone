//  LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAHealthComponent.h"

ULMAHealthComponent::ULMAHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void ULMAHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage,
                                          const UDamageType *DamageType,
                                          AController *InstigatedBy,
                                          AActor *DamageCauser) 
{
  Health -= Damage;
}


void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AActor* OwnerComponent = GetOwner();
        if (OwnerComponent) 
        {
          OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
        }
	
}

