#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/WeaponInterface.h"
#include "Interface/HitInterface.h"
#include "PlayerCharacter.generated.h"

//헤더는 전방선언 할 것
class USpringArmComponent;
class USphereComponent;
class UCameraComponent;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class USceneCaptureComponent2D;

class UInventoryComponent;

class ABasicPlayerController;
class AWeapon;
class APickupItem;
class AExpItem;
class AEnemy;

class IInteractionInterface;

struct FInputActionValue;

enum class EEquipType : uint8;

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
	EMS_Run UMETA(DisplayName = "Run"),
	EMS_Hit UMETA(DisplayName = "Hit"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_Roll UMETA(DisplayName = "Roll"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FPlayerStatus
{
	GENERATED_BODY()

	FPlayerStatus() = default;
	FPlayerStatus(float hp,float Sta, float mp, int32 lev) :
		MaxHP(hp),MaxStamina(Sta),MaxMana(mp),Level(lev)
	{
		HP = MaxHP;
		Stamina = MaxStamina;
		Mana = MaxMana;
	}
	FPlayerStatus(int32 lev, int32 Vit, int32 Str, int32 Eng, int32 Fai, int32 Int, int32 End) :
		 Vitality(Vit), Strength(Str), Energy(Eng), Faith(Fai), Intelligence(Int), Enduarance(End), Level(lev)
	{}

public:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float HP;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxHP;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float Stamina;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxStamina;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float Mana;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxMana;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float PhyDamage;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MagDamage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 BleedResistance;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 PoisonResistance;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 MaxMagicSlot; //마법 슬롯, 지성에 의해 증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxWeight; //최대하중, 체력에 의해 증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float Poise; //강인도, 근력에 의해 증가, 백분율로 강인도 데미지를 경감시켜준다.

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Vitality; //체력 - HP 증가 소지가능 무게증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Strength; //근력 - 물리 공격력, 강인도 소폭 증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Energy; //기력 - 스테미나 증가, 물리 공격력 증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Faith; //신앙 - 마법 공격력, 마나 소폭 증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Intelligence; //지성 - 마나, 마법 슬롯 증가
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Enduarance; //인내 - 강인도, 내성, 방어력 소폭 증가
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Level; //현재 레벨
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Exp; //현재 경험치

};

UCLASS()
class PROJECTH_API APlayerCharacter : public ACharacter, public IWeaponInterface, public IHitInterface
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

//ACharacter 클래스 기본 상속 함수
//클래스 만들 때 상속되는 기본함수 사용하지 않는다면 지워주기
protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;



public:

	UFUNCTION()
		void TargetingBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TargetingEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

	//외부에서 호출되는 함수 작성
	//Getter
	virtual AWeapon* GetWeapon(const EEquipType Type) const override;
	FORCEINLINE AWeapon* GetRightWeapon() const { return RightWeapon; }
	FORCEINLINE AWeapon* GetLeftWeapon() const { return LeftWeapon; }
	FORCEINLINE EWeaponEquipped GetWeaponEquipped() const { return WeaponEquipped; }
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	UFUNCTION(BlueprintCallable)
		 UInventoryComponent* GetInventory() const { return InventoryComponent; }
	
	UFUNCTION(BlueprintCallable)
		 FPlayerStatus GetPlayerStat() const { return Stat; }
	FORCEINLINE bool IsInvincible() { return bIFrame; }
	FORCEINLINE bool IsBlocking() { return bBlocking; }
	FORCEINLINE bool GetParrySucc() { return bParrySucc; }
	FORCEINLINE bool GetParryFail() { return bParryFail; }
	FORCEINLINE float GetHP() const { return Stat.HP; }
	FORCEINLINE float GetMaxHP() const { return Stat.MaxHP; }
	FORCEINLINE float GetStamina() const { return Stat.Stamina; }
	FORCEINLINE float GetMaxStamina() const { return Stat.MaxStamina; }
	FORCEINLINE	float GetStrDamage() { return Stat.PhyDamage; }
	FORCEINLINE int32 GetPlayerLevel() { return Stat.Level; }

	UFUNCTION(BlueprintPure)
		virtual float GetDamage(const EEquipType Type) const;
	UFUNCTION(BlueprintCallable)
		float GetWeaponDamage(const EEquipType Type) const;
	ABasicPlayerController* GetPlayerController();

	//Setter
	FORCEINLINE void SetMovementState(const EMovementState& state) {  MovementState = state; }
	FORCEINLINE void SetMovementNormal() {  MovementState = EMovementState::EMS_Normal; }
	FORCEINLINE void SetOverlappingActor(IInteractionInterface* actor) { OverlappingActor = actor; }
	FORCEINLINE void SetWeaponEquipped(const EWeaponEquipped EquippedType) { WeaponEquipped = EquippedType; }
	FORCEINLINE void SetBlockStaminaRate(const float value) { BlockStaminaRate = value; }
	FORCEINLINE void SetBlockStaminaRegenRate() { StaminaRegenRate *= GuardStaminaDeclineRate; }
	FORCEINLINE void SetInvincible(const bool value) { bIFrame = value; }
	FORCEINLINE void SetParrySucc(const bool value) { bParrySucc = value; }
	FORCEINLINE void SetParryFail(const bool value) { bParryFail = value; }
	FORCEINLINE void SetCanParryed(const bool value) { bCanParry = value; }
	FORCEINLINE void SetBlock(const bool value) { bBlocking = value; }
	FORCEINLINE void SetHP(const float value) { Stat.HP = value; }
	FORCEINLINE void SetStartPoint(const FVector Loc) { StartPoint = Loc; }
	void SetWeapon(EEquipType Type, AWeapon* Instance);
	
public:

	void End_Attack();
	void AttackCombo(const EEquipType Type);
	void ResetAttack();
	void PlayAttackMontage(const EEquipType Type);

	void OnGuard();

	bool Alive();
	void Die();
	virtual void DeathEnd();

	void Equip(const EEquipType Type);
	void UnEquip(const EEquipType Type);

	void IncrementExp(float Amount);
	void LevelUp(const FPlayerStatus& data);

	void DecrementStamina(float Amount);

	bool CanBlock();

	//추후 인터페이스 분리
	virtual void Hit(const FVector& ParticleSpawnLocation);

	UFUNCTION(BlueprintCallable)
		void SaveGameData(int32 SaveType = 0);
	UFUNCTION(BlueprintCallable)
		void LoadGameData();

	void InitStatusInfo();

	void SetCapture(AActor* InActor, const bool bIncludeFromChildActors);
	void RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors);
	//외부에서 접근할 수 있는 변수 작성(되도록이면 변수는 private에 작성하고 Getter Setter 이용할 것)
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* ActiveWeapon;

private:

	//캐릭터 내부에서만 호출되는 함수 작성 (주로 키입력)
	//키입력 관련 함수
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void OnRunning();
	void OffRunning();

	void UseItem();

	void SmoothRoll();
	void Roll();
	void EquipWeapon();
	void Interaction();

	void DetectTarget();
	void LockTarget();
	void UnlockTarget();
	void SwapTargetRight();
	void SwapTargetLeft();

	void LeftClick();
	void RightClick();
	void OffRightClick();
	void RightSpecialClick();

	//다른 액터와 마주보고있는지 검사하는 함수
	bool CheckFace(AActor* OtherActor);
	bool CheckGuard(float& DamageAmount, AActor* DamageCauser);
	bool CheckParry(float& DamageAmount, AActor* DamageCauser);

	bool CanRoll();
	bool CanAttack(EEquipType Type);
	bool CanMove();
	bool CanHit();

	void UpdateStamina(float DeltaStamina);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		USphereComponent* TargetingSphere;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* SceneCapture;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* IMCPlayer;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* MovementAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* RunAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* UseItemAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* RollAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* RightClickAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* RightClickSpecialAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* EquipAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* InteractionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* OpenEquipUIAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* TargetLockAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		UInputAction* EscAction;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DrinkMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ParryMontage;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool bIsAttacking = false;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool bSaveAttack = false;


	UPROPERTY(EditAnywhere, Category = "Attack")
		int32 AttackCount;
	UPROPERTY(EditAnywhere, Category = "Attack")
		int32 NumberOfAttacks;

	UPROPERTY(EditDefaultsOnly, Category = "Enums")
		EWeaponEquipped WeaponEquipped;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Enums", meta = (AllowPrivateAccess = "true"))
		EMovementState MovementState;


	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* RightWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* LeftWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FPlayerStatus Stat;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bBlockFail = false;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bBlocking = false;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bParrySucc = false; //패리 성공했는지 체크하기위한 용도
	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bParryFail = false; //패리 실패시 활성화
	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bCanParry = false;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bIFrame = false;
	UPROPERTY(EditAnywhere, Category = "Status")
		float BlockMinStamina;
	UPROPERTY(EditAnywhere, Category = "Status")
		float BlockStaminaRate;
	UPROPERTY(EditDefaultsOnly, Category = "Status")
		float GuardStaminaDeclineRate;
	UPROPERTY(EditAnywhere, Category = "Status")
		float StaminaRegenRate;
	UPROPERTY(EditAnywhere, Category = "Status")
		float RollStamina;
	UPROPERTY(EditAnywhere, Category = "Status")
		float ParryStamina;
	UPROPERTY(EditAnywhere, Category = "Status")
		float FaceAngle;
	
	UPROPERTY(EditAnywhere, Category = "Status")
		float WalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Status")
		float RunSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Status")
		FRotator RollDestination;

	UPROPERTY(EditAnywhere, Category = "SaveData")
		FVector StartPoint;
	UPROPERTY(VisibleDefaultsOnly, Category = "SaveData")
		FVector DeathLocation;

	UPROPERTY(VisibleAnywhere, Category = "TargetSystem")
		bool bTargetLock = false; 
	UPROPERTY(VisibleAnywhere, Category = "TargetSystem")
		bool bLockSwitching = false; 
	UPROPERTY(EditAnywhere, Category = "TargetSystem")
		float LockInterpSpeed = 5.0f; 
	UPROPERTY(VisibleAnywhere, Category = "TargetSystem")
		TArray<AEnemy*> TargetArray;
	UPROPERTY(VisibleAnywhere, Category = "TargetSystem")
		TArray<AEnemy*> TargetRight;
	UPROPERTY(VisibleAnywhere, Category = "TargetSystem")
		TArray<AEnemy*> TargetLeft;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "TargetSystem", meta = (AllowPrivateAccess = "true"))
		AActor* LockedTarget;

	UPROPERTY(EditAnywhere, Category = "BPClass")
		TSubclassOf<AExpItem> LostExpClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Controller")
		ABasicPlayerController* PlayerController;

		IInteractionInterface* OverlappingActor;
};
