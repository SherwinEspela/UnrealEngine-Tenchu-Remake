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
	EPS_Moving				UMETA(DisplayName = "Moving"),
	EPS_Croucing			UMETA(DisplayName = "Crouching"),
	EPS_StealthAttacking	UMETA(DisplayName = "Stealth Attacking"),
	EPS_Interacting			UMETA(DisplayName = "Interacting"),
	EPS_TakingCover			UMETA(DisplayName = "Taking Cover")
};

UENUM(BlueprintType)
enum class EEnemyDeathPose : uint8
{
	EDP_Pose1			UMETA(DisplayName = "Death Pose 1"),
	EDP_Pose2			UMETA(DisplayName = "Death Pose 2"),
	EDP_Pose3			UMETA(DisplayName = "Death Pose 3"),
	EDP_ChockedPose		UMETA(DisplayName = "Death Chocked Pose"),
	EDP_BrokenNeckPose	UMETA(DisplayName = "Death Broken Neck Pose"),
	EDP_4XSlashPose		UMETA(DisplayName = "Death 4X Slash Pose")
};

UENUM(BlueprintType)
enum class ETakeCoverType : uint8
{
	ETC_FacingLeft	UMETA(DisplayName = "Facing Left"),
	ETC_FacingRight	UMETA(DisplayName = "Facing Right")
};

UENUM(Blueprinttype)
enum class EInteractableType : uint8
{
	EIT_Enemy			UMETA(DisplayName = "Enemy"),
	EIT_TakeCoverBox	UMETA(DisplayName = "Take Cover Box")
};

UENUM(Blueprinttype)
enum class EEnemyIdleType : uint8
{
	EIT_Normal		UMETA(DisplayName = "Normal"),
	EIT_LoosenUp1	UMETA(DisplayName = "Loosen Up 1"),
	EIT_LoosenUp2	UMETA(DisplayName = "Loosen Up 2"),
	EIT_LoosenUp3	UMETA(DisplayName = "Loosen Up 3")
};
