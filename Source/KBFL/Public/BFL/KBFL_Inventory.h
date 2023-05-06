#pragma once

#include "CoreMinimal.h"

#include "FGInventoryComponent.h"
#include "KBFL_Enum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KBFL_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class KBFL_API UKBFL_Inventory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Return the Index where the ItemClass allowed on Slot in Inventory */
	UFUNCTION( BlueprintCallable, Category="KMods|Items" )
	static void FindFirstAllowedSlot( UFGInventoryComponent* Inventory, TSubclassOf< UFGItemDescriptor > Item, KBFLFoundSomething& Found, int32& Index );

	/** Return all Index where the ItemClass allowed on Slot in Inventory */
	UFUNCTION( BlueprintCallable, Category="KMods|Items" )
	static void FindAllAllowedSlots( UFGInventoryComponent* Inventory, TSubclassOf< UFGItemDescriptor > Item, KBFLFoundSomething& Found, TArray< int32 >& Index );
};
