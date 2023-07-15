#pragma once

#include "CoreMinimal.h"
#include "KBFL_Struct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Resources/FGItemDescriptorNuclearFuel.h"

#include "KBFL_Items.generated.h"

UCLASS()
class KBFL_API UKBFL_Items: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	public:
		/** Get all Desc Items in game */
		UFUNCTION(BlueprintPure, Category="KMods|Schematics", meta = (WorldContext = "WorldContext"))
		static TArray<FKBFLDescInfo> GetAllRegisteredDescWithInfos(UObject* WorldContext);
};
