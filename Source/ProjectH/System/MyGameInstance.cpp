#include "System/MyGameInstance.h"
#include "DataTable/CharacterAbilityTables.h"
#include "Global.h"

UMyGameInstance::UMyGameInstance()
{
}

FCharacterAbilityData* UMyGameInstance::GetCharAbilityData(int32 Level)
{
	return CharacterAbilityData->FindRow<FCharacterAbilityData>(*FString::FromInt(Level), TEXT(""));
}
