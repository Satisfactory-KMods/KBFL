// Copyright Coffee Stain Studios. All Rights Reserved.

#include "KBFLGameInstanceModule.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "BFL/KBFL_Asset.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"
#include "Subsystems/KBFLContentCDOHelperSubsystem.h"

DECLARE_LOG_CATEGORY_EXTERN( KBFLGameInstanceModuleLog, Log, All );

DEFINE_LOG_CATEGORY( KBFLGameInstanceModuleLog );

UKBFLGameInstanceModule::UKBFLGameInstanceModule()
{
	bRootModule = false;

	mCDOInformationMap.Add( ELifecyclePhase::CONSTRUCTION, FKBFLCDOInformation() );
	mCDOInformationMap.Add( ELifecyclePhase::INITIALIZATION, FKBFLCDOInformation() );
	mCDOInformationMap.Add( ELifecyclePhase::POST_INITIALIZATION, FKBFLCDOInformation() );

	mAssetCdoFinder.Add( FKBFLCdoRegistry( "/<yourmodref>/<path to folder>" ) );
}

void UKBFLGameInstanceModule::FindAllCDOs()
{
	if( !bScanForCDOsDone && mUseAssetCDOSearch && mAssetCdoFinder.Num() > 0 )
	{
		UE_LOG( KBFLGameInstanceModuleLog, Warning, TEXT("Option to find CDO Assets is enabled, try to find assets for world mod: %s"), *GetOwnerModReference().ToString() );

		const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >( AssetRegistryConstants::ModuleName );
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

		TArray< FAssetData > AssetDatas;
		for( FKBFLCdoRegistry Options : mAssetCdoFinder )
		{
			AssetDatas.Empty();

			if( AssetRegistry.GetAssetsByPath( Options.GetPath(), AssetDatas, Options.Recursive ) )
			{
				for( FAssetData AssetData : AssetDatas )
				{
					if( UKBFLAssetDataSubsystem::FilterAsset( AssetData ) )
					{
						TSubclassOf< UKBFL_CDOHelperClass_Base > CDOHelperClass;
						if( UKBFL_Asset::GetSubclassFromAsset< UKBFL_CDOHelperClass_Base >( AssetData, CDOHelperClass ) )
						{
							if( Options.bFindCDOHelpers )
							{
								if( !CDOHelperClass->IsChildOf( UKBFL_CDOHelperClass_RemoverBase::StaticClass() ) && Options.bFindCDOHelpers || CDOHelperClass->IsChildOf( UKBFL_CDOHelperClass_RemoverBase::StaticClass() ) && Options.bFindRemover )
								{
									UE_LOG( KBFLGameInstanceModuleLog, Warning, TEXT("Found CDO helper (%s) and add to map"), *CDOHelperClass->GetName() );
									if( mCDOInformationMap.Contains( Options.bPutInPhase ) )
									{
										mCDOInformationMap[ Options.bPutInPhase ].mCDOHelperClasses.AddUnique( CDOHelperClass );
									}
									else
									{
										FKBFLCDOInformation Information = FKBFLCDOInformation();
										Information.mCDOHelperClasses.AddUnique( CDOHelperClass );
										mCDOInformationMap.Add( Options.bPutInPhase, Information );
									}
								}
							}
						}
					}
				}
			}
		}
		bScanForCDOsDone = true;
	}
}

FKBFLCDOInformation UKBFLGameInstanceModule::GetCDOInformationFromPhase_Implementation( ELifecyclePhase Phase, bool& HasPhase )
{
	HasPhase = mCDOInformationMap.Contains( Phase );
	if( HasPhase )
	{
		return mCDOInformationMap[ Phase ];
	}
	return FKBFLCDOInformation();
}

void UKBFLGameInstanceModule::DispatchLifecycleEvent( ELifecyclePhase Phase )
{
	if( Phase == ELifecyclePhase::CONSTRUCTION )
	{
		ConstructionPhase();
		FindAllCDOs();
		GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::ConstructionPhase_Delayed );
	}

	if( Phase == ELifecyclePhase::INITIALIZATION )
	{
		InitPhase();
		GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::InitPhase_Delayed );
	}

	if( Phase == ELifecyclePhase::POST_INITIALIZATION )
	{
		PostInitPhase();
		GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::PostInitPhase_Delayed );
	}

	Super::DispatchLifecycleEvent( Phase );
}

void UKBFLGameInstanceModule::ConstructionPhase_Delayed()
{
	if( UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = GetWorld()->GetGameInstance()->GetSubsystem< UKBFLContentCDOHelperSubsystem >() )
	{
		if( !Called.Contains( ELifecyclePhase::CONSTRUCTION ) )
		{
			Called.Add( ELifecyclePhase::CONSTRUCTION );
			UE_LOG( KBFLGameInstanceModuleLog, Log, TEXT("Begin CDO call for %s as phase CONSTRUCTION"), *GetOwnerModReference().ToString() );
			CDOHelperSubsystem->BeginCDOForModule( this, ELifecyclePhase::CONSTRUCTION );
		}
	}
	else
	{
		UE_LOG( KBFLGameInstanceModuleLog, Log, TEXT("WARNING INVALID CDOHelperSubsystem : %s as phase ELifecyclePhase::CONSTRUCTION"), *GetOwnerModReference().ToString() );
		GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::ConstructionPhase_Delayed );
	}
}

void UKBFLGameInstanceModule::InitPhase_Delayed()
{
	if( UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = GetWorld()->GetGameInstance()->GetSubsystem< UKBFLContentCDOHelperSubsystem >() )
	{
		if( !Called.Contains( ELifecyclePhase::INITIALIZATION ) )
		{
			Called.Add( ELifecyclePhase::INITIALIZATION );
			UE_LOG( KBFLGameInstanceModuleLog, Log, TEXT("Begin CDO call for %s as phase INITIALIZATION"), *GetOwnerModReference().ToString() );
			CDOHelperSubsystem->BeginCDOForModule( this, ELifecyclePhase::INITIALIZATION );
		}
	}
	else
	{
		UE_LOG( KBFLGameInstanceModuleLog, Log, TEXT("WARNING INVALID CDOHelperSubsystem : %s as phase INITIALIZATION"), *GetOwnerModReference().ToString() );
		GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::InitPhase_Delayed );
	}
}

void UKBFLGameInstanceModule::PostInitPhase_Delayed()
{
	if( UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = GetWorld()->GetGameInstance()->GetSubsystem< UKBFLContentCDOHelperSubsystem >() )
	{
		if( !Called.Contains( ELifecyclePhase::POST_INITIALIZATION ) )
		{
			Called.Add( ELifecyclePhase::POST_INITIALIZATION );
			UE_LOG( KBFLGameInstanceModuleLog, Log, TEXT("Begin CDO call for %s as phase POST_INITIALIZATION"), *GetOwnerModReference().ToString() );
			CDOHelperSubsystem->BeginCDOForModule( this, ELifecyclePhase::POST_INITIALIZATION );
		}
	}
	else
	{
		UE_LOG( KBFLGameInstanceModuleLog, Log, TEXT("WARNING INVALID CDOHelperSubsystem : %s as phase POST_INITIALIZATION"), *GetOwnerModReference().ToString() );
		GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::PostInitPhase_Delayed );
	}
}

void UKBFLGameInstanceModule::PostInitPhase_Implementation()
{
}

void UKBFLGameInstanceModule::InitPhase_Implementation()
{
}

void UKBFLGameInstanceModule::ConstructionPhase_Implementation()
{
}
