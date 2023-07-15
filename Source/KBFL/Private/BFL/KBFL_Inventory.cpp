#include "BFL/KBFL_Inventory.h"

void UKBFL_Inventory::FindFirstAllowedSlot(UFGInventoryComponent* Inventory, TSubclassOf<UFGItemDescriptor> Item, KBFLFoundSomething& Found, int32& Index) {
	Index = -1;
	Found = KBFLFoundSomething::NotFound;
	if(!Inventory->IsItemAllowed(Item)) {
		return;
	}

	const int SlotSizes = Inventory->GetSizeLinear();
	for(int i = 0; i < SlotSizes; i++) {
		if(!Inventory->IsValidIndex(i)) {
			break;
		}
		if(Inventory->GetAllowedItemOnIndex(i) == Item) {
			Index = i;
			break;
		}
	}

	if(Index != -1) {
		Found = KBFLFoundSomething::Found;
	}
}

void UKBFL_Inventory::FindAllAllowedSlots(UFGInventoryComponent* Inventory, TSubclassOf<UFGItemDescriptor> Item, KBFLFoundSomething& Found, TArray<int32>& Index) {
	Index = {};
	Found = KBFLFoundSomething::NotFound;
	if(!Inventory->IsItemAllowed(Item)) {
		return;
	}


	const int SlotSizes = Inventory->GetSizeLinear();
	for(int i = 0; i < SlotSizes; i++) {
		if(!Inventory->IsValidIndex(i)) {
			break;
		}
		if(Inventory->GetAllowedItemOnIndex(i) == Item) {
			Index.Add(i);
		}
	}


	if(Index.Num() > 0) {
		Found = KBFLFoundSomething::Found;
	}
}
