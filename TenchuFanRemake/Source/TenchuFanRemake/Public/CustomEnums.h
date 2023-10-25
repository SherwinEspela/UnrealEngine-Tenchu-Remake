#pragma once

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	ES_Alive	UMETA(DisplayName = "Alive"),
	ES_Dead		UMETA(DisplayName = "Dead"),
	ES_Roaming	UMETA(DisplayName = "Roaming"),
	ES_Alert	UMETA(DisplayName = "Alert")
};

UENUM(BlueprintType)
enum class ETenchuPlayerStates : uint8
{
	EPS_Idle				UMETA(DisplayName = "Idle"),
	EPS_Running				UMETA(DisplayName = "Running"),
	EPS_Croucing			UMETA(DisplayName = "Crouching"),
	EPS_StealthAttacking	UMETA(DisplayName = "Stealth Attacking")
};

UENUM(BlueprintType)
enum class EEnemyDeathPose : uint8
{
	EDP_Pose1	UMETA(DisplayName = "Death Pose 1"),
	EDP_Pose2	UMETA(DisplayName = "Death Pose 2")
};
