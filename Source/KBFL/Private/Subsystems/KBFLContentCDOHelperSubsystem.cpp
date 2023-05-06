#include "Subsystems/KBFLContentCDOHelperSubsystem.h"

#include "FGCheatManager.h"
#include "FGGameState.h"
#include "BFL/KBFL_Player.h"
#include "Module/WorldModuleManager.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"
#include "Subsystems/HelperClasses/KBFL_CDOHelperClass_Recipes.h"

DECLARE_LOG_CATEGORY_EXTERN( ContentCDOHelperSubsystem, Log, All )

DEFINE_LOG_CATEGORY( ContentCDOHelperSubsystem )

void UKBFLContentCDOHelperSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Collection.InitializeDependency( UKBFLAssetDataSubsystem::StaticClass() );

	Super::Initialize( Collection );

#if WITH_EDITOR
	return;
#endif
}

void UKBFLContentCDOHelperSubsystem::Deinitialize()
{
	Initialized = false;

	mModulesToCall.Empty();
	mCDOCalled.Empty();
	mCalledObjects.Empty();

	Super::Deinitialize();
}

UKBFLContentCDOHelperSubsystem* UKBFLContentCDOHelperSubsystem::Get( UObject* Context )
{
	if( IsValid( Context ) )
	{
		return Context->GetWorld()->GetGameInstance()->GetSubsystem< UKBFLContentCDOHelperSubsystem >();
	}
	return nullptr;
}

void UKBFLContentCDOHelperSubsystem::MoveRecipesFromBuilding( TSoftClassPtr< > From, TSoftClassPtr< > To, TArray< TSubclassOf< UFGItemCategory > > IgnoreCategory, TArray< TSubclassOf< UFGRecipe > > IgnoreRecipe )
{
	TSubclassOf< UObject > SubFrom = nullptr;
	TSubclassOf< UObject > SubTo = nullptr;

	if( From.IsPending() || To.IsValid() )
	{
		SubFrom = From.LoadSynchronous();
	}

	if( To.IsPending() || To.IsValid() )
	{
		SubTo = To.LoadSynchronous();
	}

	if( !SubFrom ) UE_LOG( LogTemp, Error, TEXT("CDO_MoveRecipesFromBuilding -> Cannot load From") );

	if( !SubTo ) UE_LOG( LogTemp, Error, TEXT("CDO_MoveRecipesFromBuilding -> Cannot load To") );

	if( SubTo && SubFrom )
	{
		UKBFLAssetDataSubsystem* AssetDataSubsystem = UKBFLAssetDataSubsystem::Get( GetWorld() );

		TArray< TSubclassOf< UFGRecipe > > Recipes;
		AssetDataSubsystem->GetRecipesOfProducer( { SubFrom }, Recipes );

		for( auto Recipe : Recipes )
		{
			if( IsValid( Recipe ) && !IgnoreRecipe.Contains( Recipe ) )
			{
				UFGRecipe* Default = GetAndStoreDefaultObject_Native< UFGRecipe >( Recipe );
				if( IsValid( Default ) )
				{
					if( !IgnoreCategory.Contains( Default->mOverriddenCategory ) || Default->mOverriddenCategory == nullptr )
					{
						Default->mProducedIn.Remove( From );
						Default->mProducedIn.Add( To );
						UE_LOG( LogTemp, Error, TEXT("CDO_MoveRecipesFromBuilding -> Move Recipe %s : %s -> %s"), *Default->GetName(), *From->GetName(), *To->GetName() );

						Default->MarkPackageDirty();
					}
				}
			}
		}
	}
}

