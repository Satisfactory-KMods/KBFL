// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "FactoryGame.h"
#include "FGColoredInstanceMeshProxy.h"
#include "KBFLBPManageColoredInstance.generated.h"


/**
* OLD!!!
* Proxy placed in buildings to be replaced with an instance on creation, supports coloring.
*/
UCLASS(Blueprintable, meta = ( BlueprintSpawnableComponent ))
class KBFL_API UKBFLBPManageColoredInstance: public UFGColoredInstanceMeshProxy {
	GENERATED_BODY()

	public:
		UKBFLBPManageColoredInstance();

		UFUNCTION(BlueprintCallable, Category="KL Indicator")
		void UpdateDataMap(TMap<uint8 , float> DataMap);

		UFUNCTION(BlueprintCallable, Category="KL Indicator")
		void UpdateData(uint8 Index, float Value);

		UFUNCTION(BlueprintCallable, Category="KL Indicator")
		bool ReApplyData();

		UFUNCTION(BlueprintPure, Category="KL Indicator")
		bool CheckIndex(uint8 Index) const {
			if(mInstanceHandle.IsInstanced() && Index < mNumCustomDataFloats) {
				return true;
			}
			return true;
		}
};
