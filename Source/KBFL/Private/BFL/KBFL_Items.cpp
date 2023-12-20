#include "BFL/KBFL_Items.h"

#include "BFL/KBFL_Util.h"
#include "Kismet/KismetMathLibrary.h"
#include "Registry/ModContentRegistry.h"


TArray< FKBFLDescInfo > UKBFL_Items::GetAllRegisteredDescWithInfos( UObject* WorldContext ) {
	auto ModContent = UModContentRegistry::Get( WorldContext->GetWorld( ) );
	auto ItemDescriptors = ModContent->GetLoadedItemDescriptors( );
	TArray< FKBFLDescInfo > ReturnDescriptors = { };

	for( auto ItemDescriptor : ItemDescriptors ) {
		UClass* CastedClass = Cast< UClass >( ItemDescriptor.RegisteredObject );
		if( ensureAlways( CastedClass ) ) {
			FKBFLDescInfo Struc;
			Struc.mMod = ItemDescriptor.OwnedByModReference;
			Struc.mDesc = TSubclassOf< UFGItemDescriptor >( CastedClass );

			ReturnDescriptors.Add( Struc );
		}
	}

	return ReturnDescriptors;
}
