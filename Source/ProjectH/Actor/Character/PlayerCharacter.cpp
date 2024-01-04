#include "Actor/Character/PlayerCharacter.h"
#include "Global.h"

//�𸮾� ���� ����� �Ʒ��ʿ�, ���α׷��Ӱ��ۼ��� ����� �������� �и�
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

APlayerCharacter::APlayerCharacter()
{
	//Tick�Լ� �Ⱦ��� �ϴ� ������
	PrimaryActorTick.bCanEverTick = false;

	UHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	UHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->SetWalkableFloorAngle(70.f);

	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizonLock", this, &APlayerCharacter::OnHorizonLock);
	PlayerInputComponent->BindAxis("VerticalLock", this, &APlayerCharacter::OnVerticalLock);

}

void APlayerCharacter::OnMoveForward(float Axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector dir = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D();

	AddMovementInput(dir, Axis);
}

void APlayerCharacter::OnMoveRight(float Axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector dir = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();

	AddMovementInput(dir, Axis);
}

void APlayerCharacter::OnHorizonLock(float Axis)
{
	//if (bIsControlledPlayer && PlayerController->bShowMouseCursor == true) return;
	AddControllerYawInput(Axis);
}

void APlayerCharacter::OnVerticalLock(float Axis)
{
	//if (bIsControlledPlayer && PlayerController->bShowMouseCursor == true) return;
	AddControllerPitchInput(Axis);
}

