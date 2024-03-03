#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Character/Enemy.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Item/PickupItem.h"
#include "Actor/Item/ExpItem.h"
#include "UI/Slot.h"
#include "System/MySaveGame.h"

#include "Component/InventoryComponent.h"
#include "Global.h"

//언리얼 관련 헤더는 아래쪽에, 프로그래머가작성한 헤더는 위쪽으로 분리
//캐릭터 기본 구성 요소들
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Animation/AnimMontage.h"

//입력
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
	:AttackCount(0),
	NumberOfAttacks(2),
	WeaponEquipped(EWeaponEquipped::EWE_Fist),
	MovementState(EMovementState::EMS_Normal),
	Stat{ 15,15,50,50,0,1,1,1,1,0 },
	StaminaRegenRate(2.f),
	RollStamina(10.f),
	StartPoint(0.f,0.f,0.f)
{
	//Tick함수 안쓰면 일단 꺼놓기
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	UHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	UHelpers::CreateComponent<USceneCaptureComponent2D>(this, &SceneCapture, "SceneCapture", GetCapsuleComponent());
	UHelpers::CreateComponent<USphereComponent>(this, &TargetingSphere, "TargetingSphere", GetCapsuleComponent());
	UHelpers::CreateActorComponent<UInventoryComponent>(this, &InventoryComponent, "Inventory");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->SetWalkableFloorAngle(70.f);

	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	Tags.Add("Player");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ABasicPlayerController>(GetController());
	InventoryComponent->AddItem(1,false);

	TargetingSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::TargetingBeginOverlap);
	TargetingSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::TargetingEndOverlap);

	LoadGameData();
	SceneCapture->ShowOnlyComponent(GetMesh());
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
	if (bTargetLock)
	{
		LockTarget();
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
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
		EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction);
		EnhancedInput->BindAction(TargetLockAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DetectTarget);

		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRunning);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::OffRunning);

		//UI관련 입력 바인딩
		EnhancedInput->BindAction(OpenEquipUIAction, ETriggerEvent::Triggered, GetPlayerController(), &ABasicPlayerController::ToggleEquipMenu);
		//EnhancedInput->BindAction(EscAction, ETriggerEvent::Triggered, this, &APlayerCharacter::);
	}
	
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0.f /*|| EPlayerStat::EPS_Invincible == PlayerStat*/)
		return DamageAmount;

	if (Stat.HP - DamageAmount <= 0.f)
	{
		Stat.HP = FMath::Clamp(Stat.HP - DamageAmount, 0.0f, Stat.MaxHP);
		AEnemy* enemy = Cast<AEnemy>(DamageCauser);
		if (enemy)
		{
			enemy->InitTarget();
			enemy->SetAlerted(false);
		}
		Die();
	}
	else
	{
		Stat.HP = FMath::Clamp(Stat.HP - DamageAmount, 0.0f, Stat.MaxHP);
	}

	UE_LOG(LogTemp, Display, L"Player Current HP : %f", Stat.HP);
	return DamageAmount;
}

void APlayerCharacter::TargetingBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Target = Cast<AEnemy>(OtherActor);
	if (Target)
	{
		TargetArray.AddUnique(Target);
	}
}

void APlayerCharacter::TargetingEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemy* Target = Cast<AEnemy>(OtherActor);
	if (Target)
	{
		TargetArray.Remove(Target);
	}
}

AWeapon* APlayerCharacter::GetWeapon(const EEquipType Type) const
{
	if (Type == EEquipType::ET_RightWeapon)
		return RightWeapon;
	else if (Type == EEquipType::ET_LeftWeapon)
		return LeftWeapon;
	else return nullptr;
}

float APlayerCharacter::GetDamage(const EEquipType Type) const
{
	float Damage = Stat.StrengthDamage;
	Damage += GetWeaponDamage(Type);
	
	return Damage;
}

float APlayerCharacter::GetWeaponDamage(const EEquipType Type) const
{
	float Damage = 0.f;
	if (Type == EEquipType::ET_RightWeapon)
	{
		CheckNullResult(RightWeapon, Damage);
		Damage = RightWeapon->GetDamage();
	}
	else if (Type == EEquipType::ET_LeftWeapon)
	{
		CheckNullResult(LeftWeapon, Damage);
		Damage = LeftWeapon->GetDamage();
	}
	else CLog::Log("EquipType Error, Only Use Right, Left");
	return Damage;
}

void APlayerCharacter::SetWeapon(EEquipType Type, AWeapon* Instance)
{
	if (Type == EEquipType::ET_LeftWeapon)
		LeftWeapon = Instance;
	else if (Type == EEquipType::ET_RightWeapon)
		RightWeapon = Instance;
	else
		CLog::Log("Only Set Left ,Right Weapon Type");
}

void APlayerCharacter::End_Attack()
{
	bIsAttacking = false;
}

