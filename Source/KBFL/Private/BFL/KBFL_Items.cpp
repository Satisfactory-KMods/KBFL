#include "BFL/KBFL_Items.h"

#include "BFL/KBFL_Util.h"
#include "Kismet/KismetMathLibrary.h"
#include "Registry/ModContentRegistry.h"


TArray<FKBFLDescInfo> UKBFL_Items::GetAllRegisteredDescWithInfos(UObject* WorldContext) {
	auto                  ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
	auto                  ItemDescriptors = ModContent->GetLoadedItemDescriptors();
	TArray<FKBFLDescInfo> ReturnDescriptors = {};

	for(auto ItemDescriptor: ItemDescriptors) {
		FKBFLDescInfo Struc;
		Struc.mMod = ItemDescriptor.OwnedByModReference;
		Struc.mDesc = TSubclassOf<UFGItemDescriptor>(ItemDescriptor.RegisteredObject);

		ReturnDescriptors.Add(Struc);
	}

	return ReturnDescriptors;
}
