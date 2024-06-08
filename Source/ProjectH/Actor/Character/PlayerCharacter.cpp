#include "Actor/Character/PlayerCharacter.h"
#include "Actor/Character/PlayerAnimInstance.h"
#include "Actor/Controller/BasicPlayerController.h"
#include "Actor/Character/Enemy.h"
#include "Actor/Item/Weapon/Weapon.h"
#include "Actor/Item/PickupItem.h"
#include "Actor/Item/ExpItem.h"
#include "UI/Slot.h"

#include "Interface/InteractionInterface.h"

#include "System/MySaveGame.h"
#include "System/MyGameInstance.h"
#include "System/Sound/SoundManager.h"

#include "Data/CharacterAbilityTables.h"

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
#include "Data/HCollision.h"
#include "Cain.h"

APlayerCharacter::APlayerCharacter()
	:AttackCount(0),
	NumberOfAttacks(2),
	WeaponEquipped(EWeaponEquipped::EWE_Fist),
	MovementState(EMovementState::EMS_Normal),
	Stat{ 1,5,4,2,1,2,2 },
	BlockMinStamina(0.1f),
	BlockStaminaRate(0.35f),
	GuardStaminaDeclineRate(0.4f),
	StaminaRegenRate(2.f),
	RollStamina(33.f),
	RunStamina(5.f),
	ParryStamina(10.f),
	FaceAngle(150.f),
	WalkSpeed(400.f), RunSpeed(600.f),
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


	HandArmorComponents.Init(nullptr, 2);
	ShoulderArmorComponents.Init(nullptr, 2);

	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &ChestArmorComponent, "ChestArmor",GetMesh());
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &PantsArmorComponent, "PantsArmor",GetMesh());
	UHelpers::CreateComponent<USkeletalMeshComponent>(this, &ShoesArmorComponent, "ShoeArmor",GetMesh());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &HeadArmorComponent, "HeadArmor",GetMesh());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &HandArmorComponents[0], "LeftHandArmor", GetMesh());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &HandArmorComponents[1], "RightHandArmor", GetMesh());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &ShoulderArmorComponents[0], "LeftShoulderArmor", GetMesh());
	UHelpers::CreateComponent<UStaticMeshComponent>(this, &ShoulderArmorComponents[1], "RightShoulderArmor", GetMesh());

	

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->SetWalkableFloorAngle(70.f);

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_HCAPSULE);

	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	Tags.Add("Player");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ABasicPlayerController>(GetController());

	//인벤토리 아이템 추가
	InventoryComponent->AddItem(1,false);
	InventoryComponent->AddItem(10,false);
	InventoryComponent->AddItem(20,false);
	InventoryComponent->AddItem(30,false);
	InventoryComponent->AddItem(40,false);
	InventoryComponent->AddItem(50,false);
	InventoryComponent->AddItem(60,false);

	//소켓 설정
	HeadArmorComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("HeadSocket"));
	ShoulderArmorComponents[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("ShoulderLSocket"));
	ShoulderArmorComponents[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("ShoulderRSocket"));
	HandArmorComponents[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("HandLSocket"));
	HandArmorComponents[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("HandRSocket"));

	TargetingSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::TargetingBeginOverlap);
	TargetingSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::TargetingEndOverlap);

	//LoadGameData();
	InitStatusInfo();

	//인벤토리 방어구 캡처
	SceneCapture->ShowOnlyComponent(GetMesh());
	SceneCapture->ShowOnlyComponent(ChestArmorComponent);
	SceneCapture->ShowOnlyComponent(PantsArmorComponent);
	SceneCapture->ShowOnlyComponent(ShoesArmorComponent);
	SceneCapture->ShowOnlyComponent(HeadArmorComponent);
	SceneCapture->ShowOnlyComponent(ShoulderArmorComponents[0]);
	SceneCapture->ShowOnlyComponent(ShoulderArmorComponents[1]);
	SceneCapture->ShowOnlyComponent(HandArmorComponents[0]);
	SceneCapture->ShowOnlyComponent(HandArmorComponents[1]);

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
	
	SmoothRoll();

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
		//기본 조작 - 이동, 회전, 달리기
		EnhancedInput->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnRunning);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::OffRunning);


		//구르기
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Roll);

		//우측 무기 공격
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftClick);
		
		//좌측 무기 공격
		EnhancedInput->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RightClick);
		EnhancedInput->BindAction(RightClickAction, ETriggerEvent::Completed, this, &APlayerCharacter::OffRightClick);
		//좌측 무기 특수 능력
		EnhancedInput->BindAction(RightClickSpecialAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RightSpecialClick);

		//무기 장착
		EnhancedInput->BindAction(EquipAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EquipWeapon);
		//상호작용
		EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction);
		//락온
		EnhancedInput->BindAction(TargetLockAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DetectTarget);

		//UI관련 입력 바인딩
		EnhancedInput->BindAction(OpenEquipUIAction, ETriggerEvent::Triggered, GetPlayerController(), &ABasicPlayerController::ToggleEquipMenu);
		//EnhancedInput->BindAction(EscAction, ETriggerEvent::Triggered, this, &APlayerCharacter::);
	}
	
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageAmount <= 0.f)
		return DamageAmount;

	// 무적상태
	if (bIFrame)
		DamageAmount = 0;
	CheckGuard(DamageAmount, DamageCauser);
	CheckParry(DamageAmount, DamageCauser);

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
	float Damage = Stat.PhyDamage;
	Damage += GetWeaponDamage(Type);
	
	return Damage;
}

