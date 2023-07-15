#pragma once

#include "CoreMinimal.h"
#include "KBFL_CDOHelperClass_Buildable.h"
#include "KBFL_CDOHelperClass_BuildableFactory.generated.h"

/**
* This CDO should call in the Main Menu!
*/
UCLASS()
class KBFL_API UKBFL_CDOHelperClass_BuildableFactory: public UKBFL_CDOHelperClass_Buildable {
	GENERATED_BODY()

	public:
		virtual void DoCDO() override;

		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mPowerConsumptionOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mPowerConsumptionOverride ), Category="FGBuildableFactory")
		float mPowerConsumption;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mPowerConsumptionExponentOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mPowerConsumptionOverride ), Category="FGBuildableFactory")
		float mPowerConsumptionExponent;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mPowerInfoClassOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mPowerInfoClassOverride ), Category="FGBuildableFactory")
		TSubclassOf<class UFGPowerInfoComponent> mPowerInfoClass;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mDoesHaveShutdownAnimationOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mDoesHaveShutdownAnimationOverride ), Category="FGBuildableFactory")
		bool mDoesHaveShutdownAnimation;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mMinimumProducingTimeOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mMinimumProducingTimeOverride ), Category="FGBuildableFactory")
		float mMinimumProducingTime;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mMinimumStoppedTimeOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mMinimumStoppedTimeOverride ), Category="FGBuildableFactory")
		float mMinimumStoppedTime;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mCanChangePotentialnOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mCanChangePotentialnOverride ), Category="FGBuildableFactory")
		bool mCanChangePotential;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mMinPotentialOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mMinPotentialOverride ), Category="FGBuildableFactory")
		float mMinPotential;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mMaxPotentialOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mMaxPotentialOverride ), Category="FGBuildableFactory")
		float mMaxPotential;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mMaxPotentialIncreasePerCrystalOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mMaxPotentialIncreasePerCrystalOverride ), Category="FGBuildableFactory")
		float mMaxPotentialIncreasePerCrystal;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mFluidStackSizeDefaultOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mFluidStackSizeDefaultOverride ), Category="FGBuildableFactory")
		EStackSize mFluidStackSizeDefault = EStackSize::SS_FLUID;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mAddToSignificanceManagerOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mAddToSignificanceManagerOverride ), Category="FGBuildableFactory")
		bool mAddToSignificanceManager = true;
		//-------------------------------------------------------------------------------------------------------------------------------------
		UPROPERTY(meta=(NoAutoJson = true))
		bool mSignificanceRangeOverride;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( EditCondition = mSignificanceRangeOverride ), Category="FGBuildableFactory")
		float mSignificanceRange;
		//-------------------------------------------------------------------------------------------------------------------------------------
};