void APlayerCharacter::AttackCombo()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		if (NumberOfAttacks > AttackCount)
		{
			PlayAttackMontage();
		}
		else
		{
			FTimerHandle ResetTimer;
			GetWorld()->GetTimerManager().SetTimer(ResetTimer, FTimerDelegate::CreateLambda([&]()
				{
					ResetAttack();
					GetWorld()->GetTimerManager().ClearTimer(ResetTimer);
				}),0.3f,false);
		}
	}
	else
		ResetAttack();
}

void APlayerCharacter::ResetAttack()
{
	bSaveAttack = false;
	AttackCount = 0;
	bIsAttacking = false;
}

void APlayerCharacter::Attack()
{
	CheckFalse(CanAttack());
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		//왼손공격 기능 미구현 추후 수정, 오른손 공격으로 고정
		PlayAttackMontage(EEquipType::ET_RightWeapon);
	}
		
}

void APlayerCharacter::PlayAttackMontage(const EEquipType Type)
{
	CLog::Print(AttackCount);
	FString SectionName = "Attack";
	auto AnimInstance = GetMesh()->GetAnimInstance();
	switch (AttackCount)
	{
	case 0:
		SectionName += "A";
		break;
	case 1:
		SectionName += "B";
		break;
	}

	if (Type == EEquipType::ET_RightWeapon)
		SectionName += "_R";
	else if (Type == EEquipType::ET_LeftWeapon)
		SectionName += "_L";

	AnimInstance->Montage_Play(AttackMontage);
	AnimInstance->Montage_JumpToSection(FName(SectionName));

	AttackCount++;
	DecrementStamina(GetWeapon(EEquipType::ET_RightWeapon)->GetStaminaCost());
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
	SaveGameData(1);

	SetMovementState(EMovementState::EMS_Dead);
	GetCharacterMovement()->StopMovementImmediately();

	StopAnimMontage();

	PlayAnimMontage(DeathMontage);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (!!GetLeftWeapon())
		GetLeftWeapon()->DeactivateCollision();
	if (!!GetRightWeapon())
		GetRightWeapon()->DeactivateCollision();
	GetController<ABasicPlayerController>()->ShowRestartMenu();
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

	ResetAttack();
	SetMovementState(EMovementState::EMS_Hit);
	PlayAnimMontage(HitMontage);
}

void APlayerCharacter::SaveGameData(int32 SaveType)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->SaveData = {
		Stat,
		0,
		GetActorLocation(),
		GetActorRotation(),
		StartPoint,
		DeathLocation
	};
	if (SaveType == 1)//죽었을때 수정해야하는 부분
	{
		SaveGameInstance->SaveData.Location = StartPoint;
		SaveGameInstance->SaveData.LostExp = Stat.Exp; //현재경험치를 LostExp로 저장, 평소에는 0이 기본
		SaveGameInstance->SaveData.Status.Exp = 0;
		SaveGameInstance->SaveData.Status.HP = Stat.MaxHP;
		SaveGameInstance->SaveData.DeathLocation = GetActorLocation();
	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void APlayerCharacter::LoadGameData()
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		FSaveData Data = LoadGameInstance->SaveData;
		Stat = Data.Status;
		Stat.Stamina = Stat.MaxStamina;
		SetActorLocation(Data.Location);
		SetActorRotation(Data.Rotation);
		StartPoint = Data.StartPoint;
		if (Data.LostExp != 0) {
			if (LostExpClass)
			{
				AExpItem* actor = GetWorld()->SpawnActor<AExpItem>(LostExpClass);
				FVector Loc = Data.DeathLocation;
				Loc.Z -= 90.f;
				actor->Init(Data.LostExp, Loc);
			}
			else CLog::Log("LostExp BP is not valid");
		}

		SetMovementState(EMovementState::EMS_Normal);
		GetMesh()->bPauseAnims = false;
		GetMesh()->bNoSkeletonUpdate = false;
	}
	else CLog::Log("SaveData is not valid");
}

void APlayerCharacter::SetCapture(AActor* InActor, const bool bIncludeFromChildActors)
{
	SceneCapture->ShowOnlyActorComponents(InActor, bIncludeFromChildActors);
}

void APlayerCharacter::RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors)
{
	SceneCapture->RemoveShowOnlyActorComponents(InActor, bIncludeFromChildActors);
}

void APlayerCharacter::Equip(const EEquipType Type)
{
	switch (Type)
	{
	case EEquipType::ET_LeftWeapon:
		LeftWeapon->Equip(Type);
		break;
	case EEquipType::ET_RightWeapon:
		RightWeapon->Equip(Type);
		break;
	}
}

void APlayerCharacter::UnEquip(const EEquipType Type)
{
	if (ActiveWeapon)
		ActiveWeapon->UnEquip(Type);
	/*switch (Type)
	{
	case EEquipType::ET_LeftWeapon:
		LeftWeapon->UnEquip(Type);
		break;
	case EEquipType::ET_RightWeapon:
		RightWeapon->UnEquip(Type);
		break;
	}*/
}

void APlayerCharacter::IncrementExp(float Amount)
{
	Stat.Exp += Amount;
}

void APlayerCharacter::LevelUp(const FPlayerStatus& data)
{
	Stat = data;
	SaveGameData();
}