float APlayerCharacter::GetWeaponDamage(const EEquipType Type) const
{
	float Damage = 0.f;
	if (Type == EEquipType::ET_RightWeapon)
	{
		CheckNullResult(RightWeapon, Damage);
		Damage = RightWeapon->GetPhysicalDamage();
	}
	else if (Type == EEquipType::ET_LeftWeapon)
	{
		CheckNullResult(LeftWeapon, Damage);
		Damage = LeftWeapon->GetPhysicalDamage();
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

void APlayerCharacter::AttackCombo(const EEquipType Type)
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		if (NumberOfAttacks > AttackCount)
		{
			PlayAttackMontage(Type);
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

void APlayerCharacter::LeftClick()
{
	CheckFalse(CanAttack(EEquipType::ET_RightWeapon));
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		RightWeapon->BasicAttack();
	}
		
}

void APlayerCharacter::RightClick()
{
	CheckFalse(CanAttack(EEquipType::ET_LeftWeapon));
	LeftWeapon->Block();
}

void APlayerCharacter::OffRightClick()
{
	bBlocking = false;
	bBlockFail = false;
	StaminaRegenRate = 2.f;
}

void APlayerCharacter::OnGuard()
{
	CheckFalse(CanBlock());
	bBlocking = true;
	StaminaRegenRate *= 0.4f;
}

void APlayerCharacter::RightSpecialClick()
{
	CheckFalse(CanAttack(EEquipType::ET_LeftWeapon));
	LeftWeapon->SpecialAttack();
}

void APlayerCharacter::PlayAttackMontage(const EEquipType Type)
{
	auto CurrentWeapon = GetWeapon(Type);
	CheckNull(CurrentWeapon->GetAttackMontage());

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

	AnimInstance->Montage_Play(CurrentWeapon->GetAttackMontage());
	AnimInstance->Montage_JumpToSection(FName(SectionName));

	AttackCount++;
	if(CurrentWeapon)
		DecrementStamina(CurrentWeapon->GetStaminaCost());
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
	CheckFalse(CanHit());
	ASoundManager::GetSoundManager()->PlaySFXAtLocation(this, ESFXType::ESFXType_IronToMeat, GetActorLocation());

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
;
}

void APlayerCharacter::InitStatusInfo()
{
	auto GameInstance = GetGameInstance<UMyGameInstance>();
	Stat.MaxMagicSlot = GameInstance->GetCharAbilityData(Stat.Intelligence)->MaxMagicSlot;
	Stat.BleedResistance = GameInstance->GetCharAbilityData(Stat.Enduarance)->BleedResistance;
	Stat.PoisonResistance = GameInstance->GetCharAbilityData(Stat.Enduarance)->PoisonResistance;
	Stat.PhyDamage = GameInstance->GetCharAbilityData(Stat.Strength)->PhyDmg;
	Stat.MagDamage = GameInstance->GetCharAbilityData(Stat.Intelligence)->MagDmg;
	Stat.MaxWeight = GameInstance->GetCharAbilityData(Stat.Vitality)->MaxWeight;
	Stat.Poise = GameInstance->GetCharAbilityData(Stat.Enduarance)->Poise;
	Stat.MaxMana = GameInstance->GetCharAbilityData(Stat.Intelligence)->TotalMana;
	Stat.MaxHP = GameInstance->GetCharAbilityData(Stat.Vitality)->TotalHP;
	Stat.MaxStamina = GameInstance->GetCharAbilityData(Stat.Energy)->TotalStamina;

	Stat.HP = Stat.MaxHP;
	Stat.Stamina = Stat.MaxStamina;
	Stat.Mana = Stat.MaxMana;
}

void APlayerCharacter::SetCapture(AActor* InActor, const bool bIncludeFromChildActors)
{
	SceneCapture->ShowOnlyActorComponents(InActor, bIncludeFromChildActors);
}

void APlayerCharacter::RemoveCapture(AActor* InActor, const bool bIncludeFromChildActors)
{
	SceneCapture->RemoveShowOnlyActorComponents(InActor, bIncludeFromChildActors);
}

void APlayerCharacter::Equip(const EEquipType Type/*, AEquipItem* EquipItem */)
{
	//플레이어가 가진 인스턴스를 장착하지말고 인스턴스를 넘겨받아서 장착하자
	switch (Type)
	{
	case EEquipType::ET_LeftWeapon:
		LeftWeapon->Equip(Type);
		break;
	case EEquipType::ET_RightWeapon:
		RightWeapon->Equip(Type);
		break;
	}
	// EquipItem->Equip(Type);
	//무기에서 장비종류를 얻어오자
	WeaponEquipped = EWeaponEquipped::EWE_Sword;
}

void APlayerCharacter::UnEquip(const EEquipType Type)
{
	if (ActiveWeapon)
	{
		ActiveWeapon->UnEquip(Type);
		WeaponEquipped = EWeaponEquipped::EWE_None;
	}
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

void APlayerCharacter::EquipArmor(const EEquipType Type, USkeletalMesh* SkeletalMesh, const TArray<UStaticMesh*> StaticMeshes)
{
	switch (Type)
	{
	case EEquipType::ET_Head:
		CheckNull(StaticMeshes.Num() > 0);
		HeadArmorComponent->SetStaticMesh(StaticMeshes[0]);
		break;
	case EEquipType::ET_Top:
		CheckNull(SkeletalMesh);
		CheckNull(StaticMeshes.Num() > 0);
		ChestArmorComponent->SetSkeletalMesh(SkeletalMesh);
		ChestArmorComponent->SetLeaderPoseComponent(GetMesh());
		ShoulderArmorComponents[0]->SetStaticMesh(StaticMeshes[0]);
		ShoulderArmorComponents[1]->SetStaticMesh(StaticMeshes[1]);
		break;
	case EEquipType::ET_Bottom:
		CheckNull(SkeletalMesh);
		PantsArmorComponent->SetSkeletalMesh(SkeletalMesh);
		PantsArmorComponent->SetLeaderPoseComponent(GetMesh());
		break;
	case EEquipType::ET_Hand:
		CheckNull(StaticMeshes.Num() > 0);
		HandArmorComponents[0]->SetStaticMesh(StaticMeshes[0]);
		HandArmorComponents[1]->SetStaticMesh(StaticMeshes[1]);
		break;
	case EEquipType::ET_Shoe:
		CheckNull(SkeletalMesh);
		ShoesArmorComponent->SetSkeletalMesh(SkeletalMesh);
		ShoesArmorComponent->SetLeaderPoseComponent(GetMesh());
		break;
	}
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
	CheckFalse(GetStaminaRate() > RunStamina);
	SetMovementState(EMovementState::EMS_Run);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::OffRunning()
{
	SetMovementNormal();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::SmoothRoll()
{
	//구르기 연속으로 할경우 이전 방향 보간값으로 바꿔줘야 방향이 바뀐다.
	if (EMovementState::EMS_Roll == MovementState)
	{
		if (!RollDestination.Equals(FRotator::ZeroRotator))
		{
			FRotator LerpRotation = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), RollDestination,
				UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 720.f);
			SetActorRotation(LerpRotation);
		}
	}
}

void APlayerCharacter::Roll()
{
	CheckFalse(CanRoll());
	SetMovementState(EMovementState::EMS_Roll);
	DecrementStamina(RollStamina);
	PlayAnimMontage(RollMontage);
	//처음 구르기 방향 저장
	RollDestination = UKismetMathLibrary::MakeRotFromX(GetLastMovementInputVector());
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
	CheckNull(OverlappingActor);
	OverlappingActor->OnInteraction();
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

bool APlayerCharacter::CheckFace(AActor* OtherActor)
{
	//Yaw각도 차이는 서로 같은방향을 바라보면 0에 가까워지고(같은 각도니까), 
	//반대로 마주본다면 각도차이는 커질 것이다.
	float AngleZ = FMath::Abs(GetActorRotation().Yaw - OtherActor->GetActorRotation().Yaw);
	return AngleZ >= FaceAngle;
}

bool APlayerCharacter::CheckGuard(float& DamageAmount, AActor* DamageCauser)
{
	bool ret = false;

	if (CheckFace(DamageCauser) == false)
		bBlockFail = true;
	//가드성공
	if (bBlocking && !bBlockFail)
	{
		//왼쪽 무기에서 물리경감률을 얻어온다, 가드는 항상 왼쪽무기로
		CheckNullResult(LeftWeapon, false);
		ret = true;
		DamageAmount = DamageAmount * (1.0f - LeftWeapon->GetPhysicalDefenseRate());
		DecrementStamina(Stat.MaxStamina * BlockStaminaRate);
		ASoundManager::GetSoundManager()->PlaySFXAtLocation(this, ESFXType::ESFXType_Guard, GetActorLocation());
	}//가드 실패
	else if (bBlockFail)
	{
		//받는 데미지 증가
		ret = false;
		DamageAmount *= 1.2f;
	}
	return ret;
}

bool APlayerCharacter::CheckParry(float& DamageAmount, AActor* DamageCauser)
{
	//패리가능한상태에서 데미지가 들어오면 패리성공처리
	//플레이어 방향을 확인하라
	if (CheckFace(DamageCauser) == false)
		bParryFail = true;
		
	if (bCanParry && !bParryFail)
	{
		if (DamageAmount > 0)//무적은아니지만 이후 4프레임에서 패링을 성공함
		{
			CheckNullResult(LeftWeapon,false);
			DamageAmount = DamageAmount * (1.0f - LeftWeapon->GetPhysicalDefenseRate());
			DecrementStamina(Stat.MaxStamina * 0.2f);
		}
		bParrySucc = true;

		ASoundManager::GetSoundManager()->PlaySFXAtLocation(this, ESFXType::ESFXType_ParrySucc, GetActorLocation());
		
		auto enemy = Cast<AEnemy>(DamageCauser);
		enemy->SetActionState(EMonsterAction::EMA_Stun);
		enemy->Stun();

		CLog::Print("Parry Succ");
	}
	else if (bCanParry && bParryFail) //패리 실패시 패널티 부여
	{
		CLog::Print("Parry Fail");
		DamageAmount = DamageAmount * (1.0f - LeftWeapon->GetPhysicalDefenseRate());
		DecrementStamina(Stat.MaxStamina * 0.4f);
	}
	return bParrySucc;
}

bool APlayerCharacter::CanRoll()
{
	CheckTrueResult(bIsAttacking, false);
	CheckNullResult(RollMontage,false);
	CheckTrueResult(PlayerController->bShowMouseCursor, false);
	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		break;
		
	}
	if (Stat.Stamina - RollStamina > 0)
		return true;
	else return false;

	return true;
}

bool APlayerCharacter::CanAttack(EEquipType Type)
{
	CheckNullResult(GetWeapon(Type), false);
	//if (PlayerController)CheckFalseResult(PlayerController->GetGameMode(), false);
	CheckFalseResult(GetWeapon(Type)->GetEquipped(), false);
	CheckTrueResult(GetWeapon(Type)->GetEquipping(), false);
	CheckTrueResult(bBlocking, false);
	CheckTrueResult(PlayerController->bShowMouseCursor, false);

	switch (MovementState)
	{
	case EMovementState::EMS_Dead:
	case EMovementState::EMS_Hit:
	case EMovementState::EMS_Roll:
		return false;
	default:
		if (GetWeapon(Type)->GetStaminaCost() < Stat.Stamina)
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

bool APlayerCharacter::CanHit()
{
	bool result = true;

	if (!Alive()) result = false;
	if (HitMontage == nullptr) result = false;
	if (MovementState == EMovementState::EMS_Roll) result = false;

	return result;
}

bool APlayerCharacter::CanBlock()
{
	//CheckTrueResult(bBlocking,false);
	CheckTrueResult(PlayerController->bShowMouseCursor, false);
	if (Stat.Stamina < Stat.MaxStamina * BlockMinStamina)
	{
		if (!bBlockFail)
		{
			FTimerHandle BlockFailTimer;
			GetWorldTimerManager().SetTimer(BlockFailTimer, [this]() {
				bBlockFail = false;

				}, 1.f, false);
		}
		bBlockFail = true;
		OffRightClick();
		//받는데미지 1초간 증가
		return false;
	}
	return true;
}

void APlayerCharacter::UpdateStamina(float DeltaStamina)
{
	CheckTrue(MovementState == EMovementState::EMS_Dead); //죽었을 때 종료
	CheckTrue((Stat.Stamina == Stat.MaxStamina) && (MovementState != EMovementState::EMS_Run)); //스테미나 변동이 없을 시 종료

	CanBlock();

	if (MovementState == EMovementState::EMS_Run && FMath::IsNearlyZero(GetVelocity().Length()) == false)
	{
		Stat.Stamina -= DeltaStamina;
		Stat.Stamina = FMath::Clamp(Stat.Stamina, 0.f, Stat.MaxStamina);
		if (GetStaminaRate() <= 0.f)
		{
			OffRunning();
		}
		return;
	}

	Stat.Stamina += DeltaStamina;

	Stat.Stamina = FMath::Clamp(Stat.Stamina, 0.f, Stat.MaxStamina);
}

void APlayerCharacter::DecrementStamina(float Amount)
{
	Stat.Stamina = FMath::Clamp(Stat.Stamina - Amount, 0.f, Stat.MaxStamina);
}
