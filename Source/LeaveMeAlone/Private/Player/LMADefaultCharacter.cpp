//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Player/LMADefaultCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "Components/LMAWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Player/LMAPlayerController.h>
#include <BaseHUD.h>
#include "Engine/Engine.h"

ALMADefaultCharacter::ALMADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SetUsingAbsoluteRotation(true);
    SpringArmComponent->TargetArmLength = ArmLength;
    SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->bEnableCameraLag = true; 

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetFieldOfView(FOV);
    CameraComponent->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

    WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

}

void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

     if (CursorMaterial)
     {
        CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
     }

     OnHealthChanged(HealthComponent->GetHealth());
     HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
     HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

     Stamina = MaxStamina;

     RegenStamina();
}

void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

   if (!(HealthComponent->IsDead()))
   {
          RotationPlayerOnCursor();
   }

   if (IsSprinting && Stamina != 0.0f) {
     DrainStamina();
   } else {
     if (!IsSprinting && Stamina != 100.0f) {
       RegenStamina();
     }
   }

   if (FMath::IsNearlyZero(Stamina)) {
     StopSprint();
   }
}

void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

    PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ALMADefaultCharacter::CameraZoomIn);
    PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ALMADefaultCharacter::CameraZoomOut);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::MoveForward(float Value) 
{
  AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) 
{
  AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::CameraZoomIn() 
{
  ArmLength -= Fluency;

  if (ArmLength < ArmLengthMin)
    ArmLength = ArmLengthMin;

  SpringArmComponent->TargetArmLength = ArmLength;
}

void ALMADefaultCharacter::CameraZoomOut() 
{
  ArmLength += Fluency;

  if (ArmLength > ArmLengthMax)
    ArmLength = ArmLengthMax;

  SpringArmComponent->TargetArmLength = ArmLength;
}

void ALMADefaultCharacter::OnDeath() 
{
  CurrentCursor->DestroyRenderState_Concurrent();

  auto PlayerController = Cast<ALMAPlayerController>(Controller);

  if (PlayerController) 
  {
    auto HUD = Cast<ABaseHUD>(PlayerController->GetHUD());
    if (HUD)
    {
        for (auto Widget : HUD->WidgetsContainer)
        {
            Widget->RemoveFromParent();
        }
    }
  }

  PlayAnimMontage(DeathMontage);
  GetCharacterMovement()->DisableMovement();
  SetLifeSpan(5.0f);

  if (Controller) 
  {
    Controller->ChangeState(NAME_Spectating);
  }
}


void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (PC) {
    FHitResult ResultHit;
    PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
    float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(
                                     GetActorLocation(), ResultHit.Location)
                                     .Yaw;
    SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
    if (CurrentCursor) {
      CurrentCursor->SetWorldLocation(ResultHit.Location);
    }
  }
}

void ALMADefaultCharacter::StartSprint() 
{
  IsSprinting = true;
  GetCharacterMovement()->MaxWalkSpeed = 600.0f;

  DrainStamina();
}

void ALMADefaultCharacter::StopSprint() 
{
  IsSprinting = false;
  GetCharacterMovement()->MaxWalkSpeed = 300.0f;

  RegenStamina();
}

void ALMADefaultCharacter::DrainStamina() 
{
  Stamina = FMath::Clamp(Stamina - DamageStamina, 0.0f, MaxStamina);

  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Drain Stamina %f"), Stamina));
}

void ALMADefaultCharacter::RegenStamina() 
{
  Stamina = FMath::Clamp(Stamina + DamageStamina, 0.0f, MaxStamina);

  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("Regen Stamina %f"), Stamina));
}

