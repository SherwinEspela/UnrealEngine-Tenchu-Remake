#include "GameUtilities.h"

#include "CoreMinimal.h"

FName GameUtilities::GetStealthEventSectionName(int SectionIndex)
{
	FName SectionName;
	switch (SectionIndex)
	{
	case 1:
		SectionName = "Behind1";
		break;
	case 2:
		SectionName = "Behind2";
		break;
	default:
		break;
	}

	return SectionName;
}

FName GameUtilities::GetRandomStealthAnimationSection()
{
	int SectionIndex = FMath::RandRange(1, 2);
	return GetStealthEventSectionName(SectionIndex);}

EEnemyDeathPose GameUtilities::GetDeathPose(int SectionIndex)
{
	switch (SectionIndex)
	{
		case 1:
			return EEnemyDeathPose::EDP_Pose1;
	
		case 2:
			return EEnemyDeathPose::EDP_Pose2;

		default:
			break;
	}

	return EEnemyDeathPose::EDP_Pose1;
}
