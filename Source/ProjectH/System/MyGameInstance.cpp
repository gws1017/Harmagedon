#include "System/MyGameInstance.h"
#include "Data/CharacterAbilityTables.h"
#include "Data/ItemData.h"
#include "Global.h"

UMyGameInstance::UMyGameInstance()
{
}

FCharacterAbilityData* UMyGameInstance::GetCharAbilityData(int32 Level)
{
	return CharacterAbilityData->FindRow<FCharacterAbilityData>(*FString::FromInt(Level), TEXT(""));
}

FItemData* UMyGameInstance::GetItemData(int64 ItemCode)
{
	return ItemData->FindRow<FItemData>(*FString::FromInt(ItemCode), TEXT(""));
}
