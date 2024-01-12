#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "PlayerCharacter.generated.h"

//헤더는 전방선언 할 것
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

class ABasicPlayerController;

struct FInputActionValue;

UENUM(BlueprintType)
enum class EWeaponEquipped : uint8
{
	EWE_None,
	EWE_Fist UMETA(DisplayName = "Default"),
	EWE_Sword UMETA(DisplayName = "Sword"),
	EWE_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTH_API APlayerCharacter : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

//ACharacter 클래스 기본 상속 함수
//클래스 만들 때 상속되는 기본함수 사용하지 않는다면 지워주기
protected:
	
	virtual void BeginPlay() override;

public:	
	
	//virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	//외부에서 호출되는 함수 작성
	//Getter
	FORCEINLINE EWeaponEquipped GetWeaponEquipped() const { return WeaponEquipped; }
	FORCEINLINE bool IsRolling() const { return bIsRolling; }

	//Setter
	FORCEINLINE void SetIsRolling(const bool value) { bIsRolling = value; }

	//외부에서 접근할 수 있는 변수 작성(되도록이면 변수는 private에 작성하고 Getter Setter 이용할 것)

private:

	//캐릭터 내부에서만 호출되는 함수 작성 (주로 키입력)

	//키입력 관련 함수
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void OnRunning();
	void OffRunning();
	void Roll();

	void Attack();

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* IMCPlayer;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* MovementAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* RollAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* HitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RollMontage;


	UPROPERTY(VisibleAnywhere, Category = "Montage | Roll")
		bool bIsRolling = false;
	UPROPERTY(VisibleAnywhere, Category = "Montage | Attack")
		bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, Category = "Enums")
		EWeaponEquipped WeaponEquipped;

	UPROPERTY(VisibleDefaultsOnly, Category = "Controller")
		ABasicPlayerController* PlayerController;

};
