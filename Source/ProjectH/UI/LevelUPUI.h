#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUI.h"
#include "LevelUPUI.generated.h"

class UDialogBoxUI;
class UMyGameInstance;

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	EAT_None,
	EAT_Strength UMETA(DisplayName = "Strength"),
	EAT_Vigor UMETA(DisplayName = "Vigor"),
	EAT_Enduarance UMETA(DisplayName = "Enduarance"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PROJECTH_API ULevelUPUI : public UBaseUI
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SelectAbility();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OKBtnEnable();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OKBtnDisable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
		int32 GetChangeStrength();
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
		int32 GetChangeVigor();
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
		int32 GetChangeEnduarance();

private:


	UFUNCTION(BlueprintCallable)
		void OnUpKey();
	UFUNCTION(BlueprintCallable)
		void OnDownKey();
	UFUNCTION(BlueprintCallable)
		void OnLeftKey();
	UFUNCTION(BlueprintCallable)
		void OnRightKey();

	UFUNCTION(BlueprintPure)
		int32 GetChangePlayerExp();
	UFUNCTION(BlueprintPure)
		int32 GetPlayerNextExp();
	UFUNCTION(BlueprintPure)
		int32 GetChangePlayerLevel();

	UFUNCTION(BlueprintPure)
		int32 GetChangeMaxHP();
	UFUNCTION(BlueprintPure)
		int32 GetChangeMaxStamina();
	UFUNCTION(BlueprintPure)
		int32 GetChangePlayerDamage();

	UFUNCTION(BlueprintCallable)
		void OnClickOKBtn();
	UFUNCTION()
		void LevelUp();
	UFUNCTION()
		void UpdateExp();

private:

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		UDialogBoxUI* OKUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UDialogBoxUI> OKUIIClass;

	UPROPERTY()
		UMyGameInstance* GameInstance;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		int32 AbilityType;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
		TMap<int32,int32> LevelUpCount;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		bool bUpdatedAbility = false;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
		int32 SelectUINumber;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		int32 TotalExp;
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		int32 NextExp;

};