ABasicPlayerController* APlayerCharacter::GetPlayerController()
{
	if (!!PlayerController)
		return PlayerController;
	return GetController<ABasicPlayerController>();
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
	if (bTargetLock == false)
	{
		if (Controller != nullptr)
		{
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
	else
	{
		if (FMath::Abs(LookAxisVector.X) > 40.0f)
		{
			CheckNull(LockedTarget);
			CheckTrue(bLockSwitching);
			if (LookAxisVector.X > 0.f)
			{
				SwapTargetRight();
			}
			else
			{
				SwapTargetLeft();
			}
			bLockSwitching = true;
			FTimerHandle SwtichCoolTime;
			GetWorldTimerManager().SetTimer(SwtichCoolTime, [this, SwtichCoolTime]() {
				bLockSwitching = false;
				}, 1.f, false);
		}
		
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
	//if (GetWeapon() == nullptr)
	{
		//InventoryComponent->Equip(EEquipType::ET_RightHand, WeaponInstance);
	}
	//else
	{
		//InventoryComponent->UnEquip(EEquipType::ET_RightHand);
		return;
	}

	//WeaponInstance->Equip();
}

void APlayerCharacter::Interaction()
{
	CheckNull(OverlappingItem);
	OverlappingItem->OnInteraction();
}

void APlayerCharacter::DetectTarget()
{
	if (bTargetLock)
	{
		UnlockTarget();
	}
	else
	{
		float StartOffset = 280.f;
		float EndOffset = 2000.f;
		float Radius = 300.f;

		FVector Start = GetActorLocation();
		Start += Camera->GetForwardVector() * StartOffset;
		FVector End = Start + Camera->GetForwardVector() * EndOffset;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsTypes =
		{ UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody) };
		TArray<AActor*> IgonerActor = {this};
		FHitResult Result;

		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, Radius,
			ObjectsTypes, false, IgonerActor, EDrawDebugTrace::None, Result, true);

		if (Result.GetActor())
		{
			LockedTarget = Result.GetActor();
			bTargetLock = true;
		}
	}
}

void APlayerCharacter::LockTarget()
{
	float TargetZOffset = 100.f;
	FVector TargetLocation = LockedTarget->GetActorLocation();
	TargetLocation.Z -= TargetZOffset;

	float InertpSpeed = 5.f;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	FRotator LerpRotation = UKismetMathLibrary::RInterpTo_Constant(GetControlRotation(), TargetRotation,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), LockInterpSpeed);
	LerpRotation.Roll = GetControlRotation().Roll;

	GetController()->SetControlRotation(LerpRotation);
}

void APlayerCharacter::UnlockTarget()
{
	bTargetLock = false;
	LockedTarget = nullptr;
}

void APlayerCharacter::SwapTargetRight()
{
	TargetRight.Empty();
	for (auto enemy : TargetArray)
	{
		if (enemy != LockedTarget)
		{
			FVector DirVec = LockedTarget->GetActorLocation() - enemy->GetActorLocation();
			DirVec.Normalize();
			if (Camera->GetRightVector().Dot(DirVec) <= 0)
			{
				TargetRight.AddUnique(enemy);
			}
		}
	}
	CheckFalse(TargetRight.Num() > 0);
	AEnemy* CloseTarget = TargetRight[0];
	if (CloseTarget)
	{
		for (auto enemy : TargetRight)
		{
			if (enemy != LockedTarget)
			{
				if (enemy->GetDistanceTo(LockedTarget) < CloseTarget->GetDistanceTo(LockedTarget))
				{
					CloseTarget = enemy;
				}
			}
		
		}
	}
	LockedTarget = CloseTarget;
}

void APlayerCharacter::SwapTargetLeft()
{
	TargetLeft.Empty();
	for (auto enemy : TargetArray)
	{
		if (enemy != LockedTarget)
		{
			FVector DirVec = LockedTarget->GetActorLocation() - enemy->GetActorLocation();
			DirVec.Normalize();
			if (Camera->GetRightVector().Dot(DirVec) > 0)
			{
				TargetLeft.AddUnique(enemy);
			}
		}
	}
	CheckFalse(TargetLeft.Num() > 0);
	AEnemy* CloseTarget = TargetLeft[0];
	if (CloseTarget)
	{
		for (auto enemy : TargetLeft)
		{
			if (enemy != LockedTarget)
			{
				if (enemy->GetDistanceTo(LockedTarget) < CloseTarget->GetDistanceTo(LockedTarget))
				{
					CloseTarget = enemy;
				}
			}

		}
	}
	LockedTarget = CloseTarget;
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
	CheckNullResult(GetWeapon(EEquipType::ET_RightWeapon),false);
	CheckNullResult(AttackMontage,false);
	//if (PlayerController)CheckFalseResult(PlayerController->GetGameMode(), false);
	CheckFalseResult(GetWeapon(EEquipType::ET_RightWeapon)->GetEquipped(), false);
	CheckTrueResult(GetWeapon(EEquipType::ET_RightWeapon)->GetEquipping(), false);
	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (GetWeapon(EEquipType::ET_RightWeapon)->GetStaminaCost() < Stat.Stamina)
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
