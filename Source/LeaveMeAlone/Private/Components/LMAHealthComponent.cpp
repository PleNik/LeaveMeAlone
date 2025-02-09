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
  if (IsDead())
    return;

  Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
  OnHealthChanged.Broadcast(Health);

  if (IsDead()) 
  {
    OnDeath.Broadcast();
  }
}

bool ULMAHealthComponent::IsDead() const 
{ 
   return Health <= 0.0f; 
}


void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

	AActor* OwnerComponent = GetOwner();
        if (OwnerComponent) 
        {
          OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
        }
	
}

