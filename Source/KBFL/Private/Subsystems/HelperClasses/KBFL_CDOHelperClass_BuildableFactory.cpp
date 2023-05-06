#pragma once
#include "Subsystems/HelperClasses/KBFL_CDOHelperClass_BuildableFactory.h"

#include "FGPowerInfoComponent.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableFactory.h"


DECLARE_LOG_CATEGORY_EXTERN( CDOHelperItemsLog, Log, All )

DEFINE_LOG_CATEGORY( CDOHelperItemsLog )

void UKBFL_CDOHelperClass_BuildableFactory::DoCDO()
{
	for( TSubclassOf< AFGBuildableFactory > Class : GetClasses() )
	{
		if( Class )
		{
			if( AFGBuildableFactory* DefaultObject = Class.GetDefaultObject() )
			{
				if( mPowerConsumptionOverride )
				{
					DefaultObject->mPowerConsumption = this->mPowerConsumption;
				}

				if( mPowerConsumptionExponentOverride )
				{
					DefaultObject->mPowerConsumptionExponent = this->mPowerConsumptionExponent;
				}

				if( mPowerInfoClassOverride )
				{
					if( IsValid( mPowerInfoClass ) )
					{
						//mPowerInfoClass->AddToRoot();
					}

					DefaultObject->mPowerInfoClass = this->mPowerInfoClass;
				}

				if( mDoesHaveShutdownAnimationOverride )
				{
					DefaultObject->mDoesHaveShutdownAnimation = this->mDoesHaveShutdownAnimation;
				}

				if( mMinimumProducingTimeOverride )
				{
					DefaultObject->mMinimumProducingTime = this->mMinimumProducingTime;
				}

				if( mMinimumStoppedTimeOverride )
				{
					DefaultObject->mMinimumStoppedTime = this->mMinimumStoppedTime;
				}

				if( mCanChangePotentialnOverride )
				{
					DefaultObject->mCanChangePotential = this->mCanChangePotential;
				}

				if( mMinPotentialOverride )
				{
					DefaultObject->mMinPotential = this->mMinPotential;
				}

				if( mMaxPotentialOverride )
				{
					DefaultObject->mMaxPotential = this->mMaxPotential;
				}

				if( mMaxPotentialIncreasePerCrystalOverride )
				{
					DefaultObject->mMaxPotentialIncreasePerCrystal = this->mMaxPotentialIncreasePerCrystal;
				}

				if( mFluidStackSizeDefaultOverride )
				{
					DefaultObject->mFluidStackSizeDefault = this->mFluidStackSizeDefault;
				}

				if( mAddToSignificanceManagerOverride )
				{
					DefaultObject->mAddToSignificanceManager = this->mAddToSignificanceManager;
				}

				if( mSignificanceRangeOverride )
				{
					DefaultObject->mSignificanceRange = this->mSignificanceRange;
				}
			}
		}
	}

	Super::DoCDO();
}
