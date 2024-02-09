#include "UI/LevelUPUI.h"
#include "UI/DialogBoxUI.h"
#include "Data/CharacterAbilityTables.h"
#include "Actor/Character/PlayerCharacter.h"
#include "System/MyGameInstance.h"
#include "Global.h"

void ULevelUPUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = GetGameInstance<UMyGameInstance>();
	SelectUINumber = 0;
	TotalExp = 0;
	NextExp = GameInstance->GetCharAbilityData(GetChangePlayerLevel() + 1)->LevelUpExp;
	LevelUpCount.Add({ 0,0 });
	LevelUpCount.Add({ 1,0 });
	LevelUpCount.Add({ 2,0 });
	AbilityType = 3;
	SelectAbility();
}

void ULevelUPUI::OnUpKey()
{
	if (IsInViewport()) {
		SelectUINumber--;
		if (SelectUINumber < 0) SelectUINumber = AbilityType - 1;
		SelectAbility();
	}

}

void ULevelUPUI::OnDownKey()
{
	if (IsInViewport()) {
		SelectUINumber++;
		if (SelectUINumber >= AbilityType) SelectUINumber = 0;
		SelectAbility();
	}
}

void ULevelUPUI::OnLeftKey()
{
	if (IsInViewport()) {
		if (LevelUpCount[SelectUINumber] > 0) {
			LevelUpCount[SelectUINumber]--;
			UpdateExp();
		}
	}
}

void ULevelUPUI::OnRightKey()
{
	if (IsInViewport()) {
		//요구 경험치이상을 보유해야 레벨업 가능
		LevelUpCount[SelectUINumber]++;
		if (GetOwnerCharacter()->GetPlayerStat().Exp < TotalExp + NextExp)
			LevelUpCount[SelectUINumber]--;
		else
			UpdateExp();
	}
}

int32 ULevelUPUI::GetChangePlayerExp()
{
	if (bUpdatedAbility == false) {
		return GetOwnerCharacter()->GetPlayerStat().Exp;
	}
	else return GetOwnerCharacter()->GetPlayerStat().Exp - TotalExp;

}

int32 ULevelUPUI::GetPlayerNextExp()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  StaticCast<int32>
			(GameInstance->GetCharAbilityData(GetChangePlayerLevel() + 1)->LevelUpExp);
	}
	return NextExp;
}

int32 ULevelUPUI::GetChangePlayerLevel()
{
	int32 sum = 0;
	for (const auto[type,cnt] : LevelUpCount)
		sum += cnt;

	return sum + GetOwnerCharacter()->GetPlayerStat().Level;
}

int32 ULevelUPUI::GetChangeMaxHP()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  StaticCast<int32>
			(GameInstance->GetCharAbilityData(GetChangeVigor())->TotalHP);
	}
	return GetOwnerCharacter()->GetPlayerStat().MaxHP;
}

int32 ULevelUPUI::GetChangeMaxStamina()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  StaticCast<int32>
			(GameInstance->GetCharAbilityData(GetChangeEnduarance())->TotalStamina);
	}
	return GetOwnerCharacter()->GetPlayerStat().MaxStamina;
}

int32 ULevelUPUI::GetChangePlayerDamage()
{
	if (GameInstance && bUpdatedAbility)
	{
		return  StaticCast<int32>
			(GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease +
				GetOwnerCharacter()->GetDamage());
	}
	return StaticCast<int32>(GetOwnerCharacter()->GetDamage());
}

void ULevelUPUI::OnClickOKBtn()
{
	OKUI = Cast<UDialogBoxUI>(CreateWidget(GetWorld(), OKUIIClass));

	if (OKUI)
	{
		if(!OKUI->ClickOKBtnFunction.IsBound())
			OKUI->ClickOKBtnFunction.BindUFunction(this, "LevelUp");
		OKUI->AddToViewport();
	}
}

void ULevelUPUI::LevelUp()
{
	FPlayerStatus ChangeData = {
		StaticCast<float>(GetChangeMaxHP()),
		StaticCast<float>(GetChangeMaxHP()),
		StaticCast<float>(GetChangeMaxStamina()),
		StaticCast<float>(GetChangeMaxStamina()),
		GameInstance->GetCharAbilityData(GetChangeStrength())->TotalDmgIncrease,
		GetChangeVigor(),
		GetChangeEnduarance(),
		GetChangeStrength(),
		GetChangePlayerLevel(),
		GetChangePlayerExp()
	};

	GetOwnerCharacter()->LevelUp(ChangeData);
	TotalExp = 0;
	NextExp = GameInstance->GetCharAbilityData(GetOwnerCharacter()->GetPlayerLevel() + 1)->LevelUpExp;
	for (auto& [type, cnt] : LevelUpCount)
		cnt = 0;
	OKBtnDisable();
	bUpdatedAbility = false;
}

void ULevelUPUI::UpdateExp()
{
	int32 CurrentLevel = GetOwnerCharacter()->GetPlayerLevel();
	int32 TargetLevel = GetChangePlayerLevel();
	int32 delta_level = TargetLevel - CurrentLevel;

	if (delta_level > 0)
	{
		bUpdatedAbility = true;
		OKBtnEnable();
		int32 SumExp = 0;
		for (int i = CurrentLevel; i < TargetLevel; ++i)
		{
			SumExp += GameInstance->GetCharAbilityData(i + 1)->LevelUpExp;
		}
		TotalExp = SumExp;
	}
	else {
		OKBtnDisable();
		bUpdatedAbility = false;
		TotalExp = 0;
	}
	NextExp = StaticCast<int32>
		(GameInstance->GetCharAbilityData(GetChangePlayerLevel() + 1)->LevelUpExp);
}


