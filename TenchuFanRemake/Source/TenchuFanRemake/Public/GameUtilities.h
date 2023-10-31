#pragma once

#include "CustomEnums.h"

class FName;

class GameUtilities
{
public:
	static FName GetStealthEventSectionName(int SectionIndex, bool WithSword = true);
	static FName GetRandomStealthAnimationSection();
	static EEnemyDeathPose GetDeathPose(int SectionIndex, bool WithSword);
	static EEnemyDeathPose GetStealthDeathFrontPose(int SectionIndex);
};
