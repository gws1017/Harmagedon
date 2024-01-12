#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "PlayerCharacter.generated.h"

//����� ���漱�� �� ��
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;

class ABasicPlayerController;
class AWeapon;

struct FInputActionValue;

UENUM(BlueprintType)
enum class EWeaponEquipped : uint8
{
	EWE_None,
	EWE_Fist UMETA(DisplayName = "Default"),
	EWE_Sword UMETA(DisplayName = "Sword"),
	EWE_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EWE_None,
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Move UMETA(DisplayName = "Move"),
	EMS_Hit UMETA(DisplayName = "Hit"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_Roll UMETA(DisplayName = "Roll"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTH_API APlayerCharacter : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

//ACharacter Ŭ���� �⺻ ��� �Լ�
//Ŭ���� ���� �� ��ӵǴ� �⺻�Լ� ������� �ʴ´ٸ� �����ֱ�
protected:
	
	virtual void BeginPlay() override;

public:	
	
	//virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	//�ܺο��� ȣ��Ǵ� �Լ� �ۼ�
	//Getter
	FORCEINLINE virtual AWeapon* GetWeapon() const override { return WeaponInstance; }
	FORCEINLINE EWeaponEquipped GetWeaponEquipped() const { return WeaponEquipped; }
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }

	//Setter
	FORCEINLINE void SetMovementState(const EMovementState& state) {  MovementState = state; }
	FORCEINLINE void SetMovementNormal() {  MovementState = EMovementState::EMS_Normal; }

	void End_Attack();

	//�ܺο��� ������ �� �ִ� ���� �ۼ�(�ǵ����̸� ������ private�� �ۼ��ϰ� Getter Setter �̿��� ��)

private:

	//ĳ���� ���ο����� ȣ��Ǵ� �Լ� �ۼ� (�ַ� Ű�Է�)

	//Ű�Է� ���� �Լ�
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void OnRunning();
	void OffRunning();
	void Roll();
	void EquipWeapon();
	
	void Attack();

	bool CanRoll();
	bool CanAttack();

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
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* EquipAction;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* RollMontage;


	UPROPERTY(VisibleAnywhere, Category = "Montage | Attack")
		bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, Category = "Enums")
		EWeaponEquipped WeaponEquipped;
	UPROPERTY(VisibleAnywhere, Category = "Enums")
		EMovementState MovementState;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* WeaponInstance;

	UPROPERTY(VisibleDefaultsOnly, Category = "Controller")
		ABasicPlayerController* PlayerController;

};
