// Fill out your copyright notice in the Description page of Project Settings.


#include "KBFLWorldModule.h"

#include "FGBackgroundThread.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "BFL/KBFL_Asset.h"
#include "Engine/AssetManager.h"
#include "Registry/ModContentRegistry.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"
#include "Subsystems/KBFLContentCDOHelperSubsystem.h"
#include "Subsystems/KBFLCustomizerSubsystem.h"
#include "Subsystems/KBFLResourceNodeSubsystem.h"

DECLARE_LOG_CATEGORY_EXTERN( KBFLWorldModuleLog, Log, All );

DEFINE_LOG_CATEGORY( KBFLWorldModuleLog );

UKBFLWorldModule::UKBFLWorldModule( ) {
	bRootModule = false;

	mCDOInformationMap.Add( ELifecyclePhase::CONSTRUCTION, FKBFLCDOInformation( ) );
	mCDOInformationMap.Add( ELifecyclePhase::INITIALIZATION, FKBFLCDOInformation( ) );
	mCDOInformationMap.Add( ELifecyclePhase::POST_INITIALIZATION, FKBFLCDOInformation( ) );

	mPoolEntryToAdd.Add( FKBFLPool( ) );
}

FKBFLCDOInformation UKBFLWorldModule::GetCDOInformationFromPhase_Implementation( ELifecyclePhase Phase, bool& bHasPhase ) {
	if( mCDOInformationMap.Contains( Phase ) ) {
		return mCDOInformationMap[ Phase ];
	}
	return FKBFLCDOInformation( );
}

TArray< TSubclassOf< AActor > > UKBFLWorldModule::GetRemoveClasses_Implementation( ) {
	return mActorsToRemove;
}

TArray< TSubclassOf< UKBFLActorSpawnDescriptorBase > > UKBFLWorldModule::GetActorSpawnDescriptors_Implementation( ) {
	return mActorSpawnDescriptors;
}

TMap< TSubclassOf< UFGSwatchGroup >, TSubclassOf< UFGFactoryCustomizationDescriptor_Swatch > > UKBFLWorldModule::GetSwatchGroups_Implementation( ) {
	return mSwatchGroups;
}

TArray< FKBFLSwatchInformation > UKBFLWorldModule::GetSwatchDescriptionInformation_Implementation( ) {
	return mSwatchDescriptionInformation;
}

TArray< FKBFLMaterialDescriptorInformation > UKBFLWorldModule::GetMaterialInformation_Implementation( ) {
	return mMaterialInformation;
}

void UKBFLWorldModule::DispatchLifecycleEvent( ELifecyclePhase Phase ) {
	if( Phase == ELifecyclePhase::CONSTRUCTION ) {
		RegisterPoolSettings( );
		ConstructionPhase( );
	}

	if( Phase == ELifecyclePhase::INITIALIZATION ) {
		RegisterKBFLLogicContent( );
		InitPhase( );
	}

	if( Phase == ELifecyclePhase::POST_INITIALIZATION ) {
		PostInitPhase( );
	}

	Super::DispatchLifecycleEvent( Phase );
}

void UKBFLWorldModule::InitPhase_Implementation( ) {}

void UKBFLWorldModule::ConstructionPhase_Implementation( ) {}

void UKBFLWorldModule::PostInitPhase_Implementation( ) {}

void UKBFLWorldModule::RegisterKBFLLogicContent( ) {
	if( !mUseAssetRegistry ) {
		bScanForCDOsDone = true;
		return;
	}

	UWorld* WorldObject = GetWorld( );
	UModContentRegistry* ModContentRegistry = UModContentRegistry::Get( WorldObject );
	fgcheck( ModContentRegistry );

	UKBFLAssetDataSubsystem* AssetDataSub = UKBFLAssetDataSubsystem::Get( WorldObject );
	fgcheck( AssetDataSub );

	FKBFLAssetData Datas = AssetDataSub->GetModRelatedData( this );

	if( mRegisterSchematics ) {
		for( UClass* Class : Datas.mAllFoundedSchematics ) {
			if( TSubclassOf< UFGSchematic > SchematicClass = Class ) {
				if( IsAllowedToRegister( SchematicClass ) ) {
					//UE_LOG(KBFLWorldModuleLog, Warning, TEXT("Register Schematic (%s) in ModContentRegistry"), *SchematicClass->GetName());
					ModContentRegistry->RegisterSchematic( GetOwnerModReference( ), SchematicClass );

					// is a fix for SF+ Content remover!
					mSchematics.AddUnique( SchematicClass );
				}
				else {
					UE_LOG( KBFLWorldModuleLog, Warning, TEXT("Cancle Register Schematic (%s) in ModContentRegistry because it not allowed!"), *SchematicClass->GetName() );
				}
			}
		}
	}

	if( mRegisterResearchTrees ) {
		for( UClass* Class : Datas.mAllFoundResearchTrees ) {
			if( TSubclassOf< UFGResearchTree > ResearchTreeClass = Class ) {
				if( IsAllowedToRegister( ResearchTreeClass ) ) {
					//UE_LOG( KBFLWorldModuleLog, Warning, TEXT("Register ResearchTrees (%s) in ModContentRegistry"), *ResearchTreeClass->GetName() );
					ModContentRegistry->RegisterResearchTree( GetOwnerModReference( ), ResearchTreeClass );

					// is a fix for SF+ Content remover!
					mResearchTrees.AddUnique( ResearchTreeClass );
				}
				else {
					UE_LOG( KBFLWorldModuleLog, Warning, TEXT("Cancle Register ResearchTree (%s) in ModContentRegistry because it not allowed!"), *ResearchTreeClass->GetName() );
				}
			}
		}
	}

	if( mRegisterRecipes ) {
		for( UClass* Class : Datas.mAllFoundedRecipes ) {
			if( TSubclassOf< UFGRecipe > RecipeClass = Class ) {
				if( IsAllowedToRegister( RecipeClass ) ) {
					//UE_LOG(KBFLWorldModuleLog, Warning, TEXT("Register Schematic (%s) in ModContentRegistry"), *SchematicClass->GetName());
					ModContentRegistry->RegisterRecipe( GetOwnerModReference( ), RecipeClass );
				}
				else {
					UE_LOG( KBFLWorldModuleLog, Warning, TEXT("Cancle Register Recipe (%s) in ModContentRegistry because it not allowed!"), *RecipeClass->GetName() );
				}
			}
		}
	}
}

