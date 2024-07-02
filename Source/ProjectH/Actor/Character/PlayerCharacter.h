#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/WeaponInterface.h"
#include "Interface/HitInterface.h"
#include "Interface/ICharacter.h"
#include "PlayerCharacter.generated.h"

//����� ���漱�� �� ��

class USkeletalMesh;
class UStaticMesh;

class USpringArmComponent;
class USphereComponent;
class UCameraComponent;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class USceneCaptureComponent2D;
class UParticleSystem;

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
	EWE_Shield UMETA(DisplayName = "Shield"),
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
	EMS_Exhausted UMETA(DisplayName = "Exhausted"),
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
		int32 MaxMagicSlot; //���� ����, ������ ���� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float MaxWeight; //�ִ�����, ü�¿� ���� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		float Poise; //���ε�, �ٷ¿� ���� ����, ������� ���ε� �������� �氨�����ش�.

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Vitality; //ü�� - HP ���� �������� ��������
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Strength; //�ٷ� - ���� ���ݷ�, ���ε� ���� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Energy; //��� - ���׹̳� ����, ���� ���ݷ� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Faith; //�ž� - ���� ���ݷ�, ���� ���� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Intelligence; //���� - ����, ���� ���� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Enduarance; //�γ� - ���ε�, ����, ���� ���� ����
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Level; //���� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		int32 Exp; //���� ����ġ

};

UCLASS()
class PROJECTH_API APlayerCharacter 
	: 
	public ACharacter, 
	public IWeaponInterface, 
	public IHitInterface,
	public IICharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

//ACharacter Ŭ���� �⺻ ��� �Լ�
//Ŭ���� ���� �� ��ӵǴ� �⺻�Լ� ������� �ʴ´ٸ� �����ֱ�
protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Landed(const FHitResult& Hit) override;

	void SetThrownByBoss(bool flag) { IsThrownByBoss = flag; }

public:

	UFUNCTION()
		void TargetingBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TargetingEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

	//�ܺο��� ȣ��Ǵ� �Լ� �ۼ�
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
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool IsInvincible() { return bIFrame; }
	FORCEINLINE bool IsBlocking() { return bBlocking; }
	FORCEINLINE bool GetParrySucc() { return bParrySucc; }
	FORCEINLINE bool GetParryFail() { return bParryFail; }
	FORCEINLINE float GetHP() const { return Stat.HP; }
	FORCEINLINE float GetMaxHP() const { return Stat.MaxHP; }
	FORCEINLINE float GetStamina() const { return Stat.Stamina; }
	FORCEINLINE float GetMaxStamina() const { return Stat.MaxStamina; }
	FORCEINLINE float GetStaminaRate() const { return Stat.Stamina / Stat.MaxStamina * 100.f; }

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
	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetInvincible(const bool value) { bIFrame = value; }
	FORCEINLINE void SetParrySucc(const bool value) { bParrySucc = value; }
	FORCEINLINE void SetParryFail(const bool value) { bParryFail = value; }
	FORCEINLINE void SetCanParryed(const bool value) { bCanParry = value; }
	FORCEINLINE void SetBlock(const bool value) { bBlocking = value; }
	FORCEINLINE void SetHP(const float value) { Stat.HP = value; }
	FORCEINLINE void SetStartPoint(const FVector Loc) { StartPoint = Loc; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetStr(const float value) { Stat.Strength = value; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE void MoveStartPoint() { SetActorLocation(StartPoint); }

 	void SetWeapon(EEquipType Type, AWeapon* Instance);
	void EmptyWeapon();
public:

	void End_Attack();
	void AttackCombo(const EEquipType Type);
	void ResetAttack();
	void PlayAttackMontage(const EEquipType Type);

	bool Alive();
	void Die();
	virtual void DeathEnd();

	void Equip(const EEquipType Type);
	void UnEquip(const EEquipType Type);
	void QuickUnEquip(AWeapon* Instance);
	void EquipArmor(const EEquipType Type, USkeletalMesh* SkeletalMesh, const TArray<UStaticMesh*> StaticMeshes);

	void IncrementExp(float Amount);
	void LevelUp(const FPlayerStatus& data);
	void StatusRestore();

	void DecrementStamina(float Amount);

	bool CanBlock();

	virtual void Hit(const FVector& ParticleSpawnLocation);

	UFUNCTION(BlueprintCallable)
		void SaveGameData(int32 SaveType = 0);
	UFUNCTION(BlueprintCallable)
		void LoadGameData();

	UFUNCTION(BlueprintCallable)
		void InitStatusInfo();

	void SetCapture(AActor* InActor, const bool bIncludeFromChildActors);
	void RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors);
	//�ܺο��� ������ �� �ִ� ���� �ۼ�(�ǵ����̸� ������ private�� �ۼ��ϰ� Getter Setter �̿��� ��)
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* ActiveWeapon;

private:

	//ĳ���� ���ο����� ȣ��Ǵ� �Լ� �ۼ� (�ַ� Ű�Է�)
	//Ű�Է� ���� �Լ�
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void OnRunning();
	void OffRunning();

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

	//�ٸ� ���Ϳ� ���ֺ����ִ��� �˻��ϴ� �Լ�
	bool CheckFace(AActor* OtherActor);
	bool CheckGuard(float& DamageAmount, AActor* DamageCauser, const FHitResult&);
	bool CheckParry(float& DamageAmount, AActor* DamageCauser);

	bool CanRoll();
	bool CanAction(EEquipType Type);
	bool CanMove();
	bool CanHit();
	bool CanRun();

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

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Armor | Chest", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ChestArmorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor | Pants", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* PantsArmorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor | Shoes", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ShoesArmorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor | Head", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* HeadArmorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor | Hands", meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*> HandArmorComponents;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor | Chest", meta = (AllowPrivateAccess = "true"))
		TArray<UStaticMeshComponent*> ShoulderArmorComponents;


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
		UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ParryMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Particle")
		UParticleSystem* HitParticle;

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
		bool bParrySucc = false; //�и� �����ߴ��� üũ�ϱ����� �뵵
	UPROPERTY(VisibleAnywhere, Category = "Status")
		bool bParryFail = false; //�и� ���н� Ȱ��ȭ
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
		float RunStamina;
	UPROPERTY(EditAnywhere, Category = "Status")
		float ParryStamina;
	UPROPERTY(EditAnywhere, Category = "Status")
		float FaceAngle;
	

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


	bool IsThrownByBoss = false;
};
