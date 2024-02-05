#include "System/MyGameInstance.h"
#include "Data/CharacterAbilityTables.h"
#include "Global.h"

UMyGameInstance::UMyGameInstance()
{
}

FCharacterAbilityData* UMyGameInstance::GetCharAbilityData(int32 Level)
{
	return CharacterAbilityData->FindRow<FCharacterAbilityData>(*FString::FromInt(Level), TEXT(""));
}
