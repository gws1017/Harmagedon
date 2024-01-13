#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Global.h"

//�𸮾� ���� ����� �Ʒ��ʿ�, ���α׷��Ӱ��ۼ��� ����� �������� �и�
//ĳ���� �⺻ ���� ��ҵ�
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"

//�Է�
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
	:WeaponEquipped(EWeaponEquipped::EWE_Fist),
	MovementState(EMovementState::EMS_Normal)
{
	//Tick�Լ� �Ⱦ��� �ϴ� ������
	PrimaryActorTick.bCanEverTick = false;

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

void APlayerCharacter::Hit(const FVector& ParticleSpawnLocation)
{
	CheckFalse(Alive());

	//if (AudioComponent->Sound)
	//	AudioComponent->Play();

	//ResetCombo();
	SetMovementState(EMovementState::EMS_Hit);
	PlayAnimMontage(HitMontage);
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
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
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

void APlayerCharacter::OffRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 200;
}

void APlayerCharacter::Roll()
{
	CheckFalse(CanRoll());
	SetMovementState(EMovementState::EMS_Roll);
	PlayAnimMontage(RollMontage);
}

void APlayerCharacter::EquipWeapon()
{
	//CheckFalse(Alive());
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
		/*if (Stat.Stamina - RollStamina > 0)
			return true;
		else return false;*/

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
		break;
		/*if (GetWeapon()->GetStaminaCost() < Stat.Stamina)
			return true;
		else return false;*/

	}
	return true;
}
