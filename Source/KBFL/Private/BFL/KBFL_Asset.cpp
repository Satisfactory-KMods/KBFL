#include "BFL/KBFL_Asset.h"

#include "FGRecipe.h"
#include "FGSchematic.h"
#include "AssetRegistry/AssetRegistryModule.h"

DECLARE_LOG_CATEGORY_EXTERN( KBFLAssetLog, Log, All )

DEFINE_LOG_CATEGORY( KBFLAssetLog )

template< class T >
bool UKBFL_Asset::GetSubclassFromAsset( FAssetData AssetData, TSubclassOf< T >& OutClass ) {
	if( AssetData.IsValid( ) ) {
		if( !AssetData.AssetClassPath.GetAssetName( ).ToString( ).Contains( "BlueprintGeneratedClass" ) ) {
			UE_LOG( KBFLAssetLog, Log, TEXT("!Blueprint") );
			return false;
		}

		TSoftClassPtr< UObject > SoftClass = TSoftClassPtr( FSoftObjectPath( AssetData.GetObjectPathString( ) ) );
		if( SoftClass.IsPending( ) || SoftClass.IsValid( ) ) {
			UClass* Test = SoftClass.LoadSynchronous( );
			if( Test ) {
				if( Test->IsChildOf( T::StaticClass( ) ) ) {
					OutClass = Test;
					return IsValid( OutClass );
				}
			}
			return false;
		}
	}
	return false;
}

bool UKBFL_Asset::GetSubclassFromAsset( FAssetData AssetData, TSubclassOf< UObject >& OutClass ) {
	return GetSubclassFromAsset< UObject >( AssetData, OutClass );
}

bool UKBFL_Asset::FindChildsByClass( TArray< TSubclassOf< UObject > >& OutClasses, TSubclassOf< UObject > Search, bool bSearchSubClasses ) {
	OutClasses.Empty( );
	if( !IsValid( Search ) ) {
		return false;
	}

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >( AssetRegistryConstants::ModuleName );
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get( );
	TArray< FAssetData > AssetData;

	if( AssetRegistry.GetAssetsByClass( Search->GetFName( ), AssetData, bSearchSubClasses ) ) {
		for( FAssetData Data : AssetData ) {
			TSubclassOf< UObject > FoundClass;
			if( GetSubclassFromAsset( Data, FoundClass ) ) {
				if( IsValid( FoundClass ) && ( FoundClass->IsChildOf( Search ) || FoundClass == Search ) ) {
					OutClasses.AddUnique( FoundClass );
				}
			}
		}
		return OutClasses.Num( ) > 0;
	}
	return false;
}

void UKBFL_Asset::TryToEnableAllLocOnTheseClass( UClass* Class ) {
	if( IsValid( Class ) ) {
		UObject* Obj = GetMutableDefault< UObject >( Class );
		if( UFGItemDescriptor* ItemDesc = Cast< UFGItemDescriptor >( Obj ) ) {
			if( !ItemDesc->mDisplayName.IsEmpty( ) ) {
				ItemDesc->mDisplayName = ItemDesc->mDisplayName;
			}

			if( !ItemDesc->mDescription.IsEmpty( ) ) {
				ItemDesc->mDescription = ItemDesc->mDescription;
			}

			ItemDesc->MarkPackageDirty( );
		}

		if( UFGSchematic* Schematic = Cast< UFGSchematic >( Obj ) ) {
			if( !Schematic->mDisplayName.IsEmpty( ) ) {
				Schematic->mDisplayName = Schematic->mDisplayName;
			}

			if( !Schematic->mDescription.IsEmpty( ) ) {
				Schematic->mDescription = Schematic->mDescription;
			}

			Schematic->MarkPackageDirty( );
		}

		if( UFGRecipe* Recipe = Cast< UFGRecipe >( Obj ) ) {
			if( !Recipe->mDisplayName.IsEmpty( ) ) {
				Recipe->mDisplayName = Recipe->mDisplayName;
			}

			Recipe->MarkPackageDirty( );
		}
	}
}

void UKBFL_Asset::MarkDefaultDirty( UClass* Class ) {
	if( IsValid( Class ) ) {
		Class->MarkPackageDirty( );
		Class->GetDefaultObject( )->MarkPackageDirty( );
	}
}
