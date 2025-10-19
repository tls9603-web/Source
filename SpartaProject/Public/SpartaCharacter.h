#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASpartaCharacter();

    // 현재 체력을 가져오는 함수
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealth() const;

    // 체력을 회복시키는 함수
    UFUNCTION(BlueprintCallable, Category = "Health")
    void AddHealth(float Amount);

protected:
    // --- 기존 변수들 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    // --- 체력 관련 변수 ---
    // 최대 체력
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    // 현재 체력
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;

    // --- 기존 함수들 ---
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    void Move(const FInputActionValue& value);
    void StartJump(const FInputActionValue& value);
    void StopJump(const FInputActionValue& value);
    void Look(const FInputActionValue& value);
    void StartSprint(const FInputActionValue& value);
    void StopSprint(const FInputActionValue& value);

    // --- 체력 관련 함수 ---
    // 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
    UFUNCTION(BlueprintCallable, Category = "Health")
    virtual void OnDeath();

    // 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨 (AActor의 함수를 오버라이드)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
    float NormalSpeed;
    float SprintSpeedMultiplier;
    float SprintSpeed;
};