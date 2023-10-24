#pragma once

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	ES_Alive UMETA(DisplayName = "Alive"),
	ES_Dead UMETA(DisplayName = "Dead"),
	ES_Roaming UMETA(DisplayName = "Roaming"),
	ES_Alert UMETA(DisplayName = "Alert")
};