void UKBFLContentCDOHelperSubsystem::BeginCDOForModule( UModModule* Module, ELifecyclePhase Phase )
{
	UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("BeginCDOForModule > Was Called - Mod: %s"), *Module->GetOwnerModReference().ToString() );
	if( UKismetSystemLibrary::DoesImplementInterface( Module, UKBFLContentCDOHelperInterface::StaticClass() ) )
	{
		if( !WasCDOForModuleCalled( Module, Phase ) )
		{
			UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("Try to get information for mod: %s"), *Module->GetOwnerModReference().ToString() );
			bool HasPhase;
			FKBFLCDOInformation Info = IKBFLContentCDOHelperInterface::Execute_GetCDOInformationFromPhase( Module, Phase, HasPhase );
			if( HasPhase )
			{
				UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("BeginCDOForModule > HasPhase (Start CDO on Phase: %s) - Mod: %s"), *Module->LifecyclePhaseToString(Phase), *Module->GetOwnerModReference().ToString() );
				DoCDOFromInfo( Info );
				if( !mCDOCalled.Contains( Module ) )
				{
					mCDOCalled.Add( Module );
				}
				mCDOCalled[ Module ].mCalledPhases.Add( Phase );
			}
			else
			{
				UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("Scrip phase - Mod: %s"), *Module->GetOwnerModReference().ToString() );
			}
		}
		else
		{
			UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("CDO Was Called for this phase: %s"), *Module->GetOwnerModReference().ToString() );
		}
	}
}

bool UKBFLContentCDOHelperSubsystem::WasCDOForModuleCalled( UModModule* Module, ELifecyclePhase Phase ) const
{
	if( Module )
	{
		if( mCDOCalled.Contains( Module ) )
		{
			return mCDOCalled[ Module ].mCalledPhases.Contains( Phase );
		}
	}
	return false;
}

void UKBFLContentCDOHelperSubsystem::ResetCDOCallFromModule( UModModule* Module )
{
	if( Module )
	{
		if( mCDOCalled.Contains( Module ) )
		{
			mCDOCalled[ Module ].mCalledPhases.Empty();
		}
	}
}

/*
void UKBFLContentCDOHelperSubsystem::OnWorldBeginPlay()
{
	if (!Initialized)
	{
		UE_LOG(ContentCDOHelperSubsystem, Log, TEXT("World Begin Play! ContentCDOHelperSubsystem > DoCDOs"));
		DoCDOs();
	
		Initialized = true;
	}
}

void UKBFLContentCDOHelperSubsystem::RedoCDOs()
{
	UE_LOG(ContentCDOHelperSubsystem, Log, TEXT("RedoCDOs! ContentCDOHelperSubsystem > RedoCDOs"));
	Initialized = false;
	mCDOCalled.Empty();
	OnWorldBeginPlay();
}

void UKBFLContentCDOHelperSubsystem::DoCDOs()
{
	#if !WITH_EDITOR
		UWorldModuleManager * Subsystem = Cast<UWorldModuleManager>(GetWorld()->GetSubsystem<UWorldModuleManager>());
		for(UWorldModule* Module : Subsystem->RootModuleList)
		{
			const bool bImplementsInterface = Module->GetClass()->ImplementsInterface(UKBFLContentCDOHelperInterface::StaticClass());
			if (bImplementsInterface)
			{
				UE_LOG(ContentCDOHelperSubsystem, Log, TEXT("DoCDOs > %s"), *Module->GetName());
				if(Module)
					for(int i = 0; i < 3; ++i)
					{
						ELifecyclePhase Phase = i == 0 ? ELifecyclePhase::CONSTRUCTION : i == 1 ? ELifecyclePhase::INITIALIZATION : ELifecyclePhase::POST_INITIALIZATION;
						UE_LOG(ContentCDOHelperSubsystem, Log, TEXT("DoCDOs > Phase > %d"), i);
						
						bool HasPhase;
						FKBFLCDOInformation Info = IKBFLContentCDOHelperInterface::Execute_GetCDOInformationFromPhase(Module, Phase, HasPhase);

						if(HasPhase && !WasCDOForModuleCalled(Module, Phase))
						{
							DoCDOFromInfo(Info);
						}
						else
						{
							UE_LOG(ContentCDOHelperSubsystem, Log, TEXT("DoCDOs > Phase > Cannot find Phase!"));
						}
					}
			}
		}
	#endif
}
*/

