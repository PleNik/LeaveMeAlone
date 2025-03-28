//  LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;
class ULMAWeaponComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALMADefaultCharacter();

    UFUNCTION()
    ULMAHealthComponent *GetHealthComponent() const 
    {
          return HealthComponent;
    } 

    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float NewHealth);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent *SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    ULMAWeaponComponent *WeaponComponent;

    UPROPERTY()
    UDecalComponent *CurrentCursor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
    UMaterialInterface *CursorMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
    FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Zoom",
              meta = (ClampMin = "800.0", ClampMax = "1200.0"))
    float ArmLengthMin = 1000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Zoom",
              meta = (ClampMin = "1400.0", ClampMax = "1800.0"))
    float ArmLengthMax = 1800.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera|Zoom",
              meta = (ClampMin = "10.0", ClampMax = "100.0"))
    float Fluency = 25.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
    ULMAHealthComponent *HealthComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint")
    bool IsSprinting;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    float MaxStamina = 100.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    float DamageStamina = 10.0f;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);

    void CameraZoomIn();
    void CameraZoomOut();

    void OnDeath();

    //void OnHealthChanged(float NewHealth);
    void RotationPlayerOnCursor();

    void StartSprint();
    void StopSprint();
    void DrainStamina();
    void RegenStamina();

    float YRotation = -75.0f;
    float ArmLength = 1400.0f;
    float FOV = 55.0f;

    float Stamina;

};
