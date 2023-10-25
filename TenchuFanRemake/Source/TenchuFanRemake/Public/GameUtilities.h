#pragma once

#include "CustomEnums.h"

class FName;

class GameUtilities
{
public:
	static FName GetStealthEventSectionName(int SectionIndex);
	static FName GetRandomStealthAnimationSection();
	static EEnemyDeathPose GetDeathPose(int SectionIndex);
};