void UKBFLContentCDOHelperSubsystem::DoCDOFromInfo( FKBFLCDOInformation Info )
{
	UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("DoCDOFromInfo") );

	for( TSubclassOf< UKBFL_CDOHelperClass_Base > CDOHelper : Info.mCDOHelperClasses )
	{
		if( IsValid( CDOHelper ) )
		{
			CallCDOHelper( CDOHelper );
		}
	}

	for( auto CDO : Info.mItemStackSizeCDO )
	{
		for( auto Item : CDO.Value.mItems )
		{
			if( Item )
			{
				DoSetNewStackSize( Item, CDO.Key );
			}
		}
	}
}

void UKBFLContentCDOHelperSubsystem::DoSetNewStackSize( TSubclassOf< UFGItemDescriptor > Item, EStackSize StackSize )
{
	if( IsValid( Item ) )
	{
		if( UFGItemDescriptor* Default = GetAndStoreDefaultObject_Native< UFGItemDescriptor >( Item ) )
		{
			UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("ContentCDOHelperSubsystem > DoSetNewStackSize for item %s"), *Item->GetName() );
			Default->mStackSize = StackSize;
			Default->mCachedStackSize = UFGItemDescriptor::GetStackSize( Item );
			Default->MarkPackageDirty();;
		}
	}
}

void UKBFLContentCDOHelperSubsystem::CallCDOHelper( TSubclassOf< UKBFL_CDOHelperClass_Base > CDOHelperClass, bool IgnoreCallCheck )
{
	if( IsValid( CDOHelperClass ) )
	{
		if( UKBFL_CDOHelperClass_Base* CDOHelper = GetAndStoreDefaultObject_Native< UKBFL_CDOHelperClass_Base >( CDOHelperClass ) )
		{
			if( CDOHelper->ExecuteAllowed() || IgnoreCallCheck )
			{
				CDOHelper->mSubsystem = this;
				CDOHelper->ModifyValues();
				CDOHelper->DoCDO();
				CDOHelper->ExecuteBlueprintCDO();
				UE_LOG( ContentCDOHelperSubsystem, Log, TEXT("Do CDO Call for Helper: %s"), *CDOHelper->GetName() );
			}
		}
	}
}

UObject* UKBFLContentCDOHelperSubsystem::GetAndStoreDefaultObject( UClass* Class )
{
	if( IsValid( Class ) )
	{
		mCalledClasses.AddUnique( Class );
		mCalledObjects.AddUnique( Class->GetDefaultObject() );
		return Class->GetDefaultObject();
	}
	return nullptr;
}

UObject* UKBFLContentCDOHelperSubsystem::GetAndStoreCDO( UClass* Class, UObject* Context )
{
	if( UKBFLContentCDOHelperSubsystem* Sub = Get( Context ) )
	{
		return Sub->GetAndStoreDefaultObject( Class );
	}
	return nullptr;
}

void UKBFLContentCDOHelperSubsystem::StoreClass( UClass* Class )
{
	mCalledClasses.AddUnique( Class );
}

void UKBFLContentCDOHelperSubsystem::StoreObject( UObject* Object )
{
	mCalledObjects.AddUnique( Object );
}

void UKBFLContentCDOHelperSubsystem::RemoveClass( UClass* Class )
{
	mCalledClasses.Remove( Class );
}

void UKBFLContentCDOHelperSubsystem::RemoveObject( UObject* Object )
{
	mCalledObjects.Remove( Object );
}

TArray< UModModule* > UKBFLContentCDOHelperSubsystem::GetCalledModules() const
{
	TArray< UModModule* > Modules;
	mCDOCalled.GenerateKeyArray( Modules );
	return Modules;
}