void UKBFLWorldModule::RegisterPoolSettings( ) {
	UKBFLContentCDOHelperSubsystem* CDoSub = UKBFLContentCDOHelperSubsystem::Get( GetWorld( ) );
	if( mAddPoolEntry && mPoolEntryToAdd.Num( ) > 0 && CDoSub ) {
		if( UFGPoolSettings* Settings = GetMutableDefault< UFGPoolSettings >( ) ) {
			CDoSub->StoreObject( Settings );
			CDoSub->StoreClass( UFGPoolSettings::StaticClass( ) );
			for( FKBFLPool PoolEntry : mPoolEntryToAdd ) {
				if( !IsPoolEntryThere( Settings->PoolEntry, PoolEntry ) ) {
					Settings->PoolEntry.Add( PoolEntry.ToFg( ) );
					CDoSub->StoreObject( PoolEntry.mProxyComponent );
					CDoSub->StoreObject( PoolEntry.mVisual_Mesh );
				}
			}
		}
	}
}

bool UKBFLWorldModule::IsAllowedToRegister( TSubclassOf< UObject > Object ) const {
	if( IsValid( Object ) ) {
		return !mBlacklistedClasses.Contains( Object );
	}
	return false;
}

bool UKBFLWorldModule::IsPoolEntryThere( TArray< FFGPoolType > Source, FKBFLPool CheckStruc ) {
	if( !CheckStruc.mProxyComponent ) {
		return true;
	}

	if( !CheckStruc.mVisual_Mesh ) {
		return true;
	}

	for( FFGPoolType PoolType : Source ) {
		if( PoolType.mProxyComponent->IsValidLowLevel( ) && CheckStruc.mProxyComponent->IsValidLowLevel( ) ) {
			if( PoolType.mProxyComponent == CheckStruc.mProxyComponent && CheckStruc.mProxyComponent ) {
				UE_LOG( KBFLWorldModuleLog, Warning, TEXT("Ignore Pool Entry because is allready there: %s"), *CheckStruc.mProxyComponent->GetName() );
				return true;
			}
		}
	}
	return false;
}

void UKBFLWorldModule::FindAllCDOs( ) {
	if( !mUseAssetRegistry || !mRegisterCDOs ) {
		bScanForCDOsDone = true;
		return;
	}
	UKBFLAssetDataSubsystem* AssetDataSub = UKBFLAssetDataSubsystem::Get( GetWorld( ) );
	fgcheck( AssetDataSub );

	constexpr ELifecyclePhase CdoPhase = ELifecyclePhase::CONSTRUCTION;
	FKBFLAssetData Datas = AssetDataSub->GetModRelatedData( this );
	if( !mCDOInformationMap.Find( CdoPhase ) ) {
		mCDOInformationMap.Add( CdoPhase, FKBFLCDOInformation( ) );
	}
	FKBFLCDOInformation* CDOInfo = mCDOInformationMap.Find( CdoPhase );
	fgcheck( CDOInfo );

	for( UClass* CDOHelper : Datas.mAllFoundedCDOHelpers ) {
		if( TSubclassOf< UKBFL_CDOHelperClass_Base > CDOHelperClass = CDOHelper ) {
			if( mBlacklistedCDOClasses.Contains( CDOHelperClass ) ) {
				continue;
			}
			UE_LOG( KBFLWorldModuleLog, Warning, TEXT("Found CDO helper (%s) and add to map"), *CDOHelperClass->GetName() );
			CDOInfo->mCDOHelperClasses.AddUnique( CDOHelperClass );
		}
	}
	bScanForCDOsDone = true;
}
