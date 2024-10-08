#pragma once
#include "Subsystems/HelperClasses/KBFL_CDOHelperClass_BuildableFactory.h"

#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableFactory.h"


DECLARE_LOG_CATEGORY_EXTERN(CDOHelperItemsLog, Log, All)

DEFINE_LOG_CATEGORY(CDOHelperItemsLog)

void UKBFL_CDOHelperClass_BuildableFactory::DoCDO()
{
	for (TSubclassOf<AFGBuildableFactory> Class : GetClasses())
	{
		if (Class)
		{
			if (AFGBuildableFactory* DefaultObject = Class.GetDefaultObject())
			{
				if (mPowerConsumptionOverride)
				{
					DefaultObject->mPowerConsumption = this->mPowerConsumption;
				}

				if (mPowerConsumptionExponentOverride)
				{
					DefaultObject->mPowerConsumptionExponent = this->mPowerConsumptionExponent;
				}

				if (mDoesHaveShutdownAnimationOverride)
				{
					DefaultObject->mDoesHaveShutdownAnimation = this->mDoesHaveShutdownAnimation;
				}

				if (mMinimumProducingTimeOverride)
				{
					DefaultObject->mMinimumProducingTime = this->mMinimumProducingTime;
				}

				if (mMinimumStoppedTimeOverride)
				{
					DefaultObject->mMinimumStoppedTime = this->mMinimumStoppedTime;
				}

				if (mCanChangePotentialnOverride)
				{
					DefaultObject->mCanChangePotential = this->mCanChangePotential;
				}

				if (mMinPotentialOverride)
				{
					DefaultObject->mMinPotential = this->mMinPotential;
				}

				if (mMaxPotentialOverride)
				{
					DefaultObject->mMaxPotential = this->mMaxPotential;
				}

				if (mBaseProductionBoostOverride)
				{
					DefaultObject->mBaseProductionBoost = this->mBaseProductionBoost;
				}

				if (mFluidStackSizeDefaultOverride)
				{
					DefaultObject->mFluidStackSizeDefault = this->mFluidStackSizeDefault;
				}

				if (mAddToSignificanceManagerOverride)
				{
					DefaultObject->mAddToSignificanceManager = this->mAddToSignificanceManager;
				}

				if (mSignificanceRangeOverride)
				{
					DefaultObject->mSignificanceRange = this->mSignificanceRange;
				}

				if (mPotentialShardSlotsOverride)
				{
					DefaultObject->mPotentialShardSlots = this->mPotentialShardSlots;
				}

				if (mProductionShardSlotSizeOverride)
				{
					DefaultObject->mProductionShardSlotSize = this->mProductionShardSlotSize;
				}

				if (mProductionShardBoostMultiplierOverride)
				{
					DefaultObject->mProductionShardBoostMultiplier = this->mProductionShardBoostMultiplier;
				}

				if (mOverridePotentialShardSlotsOverride)
				{
					DefaultObject->mOverridePotentialShardSlots = this->mOverridePotentialShardSlots;
				}

				if (mOverrideProductionShardSlotSizeOverride)
				{
					DefaultObject->mOverrideProductionShardSlotSize = this->mOverrideProductionShardSlotSize;
				}
			}
		}
	}

	Super::DoCDO();
}