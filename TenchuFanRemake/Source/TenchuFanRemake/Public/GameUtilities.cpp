#include "GameUtilities.h"

#include "CoreMinimal.h"

FName GameUtilities::GetStealthEventSectionName(int SectionIndex, bool WithSword)
{
	FName SectionName;

	if (WithSword)
	{
		switch (SectionIndex)
		{
		case 1:
			SectionName = "Behind1";
			break;
		case 2:
			SectionName = "Behind2";
			break;
		case 3:
			SectionName = "Front1";
			break;

		default:
			break;
		}
	}
	else {
		switch (SectionIndex)
		{
		case 1:
			SectionName = "Choke";
			break;
		case 2:
			SectionName = "BreakNeck";
			break;

		default:
			break;
		}
	}

	return SectionName;
}

FName GameUtilities::GetRandomStealthAnimationSection()
{
	int SectionIndex = FMath::RandRange(1, 2);
	return GetStealthEventSectionName(SectionIndex);
}

EEnemyDeathPose GameUtilities::GetDeathPose(int SectionIndex, bool WithSword)
{
	if (WithSword)
	{
		switch (SectionIndex)
		{
		case 1:
			return EEnemyDeathPose::EDP_Pose1;

		case 2:
			return EEnemyDeathPose::EDP_Pose2;

		case 3:
			return EEnemyDeathPose::EDP_Pose3;

		default:
			break;
		}
	}
	else {
		switch (SectionIndex)
		{
		case 1:
			return EEnemyDeathPose::EDP_ChockedPose;

		case 2:
			return EEnemyDeathPose::EDP_BrokenNeckPose;

		default:
			break;
		}
	}

	return EEnemyDeathPose::EDP_Pose1;
}

EEnemyDeathPose GameUtilities::GetStealthDeathFrontPose(int SectionIndex)
{
	switch (SectionIndex)
	{
	case 1:
		return EEnemyDeathPose::EDP_Pose3;

	case 2:
		return EEnemyDeathPose::EDP_4XSlashPose;

	default:
		break;
	}

	return EEnemyDeathPose::EDP_Pose3;
}
