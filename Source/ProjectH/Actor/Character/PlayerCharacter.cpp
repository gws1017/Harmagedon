#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Global.h"

//언리얼 관련 헤더는 아래쪽에, 프로그래머가작성한 헤더는 위쪽으로 분리
//캐릭터 기본 구성 요소들
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"

//입력
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
	:WeaponEquipped(EWeaponEquipped::EWE_Fist),
	MovementState(EMovementState::EMS_Normal),
	Stat{ 15,15,50,50,0,1,1,1,1,0 },
	StaminaRegenRate(2.f),
	RollStamina(10.f)
{
	//Tick함수 안쓰면 일단 꺼놓기
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	UHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->SetWalkableFloorAngle(70.f);

	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ABasicPlayerController>(GetController());

	if(!!WeaponClass)
		WeaponInstance = AWeapon::Spawn<AWeapon>(GetWorld(),WeaponClass, this);

	CheckNull(PlayerController);
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(IMCPlayer, 0);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaRegenRate * DeltaTime;
	UpdateStamina(DeltaStamina);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInput->BindAction(EquipAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EquipWeapon);

		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Roll);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);

		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRunning);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::OffRunning);
	}
	
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void APlayerCharacter::End_Attack()
{
	bIsAttacking = false;
}

bool APlayerCharacter::Alive()
{
	if (MovementState != EMovementState::EMS_Dead)
		return true;
	else return false;
}

void APlayerCharacter::Die()
{
	CheckNull(DeathMontage);
	//SaveGameData(1);

	SetMovementState(EMovementState::EMS_Dead);
	GetCharacterMovement()->StopMovementImmediately();

	StopAnimMontage();

	PlayAnimMontage(DeathMontage);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (!!WeaponInstance)
		WeaponInstance->DeactivateCollision();
	//GetController<APlayerController>()->ShowRestartenu();
}

void APlayerCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void APlayerCharacter::Hit(const FVector& ParticleSpawnLocation)
{
	CheckFalse(Alive());
	CheckNull(HitMontage);
	//if (AudioComponent->Sound)
	//	AudioComponent->Play();

	//ResetCombo();
	SetMovementState(EMovementState::EMS_Hit);
	PlayAnimMontage(HitMontage);
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	CheckFalse(CanMove());
	FVector2D MovementVec = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector ForwardDirection = FRotationMatrix(rotator).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(rotator).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVec.Y);
		AddMovementInput(RightDirection, MovementVec.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::OnRunning()
{
	SetMovementState(EMovementState::EMS_Run);
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

void APlayerCharacter::OffRunning()
{
	SetMovementNormal();
	GetCharacterMovement()->MaxWalkSpeed = 200;
}

void APlayerCharacter::Roll()
{
	CheckFalse(CanRoll());
	SetMovementState(EMovementState::EMS_Roll);
	DecrementStamina(RollStamina);
	PlayAnimMontage(RollMontage);
}

void APlayerCharacter::EquipWeapon()
{
	CheckFalse(Alive());
	CheckNull(WeaponInstance);
	if (WeaponInstance->GetEquipped())
	{
		WeaponInstance->UnEquip();
		return;
	}

	WeaponInstance->Equip();
}

void APlayerCharacter::Attack()
{
	CheckFalse(CanAttack());
	bIsAttacking = true;
	PlayAnimMontage(AttackMontage);
	DecrementStamina(WeaponInstance->GetStaminaCost());
}

bool APlayerCharacter::CanRoll()
{
	CheckTrueResult(bIsAttacking, false);
	CheckNullResult(RollMontage,false);
	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		break;
		if (Stat.Stamina - RollStamina > 0)
			return true;
		else return false;
	}
	return true;
}

bool APlayerCharacter::CanAttack()
{
	CheckNullResult(AttackMontage,false);
	//if (PlayerController)CheckFalseResult(PlayerController->GetGameMode(), false);
	CheckTrueResult(bIsAttacking, false);
	CheckFalseResult(WeaponInstance->GetEquipped(), false);
	CheckTrueResult(WeaponInstance->GetEquipping(), false);
	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (GetWeapon()->GetStaminaCost() < Stat.Stamina)
			return true;
		else return false;

	}
	return true;
}

bool APlayerCharacter::CanMove()
{
	CheckFalseResult(Alive(), false);

	return true;
}

void APlayerCharacter::UpdateStamina(float DeltaStamina)
{
	CheckTrue(MovementState == EMovementState::EMS_Dead); //죽었을 때 종료
	CheckTrue((Stat.Stamina == Stat.MaxStamina) && (MovementState != EMovementState::EMS_Run)); //스테미나 변동이 없을 시 종료

	if (MovementState == EMovementState::EMS_Run && FMath::IsNearlyZero(GetVelocity().Length()) == false)
	{
		Stat.Stamina -= DeltaStamina;
		Stat.Stamina = FMath::Clamp(Stat.Stamina, 0.f, Stat.MaxStamina);
		if (Stat.Stamina <= 0)
		{
			OffRunning();
		}
		return;
	}

	Stat.Stamina += DeltaStamina;

	Stat.Stamina = FMath::Clamp(Stat.Stamina, 0.f, Stat.MaxStamina);
	return;
}

void APlayerCharacter::DecrementStamina(float Amount)
{
	Stat.Stamina = FMath::Clamp(Stat.Stamina - Amount, 0.f, Stat.MaxStamina);
}
